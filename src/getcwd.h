#include <unistd.h>
#ifdef __APPLE__
#include <sys/param.h>
#elif defined(__linux__)
#include <linux/limits.h>
#else
#error "Your OS is currently unsupported, you will have to specify "\
       "a header for PATH_MAX yourself."
#endif
