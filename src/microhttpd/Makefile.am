# This Makefile.am is in the public domain

AM_CPPFLAGS = \
  -I$(top_srcdir)/src/include \
  $(CPPFLAGS_ac)

AM_CFLAGS = $(CFLAGS_ac) $(HIDDEN_VISIBILITY_CFLAGS)

AM_LDFLAGS = $(LDFLAGS_ac)

AM_TESTS_ENVIRONMENT = $(TESTS_ENVIRONMENT_ac)

lib_LTLIBRARIES = \
  libmicrohttpd.la

noinst_DATA =
MOSTLYCLEANFILES =

AM_V_LIB = $(am__v_LIB_@AM_V@)
am__v_LIB_ = $(am__v_LIB_@AM_DEFAULT_V@)
am__v_LIB_0 = @echo "  LIB     " $@;
am__v_LIB_1 = 

if W32_SHARED_LIB_EXP
AM_V_DLLTOOL = $(am__v_DLLTOOL_@AM_V@)
am__v_DLLTOOL_ = $(am__v_DLLTOOL_@AM_DEFAULT_V@)
am__v_DLLTOOL_0 = @echo "  DLLTOOL " $@;
am__v_DLLTOOL_1 = 

W32_MHD_LIB_LDFLAGS = -Wl,--output-def,$(lt_cv_objdir)/libmicrohttpd.def -XCClinker -static-libgcc
noinst_DATA += $(lt_cv_objdir)/libmicrohttpd.lib $(lt_cv_objdir)/libmicrohttpd.def
MOSTLYCLEANFILES += $(lt_cv_objdir)/libmicrohttpd.lib $(lt_cv_objdir)/libmicrohttpd.def $(lt_cv_objdir)/libmicrohttpd.exp

$(lt_cv_objdir)/libmicrohttpd.def: libmicrohttpd.la
	$(AM_V_at)test -f $@ && touch $@ || \
	  ( rm -f libmicrohttpd.la ; $(MAKE) $(AM_MAKEFLAGS) libmicrohttpd.la && touch $@ )

if USE_EXPORT_FILE
noinst_DATA += $(lt_cv_objdir)/libmicrohttpd.exp

$(lt_cv_objdir)/libmicrohttpd.exp: $(lt_cv_objdir)/libmicrohttpd.lib
	$(AM_V_at)test -f $@ && touch $@ || \
	  ( rm -f $(lt_cv_objdir)/libmicrohttpd.lib ; $(MAKE) $(AM_MAKEFLAGS) $(lt_cv_objdir)/libmicrohttpd.lib  && touch $@ )
endif

if USE_MS_LIB_TOOL
$(lt_cv_objdir)/libmicrohttpd.lib: $(lt_cv_objdir)/libmicrohttpd.def libmicrohttpd.la $(libmicrohttpd_la_OBJECTS)
	$(AM_V_LIB) cd "$(lt_cv_objdir)" && dll_name=`$(SED) -n -e "s/^dlname='\(.*\)'/\1/p" libmicrohttpd.la` && test -n "$$dll_name" && \
	$(MS_LIB_TOOL) -nologo -def:libmicrohttpd.def -name:$$dll_name -out:libmicrohttpd.lib $(libmicrohttpd_la_OBJECTS:.lo=.o) -ignore:4221
else
if USE_EXPORT_FILE
$(lt_cv_objdir)/libmicrohttpd.lib: $(lt_cv_objdir)/libmicrohttpd.def libmicrohttpd.la $(libmicrohttpd_la_OBJECTS)
	$(AM_V_DLLTOOL) cd "$(lt_cv_objdir)" && dll_name=`$(SED) -n -e "s/^dlname='\(.*\)'/\1/p" libmicrohttpd.la` && test -n "$$dll_name" && \
	$(DLLTOOL) -d libmicrohttpd.def -D $$dll_name -l libmicrohttpd.lib $(libmicrohttpd_la_OBJECTS:.lo=.o) -e ./libmicrohttpd.exp
