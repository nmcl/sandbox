/*
 * Copyright (C) 1994, 1995, 1996
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Common.h,v 1.3 1996/08/29 12:34:14 arjuna Exp $
 */

#ifndef COMMON_H_
#define COMMON_H_

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

typedef struct
{
    char *name;
    char *val;
} entry;

#define MAX_ENTRIES 10000
#define MAX_FILE_NAME 256
#define lineLength 256

#define outputFile "/tmp/output_%d"
#define errorFile "/tmp/error_%d"

char *makeword(char *line, char stop);
char *fmakeword(FILE *f, char stop, int *len);
char x2c(char *what);
void unescape_url(char *url);
void plustospace(char *str);
void printerror (size_t, const char*);
void printoutput (size_t, const char*);
Boolean checkScript (const char*, const char*);

#endif
