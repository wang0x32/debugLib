#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdarg.h>
#include <dirent.h>
#include <pthread.h>

#include "debug.h"

#define LINE_SIZE 256
#define FILE_SIZE 1*1024*1024L

#define FILE_PATH "./log.txt"

pthread_mutex_t lock;
char* logPath = "/";   //日志文件目录
char logName[20] = "log1.txt";
int i = 1;

static int printf2terminal(char* _str)
{
    if(NULL == _str)
        return ERROR;
    
    printf("%s", _str);
    return SUCCESS;
}

void write2file(char* _str)
{
    int ret = ERROR;
    FILE* fd = NULL;
    long pos = 0;

    pthread_mutex_init(&lock, NULL);

    //计算文件大小是否超出
    fd = fopen(logName, "a+");
    if(fd < 0)
        return; 
    fseek(fd, 0L, SEEK_END);
    pos = ftell(fd);
    
    if((pos + strlen(_str)) > FILE_SIZE)
    {
        i = (1 == i) ? 2 : i;
        if(2 == i)
        {
            rename("log2.txt", "log1.txt");
            sprintf(logName, "log%d.txt", 2);
            fd = fopen(logName, "w+");
        }
        else
        {
            sprintf(logName, "log%d.txt", i);
            fd = fopen(logName, "a+");
        }
    }
    
    pthread_mutex_lock(&lock);
    fwrite(_str, sizeof(char), strlen(_str), fd);
    pthread_mutex_unlock(&lock);
    
    fclose(fd);
}

int JL_printf(LOG_LEVEL _logLevel, char* pszfmt, ...)
{
    va_list struAp;
    time_t now;
    struct tm tm_now;
    char result[LINE_SIZE] = "";
    char* format = NULL;
    char content[LINE_SIZE - 20] = {0};
    LOG_LEVEL logLevel = _logLevel;

    if(NULL == pszfmt)
        return -1;

    format = calloc(strlen(pszfmt)+3, 1);
    if(NULL == format)
        return -2;

    strcpy(format, pszfmt);
    if('\n' == format[strlen(pszfmt)-1])
    {
        format[strlen(pszfmt)] = format[strlen(pszfmt)-2] == '\r'?'\0':'\r';
    }
    else if('\r' == format[strlen(pszfmt)-1])
    {
        format[strlen(pszfmt)] = format[strlen(pszfmt)-2] == '\n'?'\0':'\n';
    }
    else
    {
        format[strlen(pszfmt)] = '\r';
        format[strlen(pszfmt)+1] = '\n';
    }
    
    va_start(struAp, pszfmt);
    vsprintf(content, format, struAp);
    va_end(struAp);

    //行头
    now = time(&now);
    localtime_r(&now, &tm_now);
    sprintf(result, "[%04d/%02d/%02d %02d:%02d:%02d]: %s",
                    tm_now.tm_year+1900,
                    tm_now.tm_mon+1,
                    tm_now.tm_mday,
                    (tm_now.tm_hour)%24,
                    tm_now.tm_min,
                    tm_now.tm_sec,
                    content);

    //printf2terminal(content);
    printf("%s", result);
    if(logLevel <= JL_LOG_WARNIG)
        write2file(result);
    
    free(format);
    return 0;
}

