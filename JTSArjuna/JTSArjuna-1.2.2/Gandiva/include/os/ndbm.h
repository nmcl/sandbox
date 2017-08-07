/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ndbm.h,v 1.3 1996/01/02 13:13:22 nsmw Exp $
 */

#ifndef OS_NDBM_H_
#define OS_NDBM_H_

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