else
$(lt_cv_objdir)/libmicrohttpd.lib: $(lt_cv_objdir)/libmicrohttpd.def libmicrohttpd.la
	$(AM_V_DLLTOOL) cd "$(lt_cv_objdir)" && dll_name=`$(SED) -n -e "s/^dlname='\(.*\)'/\1/p" libmicrohttpd.la` && test -n "$$dll_name" && \
	$(DLLTOOL) -d libmicrohttpd.def -D $$dll_name -l libmicrohttpd.lib
endif
endif
else
W32_MHD_LIB_LDFLAGS =
endif

if W32_STATIC_LIB
noinst_DATA += $(lt_cv_objdir)/libmicrohttpd-static.lib
MOSTLYCLEANFILES += $(lt_cv_objdir)/libmicrohttpd-static.lib

$(lt_cv_objdir)/libmicrohttpd-static.lib: libmicrohttpd.la $(libmicrohttpd_la_OBJECTS)
if USE_MS_LIB_TOOL
	$(AM_V_LIB) $(MS_LIB_TOOL) -nologo -out:$@ $(libmicrohttpd_la_OBJECTS:.lo=.o)
else
	$(AM_V_at)cp $(lt_cv_objdir)/libmicrohttpd.a $@
endif
endif


libmicrohttpd_la_SOURCES = \
  connection.c connection.h \
  reason_phrase.c \
  daemon.c  \
  internal.c internal.h \
  memorypool.c memorypool.h \
  mhd_mono_clock.c mhd_mono_clock.h \
  mhd_limits.h \
  sysfdsetsize.h \
  mhd_str.c mhd_str.h mhd_str_types.h\
  mhd_send.h mhd_send.c \
  mhd_assert.h \
  mhd_sockets.c mhd_sockets.h \
  mhd_itc.c mhd_itc.h mhd_itc_types.h \
  mhd_compat.c mhd_compat.h \
  mhd_panic.c mhd_panic.h \
  response.c response.h

if USE_POSIX_THREADS
libmicrohttpd_la_SOURCES += \
  mhd_threads.c mhd_threads.h \
  mhd_locks.h
endif
if USE_W32_THREADS
libmicrohttpd_la_SOURCES += \
  mhd_threads.c mhd_threads.h \
  mhd_locks.h
endif

if NEED_SYS_FD_SET_SIZE_VALUE
libmicrohttpd_la_SOURCES += \
  sysfdsetsize.c
endif

libmicrohttpd_la_CPPFLAGS = \
  $(AM_CPPFLAGS) $(MHD_LIB_CPPFLAGS) $(MHD_TLS_LIB_CPPFLAGS) \
  -DBUILDING_MHD_LIB=1
libmicrohttpd_la_CFLAGS = \
  $(AM_CFLAGS) $(MHD_LIB_CFLAGS) $(MHD_TLS_LIB_CFLAGS)
libmicrohttpd_la_LDFLAGS = \
  $(AM_LDFLAGS) $(MHD_LIB_LDFLAGS) $(MHD_TLS_LIB_LDFLAGS) \
  $(W32_MHD_LIB_LDFLAGS) \
  -export-dynamic -no-undefined \
  -version-info @LIB_VERSION_CURRENT@:@LIB_VERSION_REVISION@:@LIB_VERSION_AGE@
libmicrohttpd_la_LIBADD = \
  $(MHD_LIBDEPS) $(MHD_TLS_LIBDEPS)


AM_V_RC = $(am__v_RC_@AM_V@)
am__v_RC_ = $(am__v_RC_@AM_DEFAULT_V@)
am__v_RC_0 = @echo "  RC      " $@;
am__v_RC_1 = 

# General rule is not required, but keep it just in case
# Note: windres does not understand '-isystem' flag, so all
# possible '-isystem' flags are replaced by simple '-I' flags.
.rc.lo:
	$(AM_V_RC) RC_ALL_CPPFLAGS=`echo ' $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) ' | $(SED) -e 's/ -isystem / -I /g'`; \
          $(LIBTOOL) $(AM_V_lt) --tag=RC $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --mode=compile $(RC) $(RCFLAGS) $(DEFS) $${RC_ALL_CPPFLAGS} $< -o $@

