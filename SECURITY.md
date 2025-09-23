# Security policy for GNU libmicrohttpd

 * [Supported Versions](#Supported-Versions)
 * [Reporting a Vulnerability](#Reporting-a-Vulnerability)
 * [Security Announcements](#Security-Announcements)
 * [Acknowledgements](#Acknowledgements)

## Supported Versions

We support both the most recent stable release series (1.x) and
the current development release series (2.x).

## Signed Releases

All commits and releases (the files on ftp.gnu.org) are signed by a
maintainer.  Each maintainer uses their personal GPG key known to and
verified by the GNU project.

## Reporting a Vulnerability

If you think you've identified a security issue in GNU libmicrohttpd, please
**do not** report the issue publicly via a mailing list, IRC, a public issue on
the GitLab issue tracker, a merge request, or any other public venue.

Instead, report a [*confidential* ("private") issue in the Mantis
issue tracker](https://bugs.gnunet.org/set_project.php?project_id=10)]
with the “private” box checked. Please include as many details as
possible, including ideally a minimal reproducible example of the
issue, and an idea of how exploitable/severe you think it is.

Private issues are only visible to the reporter and the core developer
team.

The next steps are then:
 * The report is triaged.
 * Code is audited to find any potential similar problems.
 * The fix is prepared for the development branch, and for the most recent
   stable branch.
 * The fix is submitted to the public repository and a new release
   containing the fix is issued.
 * On the day the issue and fix are made public, an announcement is made on the
   [public channels listed below](#Security-Announcements).

As per the [GNU security processes](https://www.gnu.org/software/security/)
you may escalate the report with the GNU project if -- for any reason -- the
GNU libmicrohttpd maintainers are unable to respond in a timely fashion.


## Security Announcements

Security announcements are made publicly via the
[GNU libmicrohttpd mailinglist](https://lists.gnu.org/mailman/listinfo/libmicrohttpd).

## Acknowledgements

This text was partially based on the
[Gnome Glib security policy](https://gitlab.gnome.org/GNOME/glib/-/blob/main/SECURITY.md).
