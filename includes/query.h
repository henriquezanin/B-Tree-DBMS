/*
Henrique Gomes Zanin NUSP: 10441321
Gabriel Guimaraes Vilas Boas Marin NUSP: 11218521
*/
#ifndef __QUERY__
#define __QUERY__

#include <parser.h>

#define QUERY_SEPARATOR ' '

enum operationType{insert, search, rrn, load, table, exitCommand, showHelp, man};

Errors insertQuery(Metadata *, char *);
Errors selectQuery(Metadata *, char *, BOOL);
Errors evalQuery(Metadata**, char*);

#endif