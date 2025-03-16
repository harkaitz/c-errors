# C-ERRORS

Small and secure ANSI C error handling and logging library.

## How to

This library defines an error as a string:

    typedef const char *error_t;

And expects that functions return an error or `NULL` if no error occurred.

    error_t my_function(void) {
        if (some_error) {
            return error("Error %i happened!", 1);
        }
    }
    
    error_t e = my_function();
    if (e) {
        slog(LOG_ERR, "%s", e);
        return 1;
    }

The following API is provided:

    THREAD_LOCAL char log_error[1024];
    char const *log_program_name;
    FILE       *log_file = NULL;
    int         log_priority = 7 (if DEBUG defined) 6 (in release)

    error_t error(const char *_fmt, ...);
    void    slog(int priority, const char *_fmt, ...);
    error_t errstr(void); // Wrapper around "strerror(errno)".
    int     strprio(char const *_level_name);

## Important considerations

The global variables are defined if `PROGRAM_NAME` is defined, so it
is required to place the following in one (and only one) of your `.c`
files.

    #define PROGRAM_NAME "my-program"
    #include "errors.h"

Normaly in your main.c file.

The `error()` functions saves the error message in thread-local global
"log_error" variable.

    THREAD_LOCAL char log_error[1024];

If thread locals aren't supported a warning is displayed when compiling, you
can disable the warning by defining `SKIP_THREAD_LOCAL_WARNING`.

## Copying

I recommend to simply copy this file into your codebase, you can remove
the first line if you wish, al least keed the second line so that the
person reading it can find this documentation.

## Style guide

This project follows the OpenBSD kernel source file style guide (KNF).

Read the guide [here](https://man.openbsd.org/style).

## Collaborating

For making bug reports, feature requests, support or consulting visit
one of the following links:

1. [gemini://harkadev.com/oss/](gemini://harkadev.com/oss/)
2. [https://harkadev.com/oss/](https://harkadev.com/oss/)
