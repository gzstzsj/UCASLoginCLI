#include <stdio.h>
#include <string.h>
#include "../include/common.h"
#include "../include/connect.h"
#define POSTFIELDOFFSET 10

extern char result[];
extern char messages[];
extern char userIndex[];
extern char receiveline[];
extern int readMessages(const char*);
extern int getIndex(const char*);

static char postfield[471];

int main(void)
{
    unsigned int total_len, post_len, post_len_len;
    unsigned int post_len_temp;
    char* rdptr = postfield;
    messages[0] = '\0';

    /* Request userIndex From Server */
    if (http_req(HTTP_HEADER_REQID, LENGTH_HEADER_REQID, receiveline, MAXLINE) != 0) return -1;
    if (getIndex((const char*)receiveline) != 0) {
        printf("Cannot get userIndex from server!\n");
        return -2;
    }
    
    /* Prepare Post Field for Logout */
    post_len = strlen(userIndex) + 10;
    post_len_temp = post_len;
    post_len_len = 0;
    while (post_len_temp >= 1)
    {
        post_len_temp /= 10;
        ++post_len_len;
    }
    total_len = LENGTH_HEADER_LOGOUT + post_len_len + 4 + post_len;
    (void)snprintf(postfield, total_len, "%s%d\r\n\r\nuserIndex=%s", HTTP_HEADER_LOGOUT,\
            post_len, userIndex);

    /* Send HTTP Request and Process the Response */
    if (http_req(postfield, total_len, receiveline, MAXLINE) == 0)
    {
        if (readMessages((const char*)receiveline) == 0)
            printf("%s\n", messages);
        return 0;
    }
    return -3;
}