# Note: windres does not understand '-isystem' flag, so all
# possible '-isystem' flags are replaced by simple '-I' flags.
libmicrohttpd_la-microhttpd_dll_res.lo: $(builddir)/microhttpd_dll_res.rc
	$(AM_V_RC) RC_ALL_CPPFLAGS=`echo ' $(DEFAULT_INCLUDES) $(INCLUDES) $(libmicrohttpd_la_CPPFLAGS) $(CPPFLAGS) ' | $(SED) -e 's/ -isystem / -I /g'`; \
          $(LIBTOOL) $(AM_V_lt) --tag=RC $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --mode=compile $(RC) $(RCFLAGS) $(DEFS) $${RC_ALL_CPPFLAGS} $(builddir)/microhttpd_dll_res.rc -o $@

if HAVE_W32
MHD_DLL_RES_LO = libmicrohttpd_la-microhttpd_dll_res.lo
else
MHD_DLL_RES_LO = 
endif

EXTRA_libmicrohttpd_la_DEPENDENCIES = $(MHD_DLL_RES_LO)
libmicrohttpd_la_LIBADD += $(MHD_DLL_RES_LO)


if USE_COVERAGE
  AM_CFLAGS += --coverage
endif

if !MHD_USE_SYS_TSEARCH
libmicrohttpd_la_SOURCES += \
  tsearch.c tsearch.h
endif

if HAVE_POSTPROCESSOR
libmicrohttpd_la_SOURCES += \
  postprocessor.c postprocessor.h
endif

if HAVE_ANYAUTH
libmicrohttpd_la_SOURCES += \
  gen_auth.c gen_auth.h
endif
if ENABLE_DAUTH
libmicrohttpd_la_SOURCES += \
  digestauth.c digestauth.h \
  mhd_bithelpers.h mhd_byteorder.h mhd_align.h
if ENABLE_MD5
libmicrohttpd_la_SOURCES += \
  mhd_md5_wrap.h
if ! ENABLE_MD5_EXT
libmicrohttpd_la_SOURCES += \
  md5.c md5.h
else
libmicrohttpd_la_SOURCES += \
  md5_ext.c md5_ext.h
endif
endif
if ENABLE_SHA256
libmicrohttpd_la_SOURCES += \
  mhd_sha256_wrap.h
if ! ENABLE_SHA256_EXT
libmicrohttpd_la_SOURCES += \
  sha256.c sha256.h
else
libmicrohttpd_la_SOURCES += \
  sha256_ext.c sha256_ext.h
endif
endif
if ENABLE_SHA512_256
libmicrohttpd_la_SOURCES += \
  sha512_256.c sha512_256.h
endif
endif

if ENABLE_BAUTH
libmicrohttpd_la_SOURCES += \
  basicauth.c basicauth.h
endif

if ENABLE_HTTPS
libmicrohttpd_la_SOURCES += \
  connection_https.c connection_https.h
endif

check_PROGRAMS = \
  test_str_compare \
  test_str_to_value \
  test_str_from_value \
  test_str_token \
  test_str_token_remove \
  test_str_tokens_remove \
  test_str_pct \
  test_str_bin_hex \
  test_http_reasons \
  test_sha1 \
  test_start_stop \
  test_daemon \
  test_response_entries \
  test_postprocessor_md \
  test_client_put_shutdown \
  test_client_put_close \
  test_client_put_hard_close \
  test_client_put_steps_shutdown \
  test_client_put_steps_close \
  test_client_put_steps_hard_close \
  test_client_put_chunked_shutdown \
  test_client_put_chunked_close \
  test_client_put_chunked_hard_close \
  test_client_put_chunked_steps_shutdown \
  test_client_put_chunked_steps_close \
  test_client_put_chunked_steps_hard_close \
  test_options \
  test_mhd_version \
  test_set_panic

if ENABLE_MD5
check_PROGRAMS += \
  test_md5
endif
if ENABLE_SHA256
check_PROGRAMS += \
  test_sha256
endif
if ENABLE_SHA512_256
check_PROGRAMS += \
  test_sha512_256
endif

if HAVE_POSIX_THREADS
if ENABLE_UPGRADE
if USE_THREADS
check_PROGRAMS += test_upgrade test_upgrade_large test_upgrade_vlarge
if ENABLE_HTTPS
if USE_UPGRADE_TLS_TESTS
check_PROGRAMS += test_upgrade_tls test_upgrade_large_tls test_upgrade_vlarge_tls
endif
endif
endif
endif
endif

