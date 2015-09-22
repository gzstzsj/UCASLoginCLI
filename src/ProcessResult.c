#include <stdio.h>
#define SEPARATOR ','
#define QUOTE '"'

static const char* RES = "result\"";
static const char* MES = "message\"";
static const char* UID = "userIndex\"";

char result[15];
char messages[40];
char userIndex[200];

int readMessages(const char* input)
{
    const char* tmpptr = input;
    const char* tmpcmp;
    char* tmpwrt;
    int state = 0;
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
                tmpwrt = result;
                while (*tmpptr != QUOTE) {
                    if (*tmpptr == '\0') return -1;
                    *tmpwrt = *tmpptr;
                    ++tmpptr;
                    ++tmpwrt;
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
                while (*tmpptr != QUOTE) {
                    if (*tmpptr == '\0') return -1;
                    *tmpwrt = *tmpptr;
                    ++tmpptr;
                    ++tmpwrt;
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
                while (*tmpptr != QUOTE) {
                    if (*tmpptr == '\0') return -1;
                    *tmpwrt = *tmpptr;
                    ++tmpptr;
                    ++tmpwrt;
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
