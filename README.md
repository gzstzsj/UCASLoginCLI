# Basic CLI for UCAS ePortal

**UCASLoginCLI** is a basic command-line interface with limited features which can be used to log-in to/off from a UCAS network account. It can be used on servers via ssh sessions or on personal computers without graphical web browsers with javascript support. **FOR SAFETY REASONS, ADMINISTRATOR SHALL BE INFORMED BEFORE ANY USE ON SERVERS.**

There is still much work to be done regarding the password. I am also considering to provide command-line options for saving username/password as well as retrieving and displaying some account information. However, I am too lazy to get all these done in the near future. 

-------------------

## Prerequisite

The codes are written generally in accordance with the POSIX standard, so the program should work on most UNIX and UNIX-like systems. A suitable c compiler is needed.

## Installation

Please refer to the simple Makefile provided with the codes. It is easily modified in case of need.

## Usage

Command-line option `-i` for login; `-o` for logout; `-f` for showing remaining flow; `-i` and `-o` should not be used together, `-f` should only be used in combination with `-i`. The program will wait 0.1s before trying to retrieve user account information due to a slight delay of server response to the prior request. Occasioanlly this could be causing failure of retrieving user account information. 

- Only first up-to-19 characters in the username input will be recognized because I assumed that no network account in UCAS have username longer than that.
- Password input longer than INT_MAX (about $2^{31}$) characters will result in an error. Anyway, much shorter password than that could well cause a memory allocation error.
- Returned Index numbers are assumed to be no longer than 199 characters. As far as I have seen, they should be no longer than $98+2*(length of username)$ characters long.
- Returned queryStrings are assumed to be no longer than 299 characters. As far as I have seen, they are hashed strings and at a fixed length of 259.
- Messages returned from the authentication server might not display correctly if the terminal does not support wide characters.

## Contact

Please feel free to contact [gzstzsj](mailto:gzstzsj@gmail.com) if there is any problem, question, idea or suggestion. Your help in improving the codes is highly appreciated.
