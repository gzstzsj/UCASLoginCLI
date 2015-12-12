#include <stdio.h>
#include "../include/connect.h"

static const char* USAGE = "Usage: ucasnet [-iof]\n\tEither \'i\' or \'o\' should be specified.\n\tUse a combination of \'i\' and \'f\' \
to request remaining flow after successful login attempt.\n";

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
