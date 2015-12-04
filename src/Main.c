#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <termios.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include "../include/common.h"
#include "../include/connect.h"
#define INCRE 16
#define UNAMELEN 20
#define POSTFIELDOFFSET 10

extern char result[];
extern char messages[];
extern char userIndex[];
extern char receiveline[];
extern char queryString[];
extern struct flow flow_current;
extern int readMessages(const char*);
extern int readFlow(const char*);
extern int getIndex(const char*);
extern int readQuery(const char*);

static const char* USAGE = "Usage: ucasnet [-iof]\n\tEither \'i\' or \'o\' should be specified.\n\tUse a combination of \'i\' and \'f\' \
to request remaining flow after successful login attempt.\n";

static const char* success = "success";
static char username[UNAMELEN];
static const char* part1 = "\r\n\r\nuserId=";
static const char* part2 = "&password=";
static const char* part3 = "&service=&queryString=";
static const unsigned int len1 = 11;
static const unsigned int len2 = 10;
static const unsigned int len3 = 22;
static const struct timespec wait_time = { 0, 100000000 };
static char postfield[471];
static int gfflag;

static int get_success(void)
{
    unsigned int total_len, post_len;
    int ret;

    /* Prepare Post Field for Success */
    total_len = LENGTH_HEADER_SUCCESS_1 + LENGTH_HEADER_SUCCESS_2 + strlen(userIndex);
    (void)snprintf(postfield, total_len+1, "%s%s%s", HTTP_HEADER_SUCCESS_1,\
            userIndex, HTTP_HEADER_SUCCESS_2);

    /* Send HTTP Request and Process the Response */
    nanosleep(&wait_time, NULL);
    return http_req(postfield, total_len, receiveline, 0);
}

static int login(void)
{
    char *loginpost;
    char* pswd;
    char* temp;
    struct termios ori, new;
    sigset_t tsig, osig;
    int pwsize = INCRE;
    int lenpword;
    int cchar;
    int count;
    int need_success = 1;
    unsigned int lenuname;
    unsigned int total_len, total_len_temp, post_len;
    int sigset = 0;
    char* rdptr;

    /* Request queryString From Server */
    if (http_req(HTTP_HEADER_REQID, LENGTH_HEADER_REQID, receiveline, MAXLINE) != 0) return -1;
    if (readQuery((const char*)receiveline) != 0) 
    {
        need_success = 0;
        snprintf(queryString, 5, "null");
    }
    
    if (need_success)
	{
	    /* Read Username */
	    rdptr = username;
	    lenuname = 0;
	    printf("Username:");
	    while(lenuname < UNAMELEN && (cchar = getchar()) != '\n'){
	        if (cchar == -1) {
	            printf("Failed to read username from stdin!\n");
	            return -2;
	        }
	        *rdptr = cchar;
	        ++rdptr;
	        ++lenuname;
	    }
	    *rdptr = '\0';
	
	    /* Prepare Memory to Store Password */
	    pswd = (char*)malloc(sizeof(char)*pwsize);
	    if (pswd == NULL) {
	        printf("Failed to malloc memory space to store password string!\n");
	        return -1;
	    }
	
	    /* Prepare Terminal for Password Input */
	    if (tcgetattr(STDIN_FILENO, &ori) != 0) {
	        printf("Failed to setup terminal for password input!\n");
	        free(pswd);
	        return -3;
	    }
	    new = ori;
	    new.c_lflag &= ~ECHO;
	    new.c_lflag |= ECHONL;
	    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &new) != 0) {
	        printf("Failed to setup terminal for password input!\n");
	        free(pswd);
	        return -3;
	    }
	    if (sigemptyset(&tsig) || sigaddset(&tsig, SIGINT) || sigaddset(&tsig, SIGTSTP))
	        printf("Warning: failed to set signal mask correctly\n");
	    else {
	        sigset = 1;
	        if (sigprocmask(SIG_BLOCK, &tsig, &osig)){
	            printf("Warning: failed to set signal mask correctly\n");
	            sigset = 0;
	        }
	    }
	
	    /* Read Password */
	    rdptr = pswd;
	    lenpword = 0;
	    count = 0;
	    printf("Password:");
	    while ((cchar = getchar()) != '\n'){
	        if (cchar == -1){
	            printf("Failed to read password from stdin!\n");
	            /* Restore Terminal */
	            memset(pswd, 0, pwsize);
	            free(pswd);
	            if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &ori) != 0) {
	                printf("Warning: Failed to restore terminal!\n");
	            }
	            if (sigset)
	                sigprocmask(SIG_SETMASK, &osig, NULL);
	            return -2;
	        }
	        *rdptr = cchar;
	        ++lenpword;
	        ++count;
	        if (count == 16) {
	            pwsize += INCRE;
	            if (pwsize < 0) {
	                printf("Password too long!\n");
	                /* Restore Terminal */
	                memset(pswd, 0, pwsize-INCRE);
	                free(pswd);
	                if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &ori) != 0) {
	                    printf("Warning: Failed to restore terminal!\n");
	                }
	                if (sigset)
	                    sigprocmask(SIG_SETMASK, &osig, NULL);
	                return -4;
	            }
	            temp = (char*)malloc(sizeof(char)*pwsize);
	            if (temp == NULL) {
	                printf("Failed to malloc memory space to store password string!\n");
	                /* Restore Terminal */
	                memset(pswd, 0, pwsize-INCRE);
	                free(pswd);
	                if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &ori) != 0) {
	                    printf("Warning: Failed to restore terminal!\n");
	                }
	                if (sigset)
	                sigprocmask(SIG_SETMASK, &osig, NULL);
	                return -1;
	            }
	            (void)memcpy(temp, pswd, pwsize-INCRE);
	            memset(pswd, 0, pwsize-INCRE);
	            free(pswd);
	            pswd = temp;
	            rdptr = pswd + pwsize - 17;
	            count = 0;
	        }
	        ++rdptr;
	    }
	    *rdptr = '\0';
	
	    /* Restore Terminal */
	    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &ori) != 0) {
	        printf("Warning: Failed to restore terminal!\n");
	    }
	    if (sigset)
	        sigprocmask(SIG_SETMASK, &osig, NULL);

	    /* Prepare Login Post Field */
	    total_len = len1 + len2 + len3 + lenuname + (unsigned)lenpword - 4 + strlen(queryString);
	    total_len_temp = total_len;
	    post_len = total_len;
	    while (total_len_temp >= 1){
	        total_len_temp /= 10;
	        ++total_len;
	    }
	    total_len += (LENGTH_HEADER_LOGIN+4+strlen(queryString));
	    loginpost = (char*)malloc(sizeof(char)*(total_len+1));
	    if (loginpost == NULL) {
	        printf("Failed to malloc memory space to store login post field!\n");
	        if (need_success)
	        {
	            memset(pswd, 0, pwsize);
	            free(pswd);
	        }
	        return -1;
	    }
	    (void)snprintf(loginpost, total_len+1, "%s%d%s%s%s%s%s%s", HTTP_HEADER_LOGIN,\
	            post_len, part1, username, part2, pswd, part3, queryString);
	    if (need_success)
	    {
	        memset(pswd, 0, pwsize);
	        free(pswd);
	    }
	
	    result[0] = '\0';
	    messages[0] = '\0';
	    userIndex[0] = '\0';
	
	    /* Send HTTP Request and Process the Response */
	    if (http_req(loginpost, total_len, receiveline, MAXLINE) == 0)
	    {
	        if (readMessages((const char*)receiveline) == 0)
	        {
	            if (strcmp(result, success) != 0) 
	            {
	                gfflag = 0;
	                printf("%s\n", messages);
	            }
	            else 
	            {
	                if (!need_success) printf("%s\n", messages);
	                else 
	                {
	                    printf("Connected!\n");
	                    if (get_success() != 0)
	                        gfflag = 0;
	                }
	            }
	        }
	        memset(loginpost, 0, total_len);
	        free(loginpost);
	        return 0;
	    }
	    memset(loginpost, 0, total_len);
	    free(loginpost);
	    return -1;
    }
    else {
        /* Request userIndex From Server */
        if (getIndex((const char*)receiveline) != 0) {
            printf("Cannot get userIndex from server!\n");
            return -2;
        }
        return 1;
    }
}

