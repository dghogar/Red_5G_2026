#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "telit_sdk.h"

#if !defined(ANDROID)
static int min_log_level = LOG_INFO;
static bool to_syslog = false;

typedef struct {
    char *tag;
    int level;
} log_data_t;

static log_data_t logdata [] = {
    {"EMERG", LOG_EMERG},  /* system is unusable */
    {"ALERT", LOG_ALERT},  /* action must be taken immediately */
    {"CRIT", LOG_CRIT},    /* critical conditions */
    {"ERROR", LOG_ERR},    /* error conditions */
    {"WARN", LOG_WARNING}, /* warning conditions */
    {"NOT", LOG_NOTICE},   /* normal but significant condition */
    {"INFO", LOG_INFO},    /* informational */
    {"DBG", LOG_DEBUG}  /* debug-level messages */
};

int telit_set_log_level(int level)
{
    int ret = -1;

    if (level != LOG_NOTICE &&
            level >= LOG_ERR &&
            level <= LOG_DEBUG) {
        ret = min_log_level = level;
    }

    return ret;
}

void telit_set_syslog(bool enable)
{
    to_syslog = enable;
}

void telit_log_print(int level, char *fmt, ...)
{
    FILE *std_log;

    if (level < 0 ||
            level > LOG_DEBUG ||
            level > min_log_level) {
        return;
    }

    if (fmt == NULL) {
        fmt = "error: fmt == NULL";
#ifdef EXIT_ON_ERROR
        exit(EXIT_FAILURE);
#endif
    }

    va_list args;
    va_start(args, fmt);

    if (to_syslog) {
#if !defined (__WINDOWS_QMI__)
        openlog("telitSDK", LOG_CONS, LOG_USER);
        vsyslog(logdata[level].level, fmt, args);
        closelog();
#endif
    } else {
        struct timespec ts = {0};

        std_log = (level == LOG_ERR) ? stderr : stdout;
        if (timespec_get(&ts, TIME_UTC)) {
#if defined (__WINDOWS_QMI__)
            fprintf(std_log, "[%lld.%ld] %s: ", ts.tv_sec, ts.tv_nsec, logdata[level].tag);
#else
            fprintf(std_log, "[%ld.%.9ld] %s: ", ts.tv_sec, ts.tv_nsec, logdata[level].tag);
#endif
        } else {
            fprintf(std_log, "%s: ", logdata[level].tag);
        }
        vfprintf(std_log, fmt, args);
        fprintf(std_log, "\n");
    }

    va_end(args);
}
#endif

#define LOG_BYTES_PER_ROW 16
#define LOG_PRINTED_BYTE_SIZE 3
#define LOG_BUFFER_STRING_SIZE (LOG_BYTES_PER_ROW * LOG_PRINTED_BYTE_SIZE + 1)

void telit_print_buffer(uint8_t *buffer, uint32_t len, int level)
{
    char string[LOG_BUFFER_STRING_SIZE] = {0};
    uint32_t offset = 0;

    if (!buffer) {
#if !defined(ANDROID)
        telit_log_print(LOG_ERR, "Passed buffer pointer is null");
#else
        RLOGE("%s: Passed buffer pointer is null", __FUNCTION__);
#endif
        return;
    }

#if !defined(ANDROID)
    telit_log_print(level, "Printing buffer %p of %u bytes", buffer, len);
#else
    if (level == LOG_DEBUG) {
        RLOGD("Printing buffer %p of %u bytes", buffer, len);
    } else if (level == LOG_ERR) {
        RLOGE("Printing buffer %p of %u bytes", buffer, len);
    }
#endif

    while (offset < len) {
        uint32_t i = 0;

        for (i = 0; i < LOG_BYTES_PER_ROW && offset < len; i++, offset++) {
            char tmp[LOG_PRINTED_BYTE_SIZE + 1] = {0};

            snprintf(tmp, LOG_PRINTED_BYTE_SIZE + 1, "%02X ", buffer[offset]);
            memcpy(&string[i * LOG_PRINTED_BYTE_SIZE], tmp, LOG_PRINTED_BYTE_SIZE);
        }

#if !defined(ANDROID)
        telit_log_print(level, "[%04u] %s", offset - i, string);
#else
        if (level == LOG_DEBUG) {
            RLOGD("[%04u] %s", offset - i, string);
        } else if (level == LOG_ERR) {
            RLOGE("[%04u] %s", offset - i, string);
        }
#endif
        memset(string, '\0', LOG_BUFFER_STRING_SIZE);
    }
}
