#include <uapi/linux/ptrace.h>
#include <linux/time.h>

struct timespec {
    __s64 tv_sec;
    __s64 tv_nsec;
};
