#pragma once
#include <stdbool.h>

/*
 * runs target specified
 * from smfile
 * using argv0 to call sudo when necessary
 *
 * when target is marked as root in smfile,
 * the return value of system is returned.
 * else 0 is returned after successful execution,
 * non-zero return values indicate errors like:
 *   target wasn't found in smfile
 */
int run_target(char *smfile, char *target, char *argv0);
