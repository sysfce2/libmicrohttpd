/* Feel free to use this example code in any way
   you see fit (Public Domain) */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <stdarg.h>
#include <microhttpd.h>

/* Emulate 'asprintf()', as it is not portable */
static int
MHD_asprintf (char **resultp, const char *format, ...)
{
  va_list argptr;
  va_list argcopy;
  int len;
  int ret;

  ret = -1;
  va_start (argptr, format);

  va_copy (argcopy, argptr);
#ifndef _WIN32
  len = vsnprintf (NULL, 0, format, argcopy);
#else
  len = _vscprintf (format, argcopy);
#endif
  va_end (argcopy);
  if (0 < len)
  {
    size_t buf_size;
    char *buf;

    buf_size = (size_t) len + 1;
    buf = (char *) malloc (buf_size * sizeof(char));
    if (NULL != buf)
    {
      int res;

#ifndef _WIN32
      res = vsnprintf (buf, buf_size, format, argptr);
#else
      res = _vsnprintf (buf, buf_size, format, argptr);
#endif
      if (len == res)
      {
        *resultp = buf;
        ret = res;
      }
      else
      {
        free (buf);
        *resultp = NULL;
      }
    }
  }
  va_end (argptr);
  return ret;
}


/**
 * Invalid method page.
 */
#define METHOD_ERROR \
  "<html><head><title>Illegal request</title></head><body>Go away.</body></html>"

/**
 * Invalid URL page.
 */
#define NOT_FOUND_ERROR \
  "<html><head><title>Not found</title></head><body>Go away.</body></html>"

/**
 * Front page. (/)
 */
#define MAIN_PAGE \
  "<html><head><title>Welcome</title></head><body><form action=\"/2\" method=\"post\">What is your name? <input type=\"text\" name=\"v1\" value=\"%s\" /><input type=\"submit\" value=\"Next\" /></body></html>"

/**
 * Second page. (/2)
 */
#define SECOND_PAGE \
  "<html><head><title>Tell me more</title></head><body><a href=\"/\">previous</a> <form action=\"/S\" method=\"post\">%s, what is your job? <input type=\"text\" name=\"v2\" value=\"%s\" /><input type=\"submit\" value=\"Next\" /></body></html>"

/**
 * Second page (/S)
 */
#define SUBMIT_PAGE \
  "<html><head><title>Ready to submit?</title></head><body><form action=\"/F\" method=\"post\"><a href=\"/2\">previous </a> <input type=\"hidden\" name=\"DONE\" value=\"yes\" /><input type=\"submit\" value=\"Submit\" /></body></html>"

/**
 * Last page.
 */
#define LAST_PAGE \
  "<html><head><title>Thank you</title></head><body>Thank you.</body></html>"

/**
 * Name of our cookie.
 */
#define COOKIE_NAME "session"


/**
 * State we keep for each user/session/browser.
 */
struct Session
{
  /**
   * We keep all sessions in a linked list.
   */
  struct Session *next;

  /**
   * Unique ID for this session.
   */
  char sid[33];

  /**
   * Reference counter giving the number of connections
   * currently using this session.
   */
  unsigned int rc;

  /**
   * Time when this session was last active.
   */
  time_t start;

  /**
   * String submitted via form.
   */
  char value_1[64];

  /**
   * Another value submitted via form.
   */
  char value_2[64];

};


/**
 * Data kept per request.
 */
struct Request
{

  /**
   * Associated session.
   */
  struct Session *session;

  /**
   * Post processor handling form data (IF this is
   * a POST request).
   */
  struct MHD_PostProcessor *pp;

  /**
   * URL to serve in response to this POST (if this request
   * was a 'POST')
   */
  const char *post_url;

};


/**
 * Linked list of all active sessions.  Yes, O(n) but a
 * hash table would be overkill for a simple example...
 */
static struct Session *sessions;


/**
 * Return the session handle for this connection, or
 * create one if this is a new user.
 */
