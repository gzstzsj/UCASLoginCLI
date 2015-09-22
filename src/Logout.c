#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include "../include/common.h"
#define POSTFIELDOFFSET 10

extern char result[];
extern char messages[];
extern char userIndex[];
extern int readMessages(const char*);
extern int getIndex(FILE*);

char postfield[140];

/* Display the Message Returned from Server */
static size_t outPut(void *ptr, size_t size, size_t nmemb, void *stream)
{
    if (readMessages((const char*)ptr) == 0)
    {
        printf("%s\n", messages);
        return size*nmemb;
    }
    else return -1;
}

int main(void)
{
    CURL *curl;
    CURLcode res;
    int i;
    int lenIndex;

    messages[0] = '\0';

    /* Try to Read userIndex From File */
    FILE* indexFile = fopen(UCASIndex, "r");
    if ((lenIndex = getIndex(indexFile)) < 0) {
        printf("no valid userIndex record!\n");
        return -2;
    }
    fclose(indexFile);
    
    /* Prepare Post Field for Logout */
    curl = curl_easy_init();
    if (snprintf(postfield, POSTFIELDOFFSET+1, "userIndex=") != POSTFIELDOFFSET) {
        printf("failed to initialize post field!\n");
        return -1;
    }

    if ((i = snprintf(postfield+POSTFIELDOFFSET, lenIndex, "%s", userIndex)) != lenIndex-1){
        printf("failed to initialize post field!%d\n",i);
        return -1;
    }

    /* Send Curl HTTP Request and Process the Response */
    if(curl) {
        struct curl_slist *chunk = NULL;
        chunk = curl_slist_append(chunk, "ContentType: application/x-www-form-urlencoded");
        chunk = curl_slist_append(chunk, "User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/45.0.2454.85 Safari/537.36");
        res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
        curl_easy_setopt(curl, CURLOPT_URL, "http://210.77.16.21/eportal/InterFace.do?method=logout");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postfield);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, outPut);
        res = curl_easy_perform(curl);
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        curl_slist_free_all(chunk);
    }
    else {
        printf("Failed to initialize curl object!\n");
        return -3;
    }
    return 0;
}
