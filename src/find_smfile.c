#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/limits.h>
#include <string.h>
#include <errno.h>

char *append_smfile(char *c)
{
	c += strlen(c);
	*c++ = '/';
	char *d = c;
	*c++ = 'S';
	*c++ = 'm';
	*c++ = 'f';
	*c++ = 'i';
	*c++ = 'l';
	*c++ = 'e';
	return d;
}

void append_dots(char *c)
{
	c += strlen(c);
	*c++ = '/';
	*c++ = '.';
	*c++ = '.';
}

#define return_null { puts(strerror(errno)); free(fn); return NULL; }

// this might not be the best implementation but its the first one i
// got to work
char *find_smfile()
{
	for(;;)
	{
	        char *fn = (char *) malloc(PATH_MAX + 8);
                char nd[PATH_MAX + 4];
		if(!getcwd(fn, PATH_MAX)) return_null;
                if(!fn[1]) { free(fn); return NULL; }
		char *c = append_smfile(fn);
		if(!access(fn, R_OK)) return fn;
		*c = 's';
		if(!access(fn, R_OK)) return fn;
                if(!getcwd(nd, PATH_MAX)) return_null;
                append_dots(nd);
		if(chdir(nd)) return_null;
                free(fn);
	}
}