if HAVE_POSTPROCESSOR
check_PROGRAMS += \
  test_postprocessor \
  test_postprocessor_large \
  test_postprocessor_amp
endif

# Do not test trigger of select by shutdown of listen socket
# on Cygwin as this ability is deliberately ignored on Cygwin
# to improve compatibility with core OS.
if !CYGWIN_TARGET
if HAVE_POSIX_THREADS
if HAVE_LISTEN_SHUTDOWN
check_PROGRAMS += \
  test_shutdown_select \
  test_shutdown_poll
else
check_PROGRAMS += \
  test_shutdown_select_ignore \
  test_shutdown_poll_ignore
endif
endif
endif

if HAVE_ANYAUTH
if HAVE_MESSAGES
check_PROGRAMS += \
  test_auth_parse
endif
endif
if ENABLE_DAUTH
check_PROGRAMS += \
  test_str_quote \
  test_dauth_userdigest \
  test_dauth_userhash
endif
if ENABLE_BAUTH
check_PROGRAMS += \
  test_str_base64
endif


if HEAVY_TESTS
if TESTS_STRESS_OS
check_PROGRAMS += \
  test_client_put_hard_close_stress_os \
  test_client_put_steps_hard_close_stress_os \
  test_client_put_chunked_steps_hard_close_stress_os
endif
endif


TESTS = $(check_PROGRAMS)

update-po-POTFILES.in: $(top_srcdir)/po/POTFILES.in

$(top_srcdir)/po/POTFILES.in: $(srcdir)/Makefile.am
	@echo "Creating $@"
	@echo  src/include/microhttpd.h > "$@" && \
	for src in $(am__libmicrohttpd_la_SOURCES_DIST) ; do \
	  echo "$(subdir)/$$src" >> "$@" ; \
	done

test_start_stop_SOURCES = \
  test_start_stop.c
test_start_stop_LDADD = \
  libmicrohttpd.la

test_daemon_SOURCES = \
  test_daemon.c
test_daemon_LDADD = \
  $(builddir)/libmicrohttpd.la

test_response_entries_SOURCES = \
  test_response_entries.c
test_response_entries_LDADD = \
  libmicrohttpd.la

test_upgrade_SOURCES = \
  test_upgrade.c test_helpers.h mhd_sockets.h
test_upgrade_CPPFLAGS = \
  $(AM_CPPFLAGS) $(MHD_TLS_LIB_CPPFLAGS)
test_upgrade_CFLAGS = \
  $(AM_CFLAGS) $(PTHREAD_CFLAGS) $(MHD_TLS_LIB_CFLAGS)
test_upgrade_LDFLAGS = \
  $(MHD_TLS_LIB_LDFLAGS)
test_upgrade_LDADD = \
  $(builddir)/libmicrohttpd.la \
  $(MHD_TLS_LIBDEPS) \
  $(PTHREAD_LIBS)

test_upgrade_large_SOURCES = \
  $(test_upgrade_SOURCES)
test_upgrade_large_CPPFLAGS = \
  $(test_upgrade_CPPFLAGS)
test_upgrade_large_CFLAGS = \
  $(test_upgrade_CFLAGS)
test_upgrade_large_LDFLAGS = \
  $(test_upgrade_LDFLAGS)
test_upgrade_large_LDADD = \
  $(test_upgrade_LDADD)

test_upgrade_vlarge_SOURCES = \
  $(test_upgrade_SOURCES)
test_upgrade_vlarge_CPPFLAGS = \
  $(test_upgrade_CPPFLAGS)
test_upgrade_vlarge_CFLAGS = \
  $(test_upgrade_CFLAGS)
test_upgrade_vlarge_LDFLAGS = \
  $(test_upgrade_LDFLAGS)
test_upgrade_vlarge_LDADD = \
  $(test_upgrade_LDADD)

test_upgrade_tls_SOURCES = \
  $(test_upgrade_SOURCES)
test_upgrade_tls_CPPFLAGS = \
  $(test_upgrade_CPPFLAGS)
test_upgrade_tls_CFLAGS = \
  $(test_upgrade_CFLAGS)
