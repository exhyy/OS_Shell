#include "utils.h"

int str_is_int(const char *str)
{
    const char *p = str;
    if(*p == '-')
        p++;
    while(*p != '\0')
    {
        if (*p < '0' || *p > '9')
            return 0;
        p++;
    }
    return 1;
}
