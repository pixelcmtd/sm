#include <stdio.h>
#include <stdlib.h>
#include "find_smfile.h"
#include "run_target.h"
#include "args.h"

#define PROGNAME "sm"
#define VERSION  "0.0.10"
#define YEARS    "2019-2020"
#define AUTHORS  "Chris Häußler, chrissx Media"
#define VERSIONINFO PROGNAME" "VERSION"\n(c) "YEARS" "AUTHORS

#define HELP VERSIONINFO"\n" \
"\n" \
"Usage: %s [OPTIONS] [TARGETS]\n" \
"\n" \
"If the TARGETS are omitted, \"all\" is run.\n" \
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

#define main_return { free(smfile); return res; }

int main(int argc, char **argv)
{
	char *smfile;
        bool no_target_ran;
        int res;
        MAINSTART;
	smfile = find_smfile();
	if(!smfile) { puts("*** No Smfile found. ***"); return 1; }
        no_target_ran = 1;
        ARGSTART;
        ARG("v", "version") return  puts  (VERSIONINFO) < 0;
        ARG("h", "help")    return !printf(HELP, *argv);
        else
        {
                no_target_ran = 0;
                res = run_target(smfile, argv[_args_argc_i], *argv);
                if(res) main_return;
        }
        ARGEND;
        if(no_target_ran) res = run_target(smfile, "all", *argv);
        main_return;
}
