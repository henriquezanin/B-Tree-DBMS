#ifndef __QUERY__
#define __QUERY__

#include <parser.h>

#define QUERY_SEPARATOR ' '

Errors insertQuery(Metadata *, char *);
Errors evalQuery(Metadata*, char*);

#endif