#ifndef __UCAS_CONNECT_H__
#define __UCAS_CONNECT_H__
#define MAXLINE 4096

struct flow {
    int unit;
    float flow_value;
} flow_current;

int readMessages(const char*);
int readFlow(const char*);
int getIndex(const char*);
int readQuery(const char*);

int gfflag;

char result[15];
char messages[50];
char userIndex[200];
char queryString[300];

int get_success(void);
int login(void);
int logout(void);
int getflow(void);

#endif
