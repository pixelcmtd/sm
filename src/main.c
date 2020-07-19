#include <stdio.h>
#include <stdlib.h>
#include "find_smfile.h"
#include "run_target.h"
#include "args.h"

#define PROGNAME "sm"
#define VERSION  "0.0.4"
#define YEARS    "2019-2020"
#define AUTHORS  "Chris Häußler, chrissx Media"
#define VERSIONINFO PROGNAME" "VERSION"\n(c) "YEARS" "AUTHORS

#define HELP VERSIONINFO"\n" \
"\n" \
"Usage: %s [OPTIONS] [TARGETS]\n" \
"\n" \
"If the targets are omitted, \"all\" is run.\n" \
"\n" \
"Options:\n" \
"\n" \
"  --\n" \
"    Treat all remaining arguments as targets.\n" \
"  -v, --version\n" \
"    Print version information.\n" \
"  -h, --help\n" \
"    Print this screen.\n" \
"\n"

#define STRARRSZ(arr) sizeof(arr) / sizeof(char *)

int main(int argc, char **argv)
{
        MAINSTART;
	char *smfile = find_smfile();
        bool no_target_ran = 1;
	if(!smfile) { puts("*** No Smfile found. ***"); return 1; }
        ARGSTART;
        ARG("v", "version") return  puts  (VERSIONINFO) < 0;
        ARG("h", "help")    return !printf(HELP, *argv);
        else no_target_ran = 0,
             run_target(smfile, argv[_args_argc_i], *argv);
        ARGEND;
        if(no_target_ran)
                run_target(smfile, "all", *argv);
        free(smfile);
        return 0;
}