static struct Session *
get_session (struct MHD_Connection *connection)
{
  struct Session *ret;
  const char *cookie;

  cookie = MHD_lookup_connection_value (connection,
                                        MHD_COOKIE_KIND,
                                        COOKIE_NAME);
  if (cookie != NULL)
  {
    /* find existing session */
    ret = sessions;
    while (NULL != ret)
    {
      if (0 == strcmp (cookie, ret->sid))
        break;
      ret = ret->next;
    }
    if (NULL != ret)
    {
      ret->rc++;
      return ret;
    }
  }
  /* create fresh session */
  ret = calloc (1, sizeof (struct Session));
  if (NULL == ret)
  {
    fprintf (stderr, "calloc error: %s\n", strerror (errno));
    return NULL;
  }
  /* not a super-secure way to generate a random session ID,
     but should do for a simple example... */
  snprintf (ret->sid,
            sizeof (ret->sid),
            "%X%X%X%X",
            (unsigned int) rand (),
            (unsigned int) rand (),
            (unsigned int) rand (),
            (unsigned int) rand ());
  ret->rc++;
  ret->start = time (NULL);
  ret->next = sessions;
  sessions = ret;
  return ret;
}


/**
 * Type of handler that generates a reply.
 *
 * @param cls content for the page (handler-specific)
 * @param mime mime type to use
 * @param session session information
 * @param connection connection to process
 * @param #MHD_YES on success, #MHD_NO on failure
 */
typedef enum MHD_Result (*PageHandler)(const void *cls,
                                       const char *mime,
                                       struct Session *session,
                                       struct MHD_Connection *connection);


/**
 * Entry we generate for each page served.
 */
struct Page
{
  /**
   * Acceptable URL for this page.
   */
  const char *url;

  /**
   * Mime type to set for the page.
   */
  const char *mime;

  /**
   * Handler to call to generate response.
   */
  PageHandler handler;

  /**
   * Extra argument to handler.
   */
  const void *handler_cls;
};


/**
 * Add header to response to set a session cookie.
 *
 * @param session session to use
 * @param response response to modify
 */
static void
add_session_cookie (struct Session *session,
                    struct MHD_Response *response)
{
  char cstr[256];
  snprintf (cstr,
            sizeof (cstr),
            "%s=%s",
            COOKIE_NAME,
            session->sid);
  if (MHD_NO ==
      MHD_add_response_header (response,
                               MHD_HTTP_HEADER_SET_COOKIE,
                               cstr))
  {
    fprintf (stderr,
             "Failed to set session cookie header!\n");
  }
}


/**
 * Handler that returns a simple static HTTP page that
 * is passed in via 'cls'.
 *
 * @param cls a 'const char *' with the HTML webpage to return
 * @param mime mime type to use
 * @param session session handle
 * @param connection connection to use
 */
static enum MHD_Result
serve_simple_form (const void *cls,
                   const char *mime,
                   struct Session *session,
                   struct MHD_Connection *connection)
{
  enum MHD_Result ret;
  const char *form = cls;
  struct MHD_Response *response;

  /* return static form */
  response = MHD_create_response_from_buffer_static (strlen (form), form);
  add_session_cookie (session, response);
  MHD_add_response_header (response,
                           MHD_HTTP_HEADER_CONTENT_ENCODING,
                           mime);
  ret = MHD_queue_response (connection,
                            MHD_HTTP_OK,
                            response);
  MHD_destroy_response (response);
  return ret;
}


/**
 * Handler that adds the 'v1' value to the given HTML code.
 *
 * @param cls a 'const char *' with the HTML webpage to return
 * @param mime mime type to use
 * @param session session handle
 * @param connection connection to use
 */
static enum MHD_Result
fill_v1_form (const void *cls,
              const char *mime,
              struct Session *session,
              struct MHD_Connection *connection)
{
  enum MHD_Result ret;
  char *reply;
  struct MHD_Response *response;
  int reply_len;
  (void) cls; /* Unused */

  reply_len = MHD_asprintf (&reply,
                            MAIN_PAGE,
                            session->value_1);
  if (0 > reply_len)
  {
    /* oops */
    return MHD_NO;
  }
  /* return static form */
  response =
    MHD_create_response_from_buffer_with_free_callback ((size_t) reply_len,
                                                        (void *) reply,
                                                        &free);
  add_session_cookie (session, response);
  MHD_add_response_header (response,
                           MHD_HTTP_HEADER_CONTENT_ENCODING,
                           mime);
  ret = MHD_queue_response (connection,
                            MHD_HTTP_OK,
                            response);
  MHD_destroy_response (response);
  return ret;
}


