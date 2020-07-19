#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "getcwd.h"

char *append_smfile(char *c)
{
        char *d;
	c += strlen(c);
	*c++ = '/';
	d = c;
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

/*
 * This might not be the best implementation,
 * but it's the first one I got to work.
 */
char *find_smfile()
{
	for(;;)
	{
	        char *fn, *c, nd[PATH_MAX + 4];
                fn = (char *) malloc(PATH_MAX + 8);
                memset(fn, 0, PATH_MAX + 8);
		if(!getcwd(fn, PATH_MAX)) return_null;
                if(!fn[1]) { free(fn); return NULL; }
		c = append_smfile(fn);
		if(!access(fn, R_OK)) return fn;
		*c = 's';
		if(!access(fn, R_OK)) return fn;
                if(!getcwd(nd, PATH_MAX)) return_null;
                append_dots(nd);
		if(chdir(nd)) return_null;
                free(fn);
	}
}
