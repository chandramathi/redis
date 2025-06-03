#ifndef LOGGER_H
#define LOGGER_H

typedef enum {
    MYLOG_FINE,
    MYLOG_FINER,
    MYLOG_FINEST,
    MYLOG_DEBUG,
    MYLOG_WARNING,
    MYLOG_ERROR
} LogLevel;

typedef void (*LoggerFunc)(LogLevel level, const char *msg, int printTimestamp, int printIP);

void setLogger(LoggerFunc logger);
void logMessage(LogLevel level, const char *msg, int printTimestamp, int printIP);  

void logConsoleMessage(LogLevel level, const char *msg, int printTimestamp, int printIP);
void logToFile(LogLevel level, const char *msg, int printTimestamp, int printIP);

#endif