/**
 * Handler that adds the 'v1' and 'v2' values to the given HTML code.
 *
 * @param cls a 'const char *' with the HTML webpage to return
 * @param mime mime type to use
 * @param session session handle
 * @param connection connection to use
 */
static enum MHD_Result
fill_v1_v2_form (const void *cls,
                 const char *mime,
                 struct Session *session,
                 struct MHD_Connection *connection)
{
  enum MHD_Result ret;
  char *reply;
  struct MHD_Response *response;
  int reply_len;
  (void) cls; /* Unused */

  reply_len = MHD_asprintf (&reply,
                            SECOND_PAGE,
                            session->value_1,
                            session->value_2);
  if (0 > reply_len)
  {
    /* oops */
    return MHD_NO;
  }
  /* return static form */
  response =
    MHD_create_response_from_buffer_with_free_callback ((size_t) reply_len,
                                                        (void *) reply,
                                                        &free);
  add_session_cookie (session, response);
  MHD_add_response_header (response,
                           MHD_HTTP_HEADER_CONTENT_ENCODING,
                           mime);
  ret = MHD_queue_response (connection,
                            MHD_HTTP_OK,
                            response);
  MHD_destroy_response (response);
  return ret;
}


/**
 * Handler used to generate a 404 reply.
 *
 * @param cls a 'const char *' with the HTML webpage to return
 * @param mime mime type to use
 * @param session session handle
 * @param connection connection to use
 */
static enum MHD_Result
not_found_page (const void *cls,
                const char *mime,
                struct Session *session,
                struct MHD_Connection *connection)
{
  enum MHD_Result ret;
  struct MHD_Response *response;
  (void) cls;     /* Unused. Silent compiler warning. */
  (void) session; /* Unused. Silent compiler warning. */

  /* unsupported HTTP method */
  response = MHD_create_response_from_buffer_static (strlen (NOT_FOUND_ERROR),
                                                     NOT_FOUND_ERROR);
  ret = MHD_queue_response (connection,
                            MHD_HTTP_NOT_FOUND,
                            response);
  MHD_add_response_header (response,
                           MHD_HTTP_HEADER_CONTENT_ENCODING,
                           mime);
  MHD_destroy_response (response);
  return ret;
}


/**
 * List of all pages served by this HTTP server.
 */
static struct Page pages[] = {
  { "/", "text/html",  &fill_v1_form, NULL },
  { "/2", "text/html", &fill_v1_v2_form, NULL },
  { "/S", "text/html", &serve_simple_form, SUBMIT_PAGE },
  { "/F", "text/html", &serve_simple_form, LAST_PAGE },
  { NULL, NULL, &not_found_page, NULL }   /* 404 */
};


/**
 * Iterator over key-value pairs where the value
 * maybe made available in increments and/or may
 * not be zero-terminated.  Used for processing
 * POST data.
 *
 * @param cls user-specified closure
 * @param kind type of the value
 * @param key 0-terminated key for the value
 * @param filename name of the uploaded file, NULL if not known
 * @param content_type mime-type of the data, NULL if not known
 * @param transfer_encoding encoding of the data, NULL if not known
 * @param data pointer to size bytes of data at the
 *              specified offset
 * @param off offset of data in the overall value
 * @param size number of bytes in data available
 * @return MHD_YES to continue iterating,
 *         MHD_NO to abort the iteration
 */
static enum MHD_Result
post_iterator (void *cls,
               enum MHD_ValueKind kind,
               const char *key,
               const char *filename,
               const char *content_type,
               const char *transfer_encoding,
               const char *data, uint64_t off, size_t size)
{
  struct Request *request = cls;
  struct Session *session = request->session;
  (void) kind;               /* Unused. Silent compiler warning. */
  (void) filename;           /* Unused. Silent compiler warning. */
  (void) content_type;       /* Unused. Silent compiler warning. */
  (void) transfer_encoding;  /* Unused. Silent compiler warning. */

  if (0 == strcmp ("DONE", key))
  {
    fprintf (stdout,
             "Session `%s' submitted `%s', `%s'\n",
             session->sid,
             session->value_1,
             session->value_2);
    return MHD_YES;
  }
  if (0 == strcmp ("v1", key))
  {
    if (size + off > sizeof(session->value_1))
      size = sizeof (session->value_1) - off;
    memcpy (&session->value_1[off],
            data,
            size);
    if (size + off < sizeof (session->value_1))
      session->value_1[size + off] = '\0';
    return MHD_YES;
  }
  if (0 == strcmp ("v2", key))
  {
    if (size + off > sizeof(session->value_2))
      size = sizeof (session->value_2) - off;
    memcpy (&session->value_2[off],
            data,
            size);
    if (size + off < sizeof (session->value_2))
      session->value_2[size + off] = '\0';
    return MHD_YES;
  }
  fprintf (stderr, "Unsupported form value `%s'\n", key);
  return MHD_YES;
}


