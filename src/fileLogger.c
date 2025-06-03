#include "logger.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

static LoggerFunc currentLogger = logConsoleMessage; // default

static const char *logLevelToString(LogLevel level) {
    switch (level) {
        case MYLOG_FINE: return "FINE";
        case MYLOG_FINER: return "FINER";
        case MYLOG_FINEST: return "FINEST";
        case MYLOG_DEBUG: return "DEBUG";
        case MYLOG_WARNING: return "WARNING";
        case MYLOG_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

static void getTimestamp(char *buffer, size_t size) {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", tm_info);
}

static void getServerIP(char *ip, size_t size) {
    char hostname[256];
    struct hostent *host_entry;
    gethostname(hostname, sizeof(hostname));
    host_entry = gethostbyname(hostname);
    if (host_entry && host_entry->h_addrtype == AF_INET) {
        inet_ntop(AF_INET, host_entry->h_addr_list[0], ip, size);
    } else {
        strncpy(ip, "UnknownIP", size);
    }
}

void logToFile(LogLevel level, const char *msg, int printTimestamp, int printIP) {
    FILE *logFile = fopen("redis_logs.txt", "a");
    if (!logFile) return;

    char timestamp[32] = "", ip[INET_ADDRSTRLEN] = "", fullMsg[1024];
    if (printTimestamp) getTimestamp(timestamp, sizeof(timestamp));
    if (printIP) getServerIP(ip, sizeof(ip));

    if (printTimestamp && printIP)
        snprintf(fullMsg, sizeof(fullMsg), "[%s] [IP: %s] [%s] %s\n", timestamp, ip, logLevelToString(level), msg);
    else if (printTimestamp)
        snprintf(fullMsg, sizeof(fullMsg), "[%s] [%s] %s\n", timestamp, logLevelToString(level), msg);
    else if (printIP)
        snprintf(fullMsg, sizeof(fullMsg), "[IP: %s] [%s] %s\n", ip, logLevelToString(level), msg);
    else
        snprintf(fullMsg, sizeof(fullMsg), "[%s] %s\n", logLevelToString(level), msg);

    fputs(fullMsg, logFile);
    fclose(logFile);
}

void setLogger(LoggerFunc logger) {
    currentLogger = logger;
}

void logMessage(LogLevel level, const char *msg, int printTimestamp, int printIP) {
    currentLogger(level, msg, printTimestamp, printIP);
}
