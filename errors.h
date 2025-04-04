/* Copyright (c) 2025, Harkaitz Agirre, All rights reserved, ISC License */
/* errors.h is documented in https://github.com/harkaitz/c-errors */
#ifndef ERRORS_H
#define ERRORS_H

#ifdef __GNUC__
#  define THREAD_LOCAL __thread
#else
#  ifdef _WIN32
#    define THREAD_LOCAL __declspec(thread)
#  endif
#endif
#ifndef THREAD_LOCAL
#  define THREAD_LOCAL
#  ifndef SKIP_THREAD_LOCAL_WARNING
#    warning "THREAD_LOCAL not defined for this platform"
#  endif
#endif

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#ifdef __unix__
#  include <errno.h>
#  include <syslog.h>
#else
#  define LOG_EMERG    0  /* system is unusable */
#  define LOG_ALERT    1  /* action must be taken immediately */
#  define LOG_CRIT     2  /* critical conditions */
#  define LOG_ERR      3  /* error conditions */
#  define LOG_WARNING  4  /* warning conditions */
#  define LOG_NOTICE   5  /* normal but significant condition */
#  define LOG_INFO     6  /* informational */
#  define LOG_DEBUG    7  /* debug-level messages */
#endif
#ifdef _WIN32
#  include <errno.h>
#endif

typedef const char *error_t;

#ifdef PROGRAM_NAME
THREAD_LOCAL char log_error[1024];
char const *log_program_name = PROGRAM_NAME;
FILE       *log_file = NULL;
int         log_priority = 
#  ifdef DEBUG
    7
#  else
    6
#  endif
    ;
#else
extern THREAD_LOCAL char log_error[1024];
extern char const *log_program_name;
extern FILE       *log_file;
extern int         log_priority;
#endif

static inline error_t
error(const char *_fmt, ...)
{
	static 
	va_list ap;
	va_start(ap, _fmt);
	vsnprintf(log_error, sizeof(log_error)-1, _fmt, ap);
	va_end(ap);
	return log_error;
}

static inline void
slog(int priority, const char *_fmt, ...)
{
	va_list ap;
	char const *prefix;
	if (priority > log_priority) {
		return;
	}

	switch (priority) {
	case LOG_EMERG:   prefix = "emergency: "; break;
	case LOG_ALERT:   prefix = "alert: ";     break;
	case LOG_CRIT:    prefix = "critical: ";  break;
	case LOG_ERR:     prefix = "error: ";     break;
	case LOG_WARNING: prefix = "warning: ";   break;
	case LOG_NOTICE:  prefix = "notice: ";    break;
	case LOG_INFO:    prefix = "info: ";      break;
	case LOG_DEBUG:   prefix = "debug: ";     break;
	default:          prefix = ""; break;
	}

	if (strcmp(log_program_name, "")) {
		fprintf((log_file)?log_file:stderr, "%s: ", log_program_name);
	}
	fprintf((log_file)?log_file:stderr, "%s", prefix);
	
	va_start(ap, _fmt);
	vfprintf((log_file)?log_file:stderr, _fmt, ap);
	va_end(ap);
	
	fputs("\n", (log_file)?log_file:stderr);
	fflush((log_file)?log_file:stderr);
}

static inline error_t
errstr()
{
	return strerror(errno);
}

static inline int
strprio(char const *_level_name)
{
	if (!strncmp(_level_name, "emerg", 5) || !strcmp(_level_name, "0")) {
		return 0;
	} else if (!strncmp(_level_name, "alert", 5) || !strcmp(_level_name, "1")) {
		return 1;
	} else if (!strncmp(_level_name, "crit", 4) || !strcmp(_level_name, "2")) {
		return 2;
	} else if (!strncmp(_level_name, "err", 3) || !strcmp(_level_name, "3")) {
		return 3;
	} else if (!strncmp(_level_name, "warning", 7) || !strcmp(_level_name, "4")) {
		return 4;
	} else if (!strncmp(_level_name, "notice", 6) || !strcmp(_level_name, "5")) {
		return 5;
	} else if (!strncmp(_level_name, "info", 4) || !strcmp(_level_name, "6")) {
		return 6;
	} else if (!strncmp(_level_name, "debug", 5) || !strcmp(_level_name, "7")) {
		return 7;
	} else {
		return -1;
	}
}

#endif