/**
 * Main MHD callback for handling requests.
 *
 *
 * @param cls argument given together with the function
 *        pointer when the handler was registered with MHD
 * @param connection handle to connection which is being processed
 * @param url the requested url
 * @param method the HTTP method used ("GET", "PUT", etc.)
 * @param version the HTTP version string (i.e. "HTTP/1.1")
 * @param upload_data the data being uploaded (excluding HEADERS,
 *        for a POST that fits into memory and that is encoded
 *        with a supported encoding, the POST data will NOT be
 *        given in upload_data and is instead available as
 *        part of MHD_get_connection_values; very large POST
 *        data *will* be made available incrementally in
 *        upload_data)
 * @param upload_data_size set initially to the size of the
 *        upload_data provided; the method must update this
 *        value to the number of bytes NOT processed;
 * @param req_cls pointer that the callback can set to some
 *        address and that will be preserved by MHD for future
 *        calls for this request; since the access handler may
 *        be called many times (i.e., for a PUT/POST operation
 *        with plenty of upload data) this allows the application
 *        to easily associate some request-specific state.
 *        If necessary, this state can be cleaned up in the
 *        global "MHD_RequestCompleted" callback (which
 *        can be set with the MHD_OPTION_NOTIFY_COMPLETED).
 *        Initially, <tt>*req_cls</tt> will be NULL.
 * @return MHS_YES if the connection was handled successfully,
 *         MHS_NO if the socket must be closed due to a serious
 *         error while handling the request
 */
static enum MHD_Result
create_response (void *cls,
                 struct MHD_Connection *connection,
                 const char *url,
                 const char *method,
                 const char *version,
                 const char *upload_data,
                 size_t *upload_data_size,
                 void **req_cls)
{
  struct MHD_Response *response;
  struct Request *request;
  struct Session *session;
  enum MHD_Result ret;
  unsigned int i;
  (void) cls;               /* Unused. Silent compiler warning. */
  (void) version;           /* Unused. Silent compiler warning. */

  request = *req_cls;
  if (NULL == request)
  {
    request = calloc (1, sizeof (struct Request));
    if (NULL == request)
    {
      fprintf (stderr, "calloc error: %s\n", strerror (errno));
      return MHD_NO;
    }
    *req_cls = request;
    if (0 == strcmp (method, MHD_HTTP_METHOD_POST))
    {
      request->pp = MHD_create_post_processor (connection, 1024,
                                               &post_iterator, request);
      if (NULL == request->pp)
      {
        fprintf (stderr, "Failed to setup post processor for `%s'\n",
                 url);
        return MHD_NO; /* internal error */
      }
    }
    return MHD_YES;
  }
  if (NULL == request->session)
  {
    request->session = get_session (connection);
    if (NULL == request->session)
    {
      fprintf (stderr, "Failed to setup session for `%s'\n",
               url);
      return MHD_NO; /* internal error */
    }
  }
  session = request->session;
  session->start = time (NULL);
  if (0 == strcmp (method, MHD_HTTP_METHOD_POST))
  {
    /* evaluate POST data */
    MHD_post_process (request->pp,
                      upload_data,
                      *upload_data_size);
    if (0 != *upload_data_size)
    {
      *upload_data_size = 0;
      return MHD_YES;
    }
    /* done with POST data, serve response */
    MHD_destroy_post_processor (request->pp);
    request->pp = NULL;
    method = MHD_HTTP_METHOD_GET;   /* fake 'GET' */
    if (NULL != request->post_url)
      url = request->post_url;
  }

  if ( (0 == strcmp (method, MHD_HTTP_METHOD_GET)) ||
       (0 == strcmp (method, MHD_HTTP_METHOD_HEAD)) )
  {
    /* find out which page to serve */
    i = 0;
    while ( (pages[i].url != NULL) &&
            (0 != strcmp (pages[i].url, url)) )
      i++;
    ret = pages[i].handler (pages[i].handler_cls,
                            pages[i].mime,
                            session, connection);
    if (ret != MHD_YES)
      fprintf (stderr, "Failed to create page for `%s'\n",
               url);
    return ret;
  }
  /* unsupported HTTP method */
  response = MHD_create_response_from_buffer_static (strlen (METHOD_ERROR),
                                                     METHOD_ERROR);
  ret = MHD_queue_response (connection,
                            MHD_HTTP_NOT_ACCEPTABLE,
                            response);
  MHD_destroy_response (response);
  return ret;
}


