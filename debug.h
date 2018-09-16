#ifndef _DEBUG_H
#define _DEBUG_H

#ifdef __cplusplus 
extern "C" {
#endif

#define ERROR    -1
#define SUCCESS  0

typedef enum
{
    JL_LOG_ERROR = 0,
    JL_LOG_WARNIG,   //error和warning记日志
    JL_LOG_DEBUG,
    JL_LOG_NOTICE,
    JL_LOG_INFO,
}LOG_LEVEL;

int JL_printf(LOG_LEVEL _logLevel, char* pszfmt, ...);

#ifdef __cplusplus
}
#endif 

#endif
