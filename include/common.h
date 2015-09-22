#ifndef __UCAS_NET_COMMON_H__
#define __UCAS_NET_COMMON_H__
const char* HTTP_HEADER_LOGIN = "POST /eportal/InterFace.do?method=login&time=null HTTP/1.1\r\nHost: 210.77.16.21\r\nUser-Agent: \
Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/45.0.2454.85 Safari/537.36\r\n\
Content-Type: application/x-www-form-urlencoded\r\nContent-Length: ";
const char* HTTP_HEADER_LOGOUT = "POST /eportal/InterFace.do?method=logout HTTP/1.1\r\nHost: 210.77.16.21\r\nUser-Agent: \
Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/45.0.2454.85 Safari/537.36\r\n\
Content-Type: application/x-www-form-urlencoded\r\nContent-Length: ";
const char* HTTP_HEADER_REQID = "GET /eportal/gologout.jsp HTTP/1.1\r\nHost: 210.77.16.21\r\nUser-Agent: \
Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/45.0.2454.85 Safari/537.36\r\n\r\n";
const size_t LENGTH_HEADER_LOGIN = 263;
const size_t LENGTH_HEADER_LOGOUT = 254;
const size_t LENGTH_HEADER_REQID = 176;
int http_req(const char*, size_t, char*, size_t);
#endif