/**
 * Callback called upon completion of a request.
 * Decrements session reference counter.
 *
 * @param cls not used
 * @param connection connection that completed
 * @param req_cls session handle
 * @param toe status code
 */
static void
request_completed_callback (void *cls,
                            struct MHD_Connection *connection,
                            void **req_cls,
                            enum MHD_RequestTerminationCode toe)
{
  struct Request *request = *req_cls;
  (void) cls;         /* Unused. Silent compiler warning. */
  (void) connection;  /* Unused. Silent compiler warning. */
  (void) toe;         /* Unused. Silent compiler warning. */

  if (NULL == request)
    return;
  if (NULL != request->session)
    request->session->rc--;
  if (NULL != request->pp)
    MHD_destroy_post_processor (request->pp);
  free (request);
}


/**
 * Clean up handles of sessions that have been idle for
 * too long.
 */
static void
expire_sessions (void)
{
  struct Session *pos;
  struct Session *prev;
  struct Session *next;
  time_t now;

  now = time (NULL);
  prev = NULL;
  pos = sessions;
  while (NULL != pos)
  {
    next = pos->next;
    if (now - pos->start > 60 * 60)
    {
      /* expire sessions after 1h */
      if (NULL == prev)
        sessions = pos->next;
      else
        prev->next = next;
      free (pos);
    }
    else
      prev = pos;
    pos = next;
  }
}


/**
 * Call with the port number as the only argument.
 * Never terminates (other than by signals, such as CTRL-C).
 */
int
main (int argc, char *const *argv)
{
  struct MHD_Daemon *d;
  struct timeval tv;
  struct timeval *tvp;
  fd_set rs;
  fd_set ws;
  fd_set es;
  MHD_socket max;
  uint64_t mhd_timeout;
  unsigned int port;

  if (argc != 2)
  {
    printf ("%s PORT\n", argv[0]);
    return 1;
  }
  if ( (1 != sscanf (argv[1], "%u", &port)) ||
       (0 == port) || (65535 < port) )
  {
    fprintf (stderr,
             "Port must be a number between 1 and 65535.\n");
    return 1;
  }

  /* initialize PRNG */
  srand ((unsigned int) time (NULL));
  d = MHD_start_daemon (MHD_USE_ERROR_LOG,
                        (uint16_t) port,
                        NULL, NULL,
                        &create_response, NULL,
                        MHD_OPTION_CONNECTION_TIMEOUT, (unsigned int) 15,
                        MHD_OPTION_NOTIFY_COMPLETED,
                        &request_completed_callback, NULL,
                        MHD_OPTION_END);
  if (NULL == d)
    return 1;
  while (1)
  {
    expire_sessions ();
    max = 0;
    FD_ZERO (&rs);
    FD_ZERO (&ws);
    FD_ZERO (&es);
    if (MHD_YES != MHD_get_fdset (d, &rs, &ws, &es, &max))
      break; /* fatal internal error */
    if (MHD_get_timeout64 (d, &mhd_timeout) == MHD_YES)
    {
#if ! defined(_WIN32) || defined(__CYGWIN__)
      tv.tv_sec = (time_t) (mhd_timeout / 1000);
#else  /* Native W32 */
      tv.tv_sec = (long) (mhd_timeout / 1000);
#endif /* Native W32 */
      tv.tv_usec = ((long) (mhd_timeout % 1000)) * 1000;
      tvp = &tv;
    }
    else
      tvp = NULL;
    if (-1 == select ((int) max + 1, &rs, &ws, &es, tvp))
    {
      if (EINTR != errno)
        fprintf (stderr,
                 "Aborting due to error during select: %s\n",
                 strerror (errno));
      break;
    }
    MHD_run (d);
  }
  MHD_stop_daemon (d);
  return 0;
}
