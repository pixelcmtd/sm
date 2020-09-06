#include "run_builtin.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "system.h"

#define BUILTINSTART    if(0)
#define BUILTIN(s) else if(!strcmp(builtin, s))
#define BUILTINEND else printf("Omitting unrecognized builtin \"%s\".\n", builtin)

#define _SET(var, val) { \
        if(var) free(var); \
        var = (char *) malloc(strlen(val) + 2); \
        strcpy(var, val); \
}
#define SETNAMESTART    if(0)
#define SETNAME(s, var) else if(!strcmp(name, s)) _SET(var, args)
#define SETNAMEEND      else printf("Not setting unrecognized variable \"%s\".\n", name)

#define _APPEND(var, val) {\
        if(!var) {\
                var = (char *) malloc(strlen(val) + 2); \
                strcpy(var, val); \
        } else {\
                char *tmpvar = (char *) malloc(strlen(val) + strlen(var) + 2);\
                sprintf(tmpvar, "%s %s", var, val); \
                free(var); \
                var = tmpvar; \
        }\
}
#define APPENDNAMESTART    if(0)
#define APPENDNAME(s, var) else if(!strcmp(name, s)) _APPEND(var, args)
#define APPENDNAMEEND      else printf("Not appending to unrecognized variable \"%s\".\n", name)

int first_index_of(char *s, char c)
{
        int i;
        for(i = 0; s[i]; i++)
                if(s[i] == c)
                        return i;
        return -1;
}

char *cc = NULL, *cflags = NULL, *cppc = NULL, *cppflags = NULL;
#ifdef _WIN32
#define DEFAULT_CC       "cl"
#define DEFAULT_CFLAGS   "/c"
#define DEFAULT_CPPC     "cl"
#define DEFAULT_CPPFLAGS "/c"
#else
#define DEFAULT_CC       "gcc"
#define DEFAULT_CFLAGS   "-Wall -Wextra -pedantic -O2 -s"
#define DEFAULT_CPPC     "g++"
#define DEFAULT_CPPFLAGS "-Wall -Wextra -pedantic -std=c++2a -O2 -s"
#endif

void check_vars()
{
        if(!cc)       _SET(cc,       DEFAULT_CC);
        if(!cppc)     _SET(cppc,     DEFAULT_CPPC);
        if(!cflags)   _SET(cflags,   DEFAULT_CFLAGS);
        if(!cppflags) _SET(cppflags, DEFAULT_CPPFLAGS);
}

void CC(char *args)
{
        int i = first_index_of(args, ' ');
        char *output, *cmd;
        args[i] = '\0';
        output = args;
        args += i + 1;
        cmd = (char *) malloc(strlen(cc) + strlen(args) +
                              strlen(output) + strlen(cflags) + 16);
        #ifdef _WIN32
        /*
         * TODO: append dynamic extensions like so dll exe when needed
         * TODO: only using separate compiling and linking on windows
         *       is not the best idea, for stupid reasons some unix
         *       users might also want to do that (actually now that i
         *       am building an os i know good reasons to do that)
         */
        sprintf(cmd, "%s %s %s & link /OUT:%s.exe *.obj", cc, cflags, args, output);
        #else
        sprintf(cmd, "%s %s -o %s %s", cc, args, output, cflags);
        #endif
        SYSTEM(cmd);
        free(cmd);
}

/*
 * TODO: make this more generic
 */
void CPPC(char *args)
{
        int i = first_index_of(args, ' ');
        char *output, *cmd;
        args[i] = '\0';
        output = args;
        args += i + 1;
        cmd = (char *) malloc(strlen(cppc) + strlen(args) +
                              strlen(output) + strlen(cppflags) + 16);
        #ifdef _WIN32
        /*
         * TODO: check if this actually works
         */
        sprintf(cmd, "%s %s %s & link /OUT:%s.exe *.obj", cppc, cppflags, args, output);
        #else
        sprintf(cmd, "%s %s -o %s %s", cppc, args, output, cppflags);
        #endif
        SYSTEM(cmd);
        free(cmd);
}

void SET(char *args)
{
        int i = first_index_of(args, ' ');
        char *name;
        args[i] = '\0';
        name = args;
        args += i + 1;
        SETNAMESTART;
        SETNAME("CFLAGS", cflags)
        SETNAME("CC", cc)
        SETNAME("CPPFLAGS", cppflags)
        SETNAME("CPPC", cppc)
        SETNAMEEND;
}

void APPEND(char *args)
{
        int i = first_index_of(args, ' ');
        char *name;
        args[i] = '\0';
        name = args;
        args += i + 1;
        APPENDNAMESTART;
        APPENDNAME("CFLAGS", cflags)
        APPENDNAME("CC", cc)
        APPENDNAME("CPPFLAGS", cppflags)
        APPENDNAME("CPPC", cppc)
        APPENDNAMEEND;
}

void run_builtin(char *cmd)
{
        int i;
        char *builtin;
        check_vars();
        i = first_index_of(cmd, ' ');
        cmd[i] = '\0';
        builtin = cmd;
        cmd += i + 1;
        BUILTINSTART;
        BUILTIN("CC")     CC    (cmd);
        BUILTIN("CPPC")   CPPC  (cmd);
        BUILTIN("SET")    SET   (cmd);
        BUILTIN("APPEND") APPEND(cmd);
        BUILTINEND;
}