test_upgrade_tls_LDFLAGS = \
  $(test_upgrade_LDFLAGS)
test_upgrade_tls_LDADD = \
  $(test_upgrade_LDADD)

test_upgrade_large_tls_SOURCES = \
  $(test_upgrade_SOURCES)
test_upgrade_large_tls_CPPFLAGS = \
  $(test_upgrade_CPPFLAGS)
test_upgrade_large_tls_CFLAGS = \
  $(test_upgrade_CFLAGS)
test_upgrade_large_tls_LDFLAGS = \
  $(test_upgrade_LDFLAGS)
test_upgrade_large_tls_LDADD = \
  $(test_upgrade_LDADD)

test_upgrade_vlarge_tls_SOURCES = \
  $(test_upgrade_SOURCES)
test_upgrade_vlarge_tls_CPPFLAGS = \
  $(test_upgrade_CPPFLAGS)
test_upgrade_vlarge_tls_CFLAGS = \
  $(test_upgrade_CFLAGS)
test_upgrade_vlarge_tls_LDFLAGS = \
  $(test_upgrade_LDFLAGS)
test_upgrade_vlarge_tls_LDADD = \
  $(test_upgrade_LDADD)

test_postprocessor_SOURCES = \
  test_postprocessor.c
test_postprocessor_CPPFLAGS = \
  $(AM_CPPFLAGS) $(MHD_TLS_LIB_CPPFLAGS)
test_postprocessor_CFLAGS = \
  $(AM_CFLAGS) $(MHD_TLS_LIB_CFLAGS)
test_postprocessor_LDADD = \
  $(builddir)/libmicrohttpd.la

test_postprocessor_amp_SOURCES = \
  test_postprocessor_amp.c
test_postprocessor_amp_CPPFLAGS = \
  $(AM_CPPFLAGS) $(MHD_TLS_LIB_CPPFLAGS)
test_postprocessor_amp_CFLAGS = \
  $(AM_CFLAGS) $(MHD_TLS_LIB_CFLAGS)
test_postprocessor_amp_LDADD = \
  $(builddir)/libmicrohttpd.la

test_postprocessor_large_SOURCES = \
  test_postprocessor_large.c
test_postprocessor_large_CPPFLAGS = \
  $(AM_CPPFLAGS) $(MHD_TLS_LIB_CPPFLAGS)
test_postprocessor_large_CFLAGS = \
  $(AM_CFLAGS) $(MHD_TLS_LIB_CFLAGS)
test_postprocessor_large_LDADD = \
  $(builddir)/libmicrohttpd.la

test_postprocessor_md_SOURCES = \
  test_postprocessor_md.c postprocessor.h postprocessor.c \
  internal.h internal.c mhd_str.h mhd_str.c \
  mhd_panic.h mhd_panic.c
test_postprocessor_md_CPPFLAGS = \
  $(AM_CPPFLAGS) $(MHD_TLS_LIB_CPPFLAGS)

test_shutdown_select_SOURCES = \
  test_shutdown_select.c
test_shutdown_select_CFLAGS = $(AM_CFLAGS)
test_shutdown_select_LDADD = $(LDADD)
if USE_POSIX_THREADS
test_shutdown_select_CFLAGS += \
  $(PTHREAD_CFLAGS)
test_shutdown_select_LDADD += \
  $(PTHREAD_LIBS)
endif

test_shutdown_poll_SOURCES = \
  test_shutdown_select.c mhd_threads.h
test_shutdown_poll_CFLAGS = $(AM_CFLAGS)
test_shutdown_poll_LDADD = $(LDADD)
if USE_POSIX_THREADS
test_shutdown_poll_CFLAGS += \
  $(AM_CFLAGS) $(PTHREAD_CFLAGS)
test_shutdown_poll_LDADD += \
  $(PTHREAD_LIBS)
endif

test_shutdown_select_ignore_SOURCES = \
  test_shutdown_select.c
test_shutdown_select_ignore_CFLAGS = $(AM_CFLAGS)
test_shutdown_select_ignore_LDADD = $(LDADD)
if USE_POSIX_THREADS
test_shutdown_select_ignore_CFLAGS += \
  $(PTHREAD_CFLAGS)
