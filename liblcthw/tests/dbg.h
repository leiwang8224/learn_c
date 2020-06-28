#ifndef __dbg_h__
#define __dbg_h__

#include <stdio.h>
#include <errno.h>
#include <string.h>

// start of #ifdef that lets you recompile your program so that all
// of the debug log messages are removed.
// if you compile with NDEBUG defined, then 'no debug' messages
// will remain. 
#ifdef NDEBUG
#define debug(M, ...)
#else
// the alternative #define debug that translates any use of
// debug("format", arg1, arg2) into an fprintf call to stderr.
// Many c programmers don't know this but you can create macros
// that actually work like printf and take variable arguments.
// The magic here is the use of ##__VA_ARGS__ that says 'put
// whatever they had for extra args(...) here.' Also note the
// use of __FILE__ and __LINE__ to get the current file:line
// for the debug message.
#define debug(M, ...) fprintf(stderr, "DEBUG %s:%d: " M "\n", \
	__FILE__, __LINE__, ##__VA_ARGS__)
#endif

// the clean_errno macro that's used in the others to get a safe
// readable version of errno. That strange syntax in the middle
// is ternary operator and you will learn what it does later.
#define clean_errno() (errno == 0 ? "None" : strerror(errno))

// log_err, log_warn, and log_info, macros for logging messages
// that are meant for the end user. They work like debug but can't
// be compiled out.
#define log_err(M, ...) fprintf(stderr, \
	"[ERROR] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, \
	clean_errno(), ##__VA_ARGS__)

#define log_warn(M, ...) fprintf(stderr, \
	"[WARN] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, \
	clean_errno(), ##__VA_ARGS__)

#define log_info(M, ...) fprintf(stderr, "[INFO] (%s:%d) " M "\n", \
	__FILE__, __LINE__, ##__VA_ARGS__)

// check will make sure the condition A is true, and if not, it logs
// the error M(with variable args for log_err), and then jumps
// to the function's error: for cleanup
#define check(A, M, ...) if(!(A)) {\
	log_err(M, ##__VA_ARGS__); errno=0; goto error; }

// sentinel is placed in any part of a function that shouldn't be run
// and if it does, it prints an error message and then jumps to error:
// label. YOu put this in if-statements and switch statements to catch
// conditions that shouldn't happen, like the default:
#define sentinel(M, ...) { log_err(M, ##_VA_ARGS__); \
	errno = 0; goto error;}

// a shorthand macro called check_mem that makes sure a pointer is
// valid, and if it isn't, it reports it as an error with "Out of mem"
#define check_mem(A) check((A), "Out of memory.")

// an alternative macro, check_debug, which still checks and handles an
// error, but if the error is common, then it doesn't bother reporting
// it. In this one, it willl use debug instead of log_err to report the 
// message. So when you define NDEBUG, the check still happens,
// and the error jump goes off, but the message isn't printed.
#define check_debug(A, M, ...) if (!(A)) {debug(M, ##__VA_ARGS__);\
	errno=0; goto error;}

#endif
