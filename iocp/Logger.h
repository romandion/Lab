
#ifndef __LOGGER_H
#define __LOGGER_H 1

#include <stdio.h>
#include <stdlib.h>

extern FILE * logfile;

class LoggerInitializer{
public:
    LoggerInitializer() ;
    ~LoggerInitializer() ;
} ;

#endif /** __LOGGER_H */
