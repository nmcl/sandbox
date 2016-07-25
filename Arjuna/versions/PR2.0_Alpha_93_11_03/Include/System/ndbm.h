/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ndbm.h,v 1.1 1993/11/03 14:32:52 nmcl Exp $
 */

#ifndef NDBM_H_
#define NDBM_H_

#ifdef STUB
#  pragma @NoRename
#endif

#ifdef __GNUG__

#define DBM_INSERT      0
#define DBM_REPLACE     1

struct DBM;

typedef struct
{
    char *dptr;
    int  dsize;
} datum;

extern "C"
{
DBM   *dbm_open(const char*, int, int);
long  dbm_forder(DBM*, datum);
int   dbm_store(DBM*, datum, datum, int);
int   dbm_delete(DBM*, datum);
int   dbm_close(DBM*);
datum dbm_firstkey(DBM*);
datum dbm_nextkey(DBM*);
datum dbm_fetch(DBM*, datum);
}
#else
#include <ndbm.h>
#endif

#endif