test_shutdown_select_ignore_LDADD += \
  $(PTHREAD_LIBS)
endif

test_shutdown_poll_ignore_SOURCES = \
  test_shutdown_select.c mhd_threads.h
test_shutdown_poll_ignore_CFLAGS = $(AM_CFLAGS)
test_shutdown_poll_ignore_LDADD = $(LDADD)
if USE_POSIX_THREADS
test_shutdown_poll_ignore_CFLAGS += \
  $(AM_CFLAGS) $(PTHREAD_CFLAGS)
test_shutdown_poll_ignore_LDADD += \
  $(PTHREAD_LIBS)
endif

test_str_compare_SOURCES = \
  test_str.c test_helpers.h mhd_str.c mhd_str.h

test_str_to_value_SOURCES = \
  test_str.c test_helpers.h mhd_str.c mhd_str.h

test_str_from_value_SOURCES = \
  test_str.c test_helpers.h mhd_str.c mhd_str.h

test_str_token_SOURCES = \
  test_str_token.c mhd_str.c mhd_str.h

test_str_token_remove_SOURCES = \
  test_str_token_remove.c mhd_str.c mhd_str.h mhd_assert.h ../include/mhd_options.h

test_str_tokens_remove_SOURCES = \
  test_str_tokens_remove.c mhd_str.c mhd_str.h mhd_assert.h ../include/mhd_options.h

test_http_reasons_SOURCES = \
  test_http_reasons.c \
  reason_phrase.c mhd_str.c mhd_str.h

test_md5_SOURCES = \
  test_md5.c test_helpers.h mhd_md5_wrap.h ../include/mhd_options.h
if ! ENABLE_MD5_EXT
test_md5_SOURCES += \
  md5.c md5.h mhd_bithelpers.h mhd_byteorder.h mhd_align.h
test_md5_CPPFLAGS = $(AM_CPPFLAGS)
test_md5_CFLAGS = $(AM_CFLAGS)
test_md5_LDFLAGS = $(AM_LDFLAGS)
test_md5_LDADD = $(LDADD)
else
test_md5_SOURCES += \
  md5_ext.c md5_ext.h
test_md5_CPPFLAGS = $(AM_CPPFLAGS) $(MHD_TLS_LIB_CPPFLAGS)
test_md5_CFLAGS = $(AM_CFLAGS) $(MHD_TLS_LIB_CFLAGS)
test_md5_LDFLAGS = $(AM_LDFLAGS) $(MHD_TLS_LIB_LDFLAGS)
test_md5_LDADD = $(MHD_TLS_LIBDEPS) $(LDADD)
endif

test_sha256_SOURCES = \
  test_sha256.c test_helpers.h mhd_sha256_wrap.h ../include/mhd_options.h
if ! ENABLE_SHA256_EXT
test_sha256_SOURCES += \
  sha256.c sha256.h mhd_bithelpers.h mhd_byteorder.h mhd_align.h
test_sha256_CPPFLAGS = $(AM_CPPFLAGS)
test_sha256_CFLAGS = $(AM_CFLAGS)
test_sha256_LDFLAGS = $(AM_LDFLAGS)
test_sha256_LDADD = $(LDADD)
else
test_sha256_SOURCES += \
  sha256_ext.c sha256_ext.h
test_sha256_CPPFLAGS = $(AM_CPPFLAGS) $(MHD_TLS_LIB_CPPFLAGS)
test_sha256_CFLAGS = $(AM_CFLAGS) $(MHD_TLS_LIB_CFLAGS)
test_sha256_LDFLAGS = $(AM_LDFLAGS) $(MHD_TLS_LIB_LDFLAGS)
test_sha256_LDADD = $(MHD_TLS_LIBDEPS) $(LDADD)
endif

test_sha512_256_SOURCES = \
  test_sha512_256.c test_helpers.h \
  sha512_256.c sha512_256.h mhd_bithelpers.h mhd_byteorder.h mhd_align.h

test_sha1_SOURCES = \
  test_sha1.c test_helpers.h \
  sha1.c sha1.h mhd_bithelpers.h mhd_byteorder.h mhd_align.h

test_auth_parse_SOURCES = \
  test_auth_parse.c gen_auth.c gen_auth.h  mhd_str.h mhd_str.c mhd_assert.h
