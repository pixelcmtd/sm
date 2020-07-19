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

int first_index_of(char *s, char c)
{
        int i;
        for(i = 0; s[i]; i++)
                if(s[i] == c)
                        return i;
        return -1;
}

char *cc = NULL, *cflags = NULL;
#define DEFAULT_CC     "gcc"
#define DEFAULT_CFLAGS "-O2 -s"

void check_vars()
{
        if(!cc)     _SET(cc,     DEFAULT_CC);
        if(!cflags) _SET(cflags, DEFAULT_CFLAGS);
}

void CC(char *args)
{
        int i = first_index_of(args, ' ');
        char *output, *cmd;
        args[i] = '\0';
        output = args;
        args += i + 1;
        cmd = malloc(strlen(cc) + strlen(args) +
                     strlen(output) + strlen(cflags) + 16);
        sprintf(cmd, "%s %s -o %s %s", cc, args, output, cflags);
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
        SETNAMEEND;
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
        BUILTIN("CC" ) CC (cmd);
        BUILTIN("SET") SET(cmd);
        BUILTINEND;
}
