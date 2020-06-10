#ifdef __APPLE__
#include <unistd.h>
#include <sys/param.h>
#elif defined(__linux__)
#include <unistd.h>
#include <linux/limits.h>
#elif defined(_WIN32)
#include <direct.h>
#else
#error "Your OS is currently unsupported, you will have to specify "\
       "a header for getcwd and PATH_MAX yourself."
#endif
