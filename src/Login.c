#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <termios.h>
#include <unistd.h>
#include <curl/curl.h>
#include "../include/common.h"
#define INCRE 16
#define UNAMELEN 20

extern char result[];
extern char messages[];
extern char userIndex[];
extern int readMessages(const char*);

static const char* success = "success";
static char username[UNAMELEN];
static const char* part1 = "userId=";
static const char* part2 = "&password=";
static const char* part3 = "&service=&queryString=null";
static const unsigned int len1 = 7;
static const unsigned int len2 = 10;
static const unsigned int len3 = 26;

static size_t recId(void *ptr, size_t size, size_t nmemb, void *stream)
{
    FILE *IndexFile;
    if (readMessages((const char*)ptr) == 0)
    {
        if (strcmp(result, success) != 0) printf("\n%s\n", messages);
        else {
            printf("\nConnected!\n");
            IndexFile = fopen(UCASIndex, "w");
            if (IndexFile == NULL) {
                printf("Failed to open file, userIndex not recorded!\n");
                return size*nmemb;
            }
            fprintf(IndexFile, "result=%s\nmessage=%s\nIndexLength=%lu\nuserIndex=%s\n", result, messages, (unsigned long)(strlen(userIndex))+1, userIndex);
            (void)fclose(IndexFile);
        }
        return size*nmemb;
    }
    else return -1;
}

int main(void)
{
    CURL *curl;
    CURLcode res;

    char* loginpost;
    char* pswd;
    char* temp;
    struct termios ori, new;
    int pwsize = INCRE;
    int lenpword;
    int cchar;
    int count;
    unsigned int lenuname;
    unsigned int tlength;
    char* rdptr;

    /* Read Username */
    rdptr = username;
    lenuname = 0;
    printf("Username:");
    while(lenuname < UNAMELEN && (cchar = getchar()) != '\n'){
        if (cchar == -1) {
            printf("Failed to read username from stdin!\n");
            return -2;
        }
        if (cchar > 96) cchar-=32;
        *rdptr = cchar;
        ++rdptr;
        ++lenuname;
    }
    *rdptr = '\0';

    /* Prepare Terminal for Password Input */
    if (tcgetattr(STDIN_FILENO, &ori) != 0) {
        printf("Failed to setup terminal for password input!\n");
        return -3;
    }
    new = ori;
    new.c_lflag &= ~ECHO;
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &new) != 0) {
        printf("Failed to setup terminal for password input!\n");
        return -3;
    }

    /* Prepare Memory to Store Password */
    pswd = (char*)malloc(sizeof(char)*pwsize);
    if (pswd == NULL) {
        printf("Failed to malloc memory space to store password string!\n");
        return -1;
    }

    /* Read Password */
    rdptr = pswd;
    lenpword = 0;
    count = 0;
    printf("Password:");
    while ((cchar = getchar()) != '\n'){
        if (cchar == -1){
            printf("Failed to read password from stdin!\n");
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
                if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &ori) != 0) {
                    printf("Warning: Failed to restore terminal!\n");
                }
                free(pswd);
                return -4;
            }
            temp = (char*)realloc(pswd, sizeof(char)*pwsize);
            if (temp == NULL) {
                printf("Failed to malloc memory space to store password string!\n");
                if (pswd != NULL) free(pswd);
                return -1;
            }
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

    /* Prepare Login Post Field */
    tlength = len1 + len2 + len3 + lenuname + (unsigned)lenpword;
    loginpost = (char*)malloc(sizeof(char)*(tlength+1));
    if (loginpost == NULL) {
        printf("Failed to malloc memory space to store login post field!\n");
        return -1;
    }
    rdptr = loginpost;
    (void)strcpy(rdptr, part1);
    rdptr += len1;
    (void)strcpy(rdptr, username);
    rdptr += lenuname;
    (void)strcpy(rdptr, part2);
    rdptr += len2;
    (void)strcpy(rdptr, pswd);
    rdptr += lenpword;
    (void)strcpy(rdptr, part3);
    rdptr += len3;
    *rdptr = '\0';
    free(pswd);

    result[0] = '\0';
    messages[0] = '\0';
    userIndex[0] = '\0';

    /* Send Curl HTTP Request and Process the Response */
    curl = curl_easy_init();
    if(curl) {
        struct curl_slist *chunk = NULL;
        chunk = curl_slist_append(chunk, "ContentType: application/x-www-form-urlencoded");
        chunk = curl_slist_append(chunk, "User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/45.0.2454.85 Safari/537.36");
        res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
        curl_easy_setopt(curl, CURLOPT_URL, "http://210.77.16.21/eportal/InterFace.do?method=login&time=null");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, loginpost);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, recId);
        res = curl_easy_perform(curl);
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        curl_slist_free_all(chunk);
    }
    else {
        printf("Failed to initialize curl object!\n");
        free(loginpost);
        return -5;
    }

    free(loginpost);
    return 0;
}
