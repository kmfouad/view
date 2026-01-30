#ifndef NKM_ESTD_H
#define NKM_ESTD_H

#include <stdio.h>
#include <stdlib.h>

/*  Macro helpers */
#define FIRST_ARG(arg1, ...) arg1

#define die(fname, str) \
    do { \
        fprintf(stderr, "%s: %s\n", fname, str); \
        exit(1); \
    } while(0) 

#define diearg(fname, arg, str) \
    do { \
        fprintf(stderr, "%s: '%s': %s\n", fname, arg, str); \
        exit(1); \
    } while(0)

#define bind_error(func, condition, string, ...) \
    ({ \
        typeof(func(__VA_ARGS__)) _result = func(__VA_ARGS__); \
        if(condition) { \
            die(#func, string); \
        } \
        _result; \
    })

#define bind_error_arg(func, condition, string, ...) \
    ({ \
        typeof(func(__VA_ARGS__)) _result = func(__VA_ARGS__); \
        if(condition) { \
            diearg(#func, string, FIRST_ARG(__VA_ARGS__)); \
        } \
        _result; \
    })

// Files
#define eopen(...) bind_error(open, _result == -1, "Couldn't open file", __VA_ARGS__)
#define eopendir(...) bind_error_arg(opendir, !_result, "Couldn't open directory", __VA_ARGS__)
#define efopen(...) bind_error_arg(fopen, !_result, "Couldn't open file", __VA_ARGS__)
#define echdir(...) bind_error_arg(chdir, _result, "Directory doesn't exist", __VA_ARGS__)

// Memory
#define emalloc(...) bind_error(malloc, !_result, "Out of memory", __VA_ARGS__)
#define erealloc(...) bind_error(realloc, !_result, "Out of memory", __VA_ARGS__)
#define emmap(...) bind_error(mmap, !_result, "Out of memory", __VA_ARGS__)
#define ecalloc(...) bind_error(calloc, !_result, "Out of memory", __VA_ARGS__)

// String
#define estrdup(...) bind_error(strdup, !_result, "Out of memory", __VA_ARGS__)

// misc
#define eioctl(...) bind_error(ioctl, _result == -1, "ioctl error", __VA_ARGS__)
#define egetenv(...) bind_error_arg(getenv, !_result, "Is not set", __VA_ARGS__)


#endif
