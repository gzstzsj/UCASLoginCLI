# Basic CLI for UCAS ePortal

**UCASLoginCLI** is a basic command-line interface with limited features which can be used to log-in to/off from a UCAS network account. It can be used on servers via ssh sessions or on personal computers without graphical web browsers with javascript support. **FOR SAFETY REASONS, ADMINISTRATOR SHALL BE INFORMED BEFORE ANY USE ON SERVERS.**

I am planning to provide command-line options for saving username/password as well as retrieving and displaying some account information. However, I am too lazy to get all these done in the near future. 

-------------------

## Prerequisite

[Libcurl](http://curl.haxx.se/) is used for sending HTTP requests. It can be installed either with [source codes](https://github.com/bagder/curl) or with binary packages which are provided by most Linux distributions. Other parts of the codes are written generally within the POSIX standard, so the program should work on most UNIX and UNIX-like systems. 

## Installation

Please refer to the simple Makefile provided with the codes. It is easily modified in case of need.

## Usage

Currently the binaries do not read command-line options, work on them is on the plan as mentioned before.

As their name indicates, the `online` binary gets you online and the `offline` binary gets you offline.

- Only first up-to-19 characters in the username input will be recognized because I assumed that no network account in UCAS have username longer than that.
- Password input longer than INT_MAX (about $2^{31}$) characters will result in an error. Anyway, much shorter password than that could well cause a memory allocation error.
- Returned Index numbers are assumed to be no longer than 199 characters. As far as I have seen, they are about 130 characters long.
- Some error messages returned from the authentication server might not display correctly on some systems due to different encoding.

## Contact

Please feel free to contact [gzstzsj](mailto:gzstzsj@gmail.com) if there is any problems, questions, ideas or suggestions. Your help in improving the codes is highly appreciated.

## Notice

**UCASLoginCLI** contains pieces of source codes provided in examples of [GNU C Library](http://www.gnu.org/software/libc/manual/html_mono/libc.html) and/or [libcurl](http://curl.haxx.se/libcurl/c/example.html).
