
#include "Logger.h"

FILE * logfile = NULL;

LoggerInitializer::LoggerInitializer()
{
    logfile = ::fopen("iocp.log" , "a+b") ;
}

LoggerInitializer::~LoggerInitializer()
{
    if(logfile != NULL)
    {
        ::fflush(logfile) ;
        ::fclose(logfile) ;
        logfile = NULL ;
    }
}


static LoggerInitializer __logger_initializer__ ;

