/*
 * this header - despite the name - doesn't only define getcwd, but
 * also R_OK and PATH_MAX
 */

#ifdef __APPLE__
#include <unistd.h>
#include <sys/param.h>
#elif defined(__linux__)
#include <unistd.h>
#include <linux/limits.h>
#elif defined(_WIN32)
#include <Windows.h>
#define PATH_MAX MAX_PATH
#define R_OK 4
#define getcwd(bfr) _getcwd(bfr, MAX_PATH)
#else
#error "Your OS is currently unsupported, you will have to specify "\
       "a header for getcwd and PATH_MAX yourself. If this piece of "\
       "software is still somewhat supported (it's the 2020s and I "\
       "am still alive), please make a PR on GitHub. (chrissxYT/sm)"
#endif