static int logout(void)
{
    unsigned int total_len, post_len;
    unsigned int post_len_temp;
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
    total_len = LENGTH_HEADER_LOGOUT + 4 + post_len;
    while (post_len_temp >= 1)
    {
        post_len_temp /= 10;
        ++total_len;
    }
    (void)snprintf(postfield, total_len+1, "%s%d\r\n\r\nuserIndex=%s", HTTP_HEADER_LOGOUT,\
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

static int getflow(void)
{
    unsigned int total_len, post_len;
    unsigned int post_len_temp;
    int ret;
    messages[0] = '\0';

    /* Prepare Post Field for GetOnlineUserInfo */
    post_len = strlen(userIndex) + 10;
    post_len_temp = post_len;
    total_len = LENGTH_HEADER_INFO + 4 + post_len;
    while (post_len_temp >= 1)
    {
        post_len_temp /= 10;
        ++total_len;
    }
    (void)snprintf(postfield, total_len+1, "%s%d\r\n\r\nuserIndex=%s", HTTP_HEADER_INFO,\
            post_len, userIndex);

    /* Send HTTP Request and Process the Response */
    if (http_req(postfield, total_len, receiveline, MAXLINE) == 0)
    {
        if ( (ret = readFlow((const char*)receiveline)) == 0)
        {
            printf("%.2f", flow_current.flow_value);
            if (flow_current.unit)
                printf("MB Remaining.\n");
            else
                printf("GB Remaining.\n");
        }
        return ret;
    }
    return -3;
}

int main(int argc, char** argv)
{
    int ret, task;
    char cc;
    task = 2;
    while ( (cc = getopt(argc, argv, "+iof")) != -1)
    {
        switch (cc)
        {
            case 'f':
                gfflag = 1;
                break;
            case 'i':
                if (task != 2) task = -1;
                else task = 1;
                break;
            case 'o':
                if (task != 2) task = -1;
                else task = 0;
            default:
                ;
        }
    }
    switch (task)
    {
        case 1:
            ret = login();
            if (ret >= 0 && gfflag) 
            {
                return getflow();
            }
            else return ret;
        case 0:
            return logout();
        default:
            ;
    }
    printf(USAGE);
    return -10;
}
