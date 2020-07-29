#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "run_command.h"
#include "system.h"

#define NEWLINE(c) ((c) == '\r' || (c) == '\n')
#define SON(c) ((c) == ' ' || (c) == '\t' || NEWLINE(c))
#define CREADF c = fgetc(f)
#define CWRITED *d++ = c
#define READ_WHILE(b) for(CREADF; (b) && c != EOF; CREADF)
#define COPY_WHILE(b) READ_WHILE(b) CWRITED;
#define SKIP_WHILE(b) while(b) c = fgetc(f)

#define OPTIONSTART if(0)
#define OPTION(s) else if(!strcmp(s, bfr))
#define OPTIONEND else printf("Omitting unknown option \"%s\".\n", bfr);

#define TRETURN fclose(f); free(bfr); return

int run_target(char *smfile, char *target, char *argv0)
{
        off_t size;
        struct stat s;
        char *bfr, *d;
        int c, m;
        FILE *f;
        if(stat(smfile, &s))
        {
                printf("Can't get file size: %s\n", strerror(errno));
                puts("(assuming 256K)");
                size = 256 * 1024;
        }
        else size = s.st_size;
	bfr = malloc(size + strlen(target) + strlen(argv0));
	f = fopen(smfile, "r");
	READ_WHILE(1)
	{
                if(SON(c)) continue; /* skip whitespace */
                /* read the target name */
		d = bfr;
                CWRITED;
		COPY_WHILE(!SON(c) && c != '{' && c != '(');
		*d = '\0';
		if(strcmp(bfr, target)) /* target != this one */
                {
                        /* skip the whole code block */
                        READ_WHILE(c != '}');
                        continue;
                }
                if(c == '{') goto execute;
                if(c == '(') goto deps;
                READ_WHILE(c != '{' && c != '(')
                {
                        if(SON(c)) continue;
                        d = bfr;
                        CWRITED;
                        COPY_WHILE(!SON(c) && c != '{');
                        *d = '\0';
                        OPTIONSTART;
                        OPTION("root") { if(getuid() != 0)
                        {
                                sprintf(bfr, "sudo %s %s", argv0, target);
                                fclose(f);
                                c = system(bfr);
                                free(bfr);
                                return c;
                        } }
                        OPTIONEND;
                }
                if(c == '{') goto execute;
deps: /* read all the dependencies and run them */
                while(c != ')')
                {
                        SKIP_WHILE(SON(c) || c == ',' || c == '(');
                        if(c == ')') break;
                        d = bfr;
                        CWRITED;
                        COPY_WHILE(!SON(c) && c != ',' && c != ')');
                        *d = '\0';
                        m = run_target(smfile, bfr, argv0);
                        if(m) TRETURN m;
                }
                SKIP_WHILE(c != '{');
execute: /* execute the code */
                READ_WHILE(c != '}')
                {
                        SKIP_WHILE(SON(c));
                        if(c == '}') break;
                        m = c;
                        d = bfr;
                        COPY_WHILE(!NEWLINE(c));
                        *d = '\0';
                        run_command(m, bfr);
                }
                TRETURN 0;
        }
        printf("Target \"%s\" not found.\n", target);
        TRETURN 1;
}
