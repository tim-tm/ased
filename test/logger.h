/**
 *  logger.h
 *
 *  logger.h is a simple STB-Style library for dealing with logging in C.
 *  
 *  If you want the implementation, this should be defined once:
 *      #define LOGGER_IMPLEMENTATION
 *      #include <logger.h>
 *
 *
 *  MIT License
 *
 *  Copyright (c) 2024 Tim Teichmann
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#ifndef LOGGER_H
#define LOGGER_H
#include <stdio.h>

#ifndef LOGGER_MAX_MESSAGE_SIZE
#define LOGGER_MAX_MESSAGE_SIZE 16384
#endif // !LOGGER_MAX_MESSAGE_SIZE

#define LOGGER_LOG(level, ...) logger_log_file(stdout, (level), ##__VA_ARGS__)

#if _MSC_VER
#include <intrin.h>
#define LOGGER_DEBUG_BREAK() __debugbreak()
#else
#include <signal.h>
#define LOGGER_DEBUG_BREAK() raise(SIGTRAP)
#endif // _MSC_VER

#define LOGGER_ASSERT_MSG(expr, ...) \
    { if (expr) {} else { logger_log_assertion(#expr, ##__VA_ARGS__, __FILE__, __LINE__); LOGGER_DEBUG_BREAK(); } }

#define LOGGER_ASSERT(expr) \
    { if (expr) {} else { logger_log_assertion(#expr, "", __FILE__, __LINE__); LOGGER_DEBUG_BREAK(); } }

typedef enum _LoggerLogLevel_ {
    LOGGER_LOG_LEVEL_DEBUG = 0,
    LOGGER_LOG_LEVEL_INFO = 1,
    LOGGER_LOG_LEVEL_WARNING = 2,
    LOGGER_LOG_LEVEL_ERROR = 3,
    LOGGER_LOG_LEVEL_FATAL = 4
} LoggerLogLevel;

/**
 *  logger_get_level_str
 *
 *  Get the string for the enum value.
 *  Return:
 *      - string on success
 *      - NULL on failure
 */
const char *logger_get_level_str(LoggerLogLevel level);

/**
 *  logger_log_file
 *
 *  Prints the message 'fmt' to 'fp' with LogLevel of 'level'.
 */
void logger_log_file(FILE *fp, LoggerLogLevel level, const char *fmt, ...);

/**
 *  logger_log_assertion
 *
 *  Prints an failed assertion.
 */
void logger_log_assertion(const char *expr, const char *msg, const char *file, size_t line);

#ifdef LOGGER_IMPLEMENTATION
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

const char *logger_get_level_str(LoggerLogLevel level) {
    char *levels[] = {"[DEBUG]: ", "[INFO]: ", "[WARNING]: ", "[ERROR]: ", "[FATAL]: "};
    if (level < 0 || level > 4) return NULL;
    return levels[level];
}

void logger_log_file(FILE *fp, LoggerLogLevel level, const char *fmt, ...) {
    if (fp == NULL || fmt == NULL) return;

    const char *log_str = logger_get_level_str(level);
    if (log_str == NULL) return;
    size_t len = strnlen(log_str, LOGGER_MAX_MESSAGE_SIZE);

    size_t fmt_eval_len = LOGGER_MAX_MESSAGE_SIZE-len;
    char *fmt_eval = calloc(fmt_eval_len, sizeof(char));
    if (fmt_eval == NULL) return;

    va_list arg_ptr;
    va_start(arg_ptr, fmt);
    vsnprintf(fmt_eval, fmt_eval_len, fmt, arg_ptr);
    va_end(arg_ptr);

    char *str = calloc(LOGGER_MAX_MESSAGE_SIZE, sizeof(char));
    if (str == NULL) {
        free(fmt_eval);
        return;
    }
    
    snprintf(str, LOGGER_MAX_MESSAGE_SIZE, "%s%s\n", log_str, fmt_eval);
    fprintf(fp, "%s", str);

    free(fmt_eval);
    free(str);
}

void logger_log_assertion(const char *expr, const char *msg, const char *file, size_t line) {
    if (expr == NULL || file == NULL) return;
    (void)line;

    if (msg == NULL || strcmp(msg, "") == 0) {
        LOGGER_LOG(LOGGER_LOG_LEVEL_FATAL, "Assertion failed, %s, %s:%zu", expr, file, line);
    } else {
        LOGGER_LOG(LOGGER_LOG_LEVEL_FATAL, "%s, %s, %s:%zu", msg, expr, file, line);
    }
}

#endif // LOGGER_IMPLEMENTATION
#endif // !LOGGER_H
