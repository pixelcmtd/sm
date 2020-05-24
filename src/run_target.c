#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <linux/limits.h>
#include "run_command.h"
#include "system.h"

#define NEWLINE(c) ((c) == '\r' || (c) == '\n')
#define SON(c) ((c) == ' ' || (c) == '\t' || NEWLINE(c))
#define READ_WHILE(b) \
	for(c = fgetc(f); (b) && c != EOF; c = fgetc(f))

#define OPTIONSTART if(0)
#define OPTION(s) else if(!strcmp(s, bfr))
#define OPTIONEND else printf("Omitting unknown option \"%s\".\n", bfr);

void run_target(char *smfile, char *target, char *argv0)
{
        off_t size;
        struct stat s;
        if(stat(smfile, &s))
        {
                printf("Can't get file size: %s\n", strerror(errno));
                size = 256 * 1024;
        }
        else size = s.st_size;
	char bfr[size + strlen(target) + strlen(argv0)];
	int c;
	FILE *f = fopen(smfile, "r");
	READ_WHILE(1)
	{
                if(SON(c)) continue;
		char *d = bfr;
                *d++ = c;
                // read the target name
		READ_WHILE(!SON(c) && c != '{' && c != '(') *d++ = c;
		*d = '\0';
		if(strcmp(bfr, target)) // target != this one
                {
                        // skip all of this
                        READ_WHILE(c != '}');
                        continue;
                }
                if(c == '{') goto execute;
                if(c == '(') goto deps;
                READ_WHILE(c != '{' && c != '(')
                {
                        if(SON(c)) continue;
                        d = bfr;
                        *d++ = c;
                        READ_WHILE(!SON(c) && c != '{') *d++ = c;
                        *d = '\0';
                        OPTIONSTART;
                        OPTION("root") { if(getuid() != 0)
                        {
                                sprintf(bfr, "sudo %s %s", argv0, target);
                                SYSTEM(bfr);
                                return;
                        } }
                        OPTIONEND;
                }
                if(c == '{') goto execute;
deps:
                while(c != ')')
                {
                        if(SON(c) || c == ',') { c = fgetc(f); continue; }
                        d = bfr;
                        READ_WHILE(!SON(c) && c != ',' && c != ')') *d++ = c;
                        *d = '\0';
                        run_target(smfile, bfr, argv0);
                }
                READ_WHILE(c != '{');
execute:
                //execute the code
                READ_WHILE(c != '}')
                {
                        if(SON(c)) continue;
                        int modifier = c;
                        d = bfr;
                        READ_WHILE(!NEWLINE(c)) *d++ = c;
                        *d = '\0';
                        run_command(modifier, bfr);
                }
                fclose(f);
                return;
	}
}
