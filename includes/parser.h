#ifndef __PARSER__
#define __PARSER__

#include <stdio.h>
#include <utils.h>

/* Separator of metadata parameter */
#define METADATA_PARAMETER_SEP ':'
#define FIELD_DELIMITER ','
#define STRING_DELIMITER '\"'
#define SPACE ' '
#define COMMENT "//"

enum ParameterType{Name,Type};


Metadata *parseMetadata(char *);
char *extractParameter(char *, char);
char *extractParameterValue(char *, char);
Data **parseInsertQueryData(Metadata *, char *);
char *extractKeyFromInsertQuery(char *);
#endif