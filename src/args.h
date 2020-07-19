#include <string.h>

#define MAINSTART int _args_inargs, _args_argc_i;

#define ARGSTART _args_inargs = 1; \
                 for(_args_argc_i = 1; \
                     _args_argc_i < argc; \
                     _args_argc_i++) { \
                         if(!strcmp(argv[_args_argc_i], "--")) \
                                 _args_inargs = 0
#define ARG(unix, gnu) \
                else if(_args_inargs && \
                          (!strcmp(argv[_args_argc_i], "-"  unix) || \
                           !strcmp(argv[_args_argc_i], "--" gnu)))
#define ARGEND }
