#include "server.h"
#include "logger.h"
#include <time.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

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

void logConsoleMessage(LogLevel level, const char *msg, int printTimestamp, int printIP) {
    int redisLogLevel;

    switch (level) {
        case MYLOG_FINE:
        case MYLOG_FINER:
        case MYLOG_FINEST: redisLogLevel = LL_VERBOSE; break;
        case MYLOG_DEBUG:  redisLogLevel = LL_DEBUG; break;
        case MYLOG_WARNING:
        case MYLOG_ERROR:  redisLogLevel = LL_WARNING; break;
        default:           redisLogLevel = LL_NOTICE;
    }

    char timestamp[32] = "", ip[INET_ADDRSTRLEN] = "", fullMsg[1024];
    if (printTimestamp) getTimestamp(timestamp, sizeof(timestamp));
    if (printIP) getServerIP(ip, sizeof(ip));

    if (printTimestamp && printIP)
        snprintf(fullMsg, sizeof(fullMsg), "[%s] [IP: %s] %s", timestamp, ip, msg);
    else if (printTimestamp)
        snprintf(fullMsg, sizeof(fullMsg), "[%s] %s", timestamp, msg);
    else if (printIP)
        snprintf(fullMsg, sizeof(fullMsg), "[IP: %s] %s", ip, msg);
    else
        snprintf(fullMsg, sizeof(fullMsg), "%s", msg);

    serverLog(redisLogLevel, "%s", fullMsg);
}
