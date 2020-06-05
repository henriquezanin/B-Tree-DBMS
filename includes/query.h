#ifndef __QUERY__
#define __QUERY__

#include <parser.h>

#define QUERY_SEPARATOR ' '

enum operationType{insert, search, rrn, load, table, exitCommand, showHelp, man};

Errors insertQuery(Metadata *, char *);
Errors selectQuery(Metadata *, char *);
Errors evalQuery(Metadata**, char*);

#endif