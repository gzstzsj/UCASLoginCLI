### v0.7.2
- Adjusted post fields.

### v0.7.1
- Fixed that returned messages on logoff were not correctly displayed.

## v0.7
- Fixed that username and password were not correctly encoded which could cause the program blocked at HTTP requests.
- Fixed that returned messages not surrounded by quotation marks were not correctly recorded.

### v0.6.3
- Reorganized files.

### v0.6.2
- Fixed a few bugs.
- Slightly modified codes.

### v0.6.1
- No longer try to send a log-in request if you have already logged in.
- removed a bad `sleep()` call.

## v0.6
- Add a command-line option to show remaining flow.
- Fixed a serious buffer overflow.
- No longer require username and password if you have already logged in.

### v0.5.2
- Combined the two binaries into one.

### v0.5.1
- Removed pieces of obsoleted codes.
- Fixed a few exits which is not safe.

## v0.5
- Rewrote codes to send HTTP requests and receive responses. Libcurl no longer needed.
- Revised codes to manipulate the terminal.
- Corrected an erroneous fix which promotes username to upper-case in an earlier version.
- Overwrite password stored in memory before the part of memory released.

## v0.4
- userIndex information now requested from the server rather than recorded locally.
- Fixed wrong length set for post request for logging out.
- Slight improvement to the codes.

## v0.3
- Fixed that username in lower case would be recognized as invalid. (Note: this is a wrong fix.)
