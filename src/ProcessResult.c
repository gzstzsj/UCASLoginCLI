#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/connect.h"
#define GB 1073741824
#define MB 1048576
#define SEPARATOR ','
#define QUOTE '"'

static const char* RES = "result\"";
static const char* MES = "message\"";
static const char* UID = "userIndex\"";

char result[15];
char messages[50];
char userIndex[200];
char queryString[300];
struct flow flow_current;

int readMessages(const char* input)
{
    const char* tmpptr = input;
    const char* tmpcmp;
    char* tmpwrt;
    int state = 0;
    int tmpcnt;
    while(*tmpptr != '\0')
    {
        switch (state) {
            case -1:
                /* Invalid Input */
                if (*tmpptr == SEPARATOR) state = 0;
                break;
            case 0: 
                if (*tmpptr == QUOTE) state = 1;
                break;
            case 1:
                switch(*tmpptr){
                    case 'r':
                        state = 10;
                        break;
                    case 'm':
                        state = 20;
                        break;
                    case 'u':
                        state = 30;
                        break;
                    default:
                        state = -1;
                }
                break;
            case 10:
                /* Entering result */
                tmpcmp = RES+1;
                while (*tmpptr != '\0' && *tmpcmp != '\0') {
                    if (*tmpptr != *tmpcmp) {
                        state = -1;
                        break;
                    }
                    ++tmpptr;
                    ++tmpcmp;
                }
                if (state == 10) state = 15;
                break;
            case 15:
                if (*tmpptr == QUOTE) state = 16;
                break;
            case 16:
                /* Reading result */
                tmpcnt = 0;
                tmpwrt = result;
                while (*tmpptr != QUOTE) {
                    if (*tmpptr == '\0') return -1;
                    if (tmpcnt > 14) return -2;
                    *tmpwrt = *tmpptr;
                    ++tmpptr;
                    ++tmpwrt;
                    ++tmpcnt;
                }
                *tmpwrt = '\0';
                state = 0;
                break;
            case 20:
                /* Entering message */
                tmpcmp = MES+1;
                while (*tmpptr != '\0' && *tmpcmp != '\0') {
                    if (*tmpptr != *tmpcmp) {
                        state = -1;
                        break;
                    }
                    ++tmpptr;
                    ++tmpcmp;
                }
                if (state == 20) state = 25;
                break;
            case 25:
                if (*tmpptr == QUOTE) state = 26;
                break;
            case 26:
                /* Reading message */
                tmpwrt = messages;
                tmpcnt = 0;
                while (*tmpptr != QUOTE) {
                    if (*tmpptr == '\0') return -1;
                    if (tmpcnt > 49) return -2;
                    *tmpwrt = *tmpptr;
                    ++tmpptr;
                    ++tmpwrt;
                    ++tmpcnt;
                }
                *tmpwrt = '\0';
                state = 0;
                break;
            case 30:
                /* Entering userIndex */
                tmpcmp = UID+1;
                while (*tmpptr != '\0' && *tmpcmp != '\0') {
                    if (*tmpptr != *tmpcmp) {
                        state = -1;
                        break;
                    }
                    ++tmpptr;
                    ++tmpcmp;
                }
                if (state == 30) state = 35;
                break;
            case 35:
                if (*tmpptr == QUOTE) state = 36;
                break;
            case 36:
                /* Reading userIndex */
                tmpwrt = userIndex;
                tmpcnt = 0;
                while (*tmpptr != QUOTE) {
                    if (*tmpptr == '\0') return -1;
                    if (tmpcnt > 199) return -2;
                    *tmpwrt = *tmpptr;
                    ++tmpptr;
                    ++tmpwrt;
                    ++tmpcnt;
                }
                *tmpwrt = '\0';
                state = 0;
                break;
            default:
                ;
        }
        ++tmpptr;
    }
    return 0;
}

int readFlow(const char* input)
{
    const char *ptr = input;
    double flowinbyte;
    ptr = strstr(ptr, "flow");
    if (ptr == NULL) 
    {
        printf("Failed to get user information!\n");
        return -1;
    }
    ptr = strstr(ptr, "value");
    if (ptr == NULL) 
    {
        printf("Failed to get user information!\n");
        return -2;
    }
    ptr += 10;
    flowinbyte = atof(ptr);
    if (flow_current.unit = (flowinbyte < GB))
        flow_current.flow_value = flowinbyte/(float)MB;
    else 
        flow_current.flow_value = flowinbyte/(float)GB;
    return 0;
}
    
int getIndex(const char* retstr)
{
    const char* rdptr;
    const char* tpptr = UID;
    char* wtptr = userIndex;
    int state = 0;
    int cnt;
    if (retstr == NULL) return -1;
    for (rdptr = retstr; *rdptr != '\0'; ++rdptr)
    {
        switch (state){
            case 0:
                if (*rdptr == 'u') {
                    state = 1;
                    tpptr = UID+1;
                }
                break;
            case 1:
                while(*rdptr != '\0')
                {
                    if (*rdptr == 'u') tpptr = UID;
                    else if (*rdptr != *tpptr) {
                        state = 0;
                        break;
                    }
                    ++rdptr;
                    ++tpptr;
                    if (*tpptr == 'x') {
                        state = 2;
                        break;
                    }
                }
                break;
            case 2:
                if (*rdptr == '=') state = 3;
                break;
            case 3:
                cnt = 0;
                while (cnt <= 199 && (*rdptr != '\0') && (*rdptr != '\n')) {
                    *wtptr++ = *rdptr++;
                    ++cnt;
                }
                *wtptr = '\0';
                state = 4;
                break;
            default:
                ;
        }
    }
    if (state == 4) return 0;
    return -1;
}

int readQuery(const char* input)
{
    const char *ptr;
    char *ptr_w = queryString;
    int tmpcnt;
    ptr = strstr(input, "wlanuserip");
    if (ptr == NULL) 
        return -1;
    while ( tmpcnt < 297 && (*ptr != '\r') ) 
    {
        switch (*ptr)
        {
            case '=' :
                *ptr_w ++ = '%';
                *ptr_w ++ = '3';
                *ptr_w ++ = 'D';
                break;
            case '&':
                *ptr_w ++ = '%';
                *ptr_w ++ = '2';
                *ptr_w ++ = '6';
                break;
            default:
                *ptr_w ++ = *ptr;
        }
        ++ ptr;
    }
    *ptr_w = '\0';
    return 0;
}
