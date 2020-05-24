#include <string.h>

#define ARGSTART bool inargs = 1; \
        for(int i = 1; i < argc; i++) { \
                if(!strcmp(argv[i], "--")) inargs = 0
#define ARG(unix, gnu) \
                else if(inargs && (!strcmp(argv[i], "-"  unix) || \
                                   !strcmp(argv[i], "--" gnu)))
#define ARGEND }
