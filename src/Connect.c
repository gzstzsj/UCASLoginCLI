#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "../include/connect.h"

const char* AUTH_SERVER = "210.77.16.21";
const uint16_t AUTH_PORT = 80;
char receiveline[MAXLINE+1];

/* Connect to the Auth Server */
static int connect_err(int sockfd, const struct sockaddr *servaddr, socklen_t addrlen)
{
    if (connect(sockfd, servaddr, addrlen) == 0) return 0;
    else 
        switch (errno){
            case ETIMEDOUT:
                printf("Connection timed out!\n");
                return -1;
            case ECONNREFUSED:
                printf("Connection refused!\n");
                return -1;
            case EHOSTUNREACH:
                printf("Server unreachable!\n");
                return -1;
            case ENETUNREACH:
                printf("Server unreachable!\n");
                return -1;
            default:
                return -1;
        }
}

static int addr_init(struct sockaddr_in *servaddr, const char *addr_host, const uint16_t addr_port)
{
    (void)memset(servaddr, 0, sizeof(struct sockaddr_in));
    servaddr->sin_family = AF_INET;
    servaddr->sin_port = htons(addr_port);
    if (inet_pton(AF_INET, addr_host, &(servaddr->sin_addr)) <= 0) {
        printf("Failed to initialize server address!\n");
        return -1;
    }
    return 0;
}

int http_req(const char* http_header, size_t size_header, char* receiveline, size_t max_receive)
{
    struct sockaddr_in servaddr;
    int socketd;
    ssize_t processed;
    size_t nleft = size_header;
    const char* wrptr = http_header;
    char* rdptr = receiveline;
    if (addr_init(&servaddr, AUTH_SERVER, AUTH_PORT) < 0)
        return -1;
    if ((socketd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
        printf("Failed to initialize socket descriptor!\n");
        return -1;
    }
    if (connect_err(socketd, (struct sockaddr*)&servaddr, sizeof(struct sockaddr_in)) < 0)
        return -1;
    /* Write to the Socket */
    while (nleft > 0) 
    {
        if ((processed = write(socketd, wrptr, nleft)) < 0)
        {
            if (errno == EINTR) processed = 0;
            else {
                printf("Write error!\n");
                close(socketd);
                return -2;
            }
        }
        nleft -= processed;
        wrptr += processed;
    }
    (void)shutdown(socketd, SHUT_WR);
    nleft = max_receive;
    /* Read from the Socket */
    while ( nleft > 0 && (processed = read(socketd, rdptr, nleft)) > 0 )
    {
        rdptr += processed;
        nleft -= processed;
    }
    *rdptr = '\0';
    return 0;
}