test_auth_parse_CPPFLAGS = \
  $(AM_CPPFLAGS) $(MHD_TLS_LIB_CPPFLAGS)

test_str_quote_SOURCES = \
  test_str_quote.c mhd_str.h mhd_str.c mhd_assert.h

test_str_base64_SOURCES = \
  test_str_base64.c mhd_str.h mhd_str.c mhd_assert.h

test_str_pct_SOURCES = \
  test_str_pct.c mhd_str.h mhd_str.c mhd_assert.h

test_str_bin_hex_SOURCES = \
  test_str_bin_hex.c mhd_str.h mhd_str.c mhd_assert.h

test_options_SOURCES = \
  test_options.c
test_options_LDADD = \
  $(builddir)/libmicrohttpd.la

test_client_put_shutdown_SOURCES = \
  test_client_put_stop.c
test_client_put_shutdown_LDADD = \
  libmicrohttpd.la

test_client_put_close_SOURCES = \
  test_client_put_stop.c
test_client_put_close_LDADD = \
  libmicrohttpd.la

test_client_put_hard_close_SOURCES = \
  test_client_put_stop.c
test_client_put_hard_close_LDADD = \
  libmicrohttpd.la

test_client_put_hard_close_stress_os_SOURCES = \
  $(test_client_put_hard_close_SOURCES)
test_client_put_hard_close_stress_os_LDADD = \
  $(test_client_put_hard_close_LDADD)

test_client_put_steps_shutdown_SOURCES = \
  test_client_put_stop.c
test_client_put_steps_shutdown_LDADD = \
  libmicrohttpd.la

test_client_put_steps_close_SOURCES = \
  test_client_put_stop.c
test_client_put_steps_close_LDADD = \
  libmicrohttpd.la

test_client_put_steps_hard_close_SOURCES = \
  test_client_put_stop.c
test_client_put_steps_hard_close_LDADD = \
  libmicrohttpd.la

test_client_put_steps_hard_close_stress_os_SOURCES = \
  $(test_client_put_steps_hard_close_SOURCES)
test_client_put_steps_hard_close_stress_os_LDADD = \
  $(test_client_put_steps_hard_close_LDADD)

test_client_put_chunked_shutdown_SOURCES = \
  test_client_put_stop.c
test_client_put_chunked_shutdown_LDADD = \
  libmicrohttpd.la

test_client_put_chunked_close_SOURCES = \
  test_client_put_stop.c
test_client_put_chunked_close_LDADD = \
  libmicrohttpd.la

test_client_put_chunked_hard_close_SOURCES = \
  test_client_put_stop.c
test_client_put_chunked_hard_close_LDADD = \
  libmicrohttpd.la

test_client_put_chunked_steps_shutdown_SOURCES = \
  test_client_put_stop.c
test_client_put_chunked_steps_shutdown_LDADD = \
  libmicrohttpd.la

test_client_put_chunked_steps_close_SOURCES = \
  test_client_put_stop.c
test_client_put_chunked_steps_close_LDADD = \
  libmicrohttpd.la

test_client_put_chunked_steps_hard_close_SOURCES = \
  test_client_put_stop.c
test_client_put_chunked_steps_hard_close_LDADD = \
  libmicrohttpd.la

test_client_put_chunked_steps_hard_close_stress_os_SOURCES = \
  $(test_client_put_chunked_steps_hard_close_SOURCES)
test_client_put_chunked_steps_hard_close_stress_os_LDADD = \
  $(test_client_put_chunked_steps_hard_close_LDADD)

test_set_panic_SOURCES = \
  test_set_panic.c
test_set_panic_LDADD = \
  libmicrohttpd.la

test_dauth_userdigest_SOURCES = \
  test_dauth_userdigest.c
test_dauth_userdigest_LDADD = \
  libmicrohttpd.la

test_dauth_userhash_SOURCES = \
  test_dauth_userhash.c
test_dauth_userhash_LDADD = \
  libmicrohttpd.la

test_mhd_version_SOURCES = \
  test_mhd_version.c
test_mhd_version_LDADD = \
  libmicrohttpd.la

.PHONY: update-po-POTFILES.in
