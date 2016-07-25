/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: NameServ.cc,v 1.11 1993/03/18 15:24:31 nmcl Exp $
 *
 * Primitive Name Server 
 *
 */

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef NDBM_H_
#  include <System/ndbm.h>
#endif

#ifndef SYS_FILES_H_
#  include <System/sys/file.h>
#endif

#ifndef DEBUG_H_
#  ifdef DEBUG
#    include <Common/Debug.h>
#  endif
#endif

#ifndef CONFIGURE_H_
#  include <Configure.h>
#endif

#define NAMESERVER_SERVER

#include "NameServ_stub.h"

#define DBM_PERM 0666

NameServer::NameServer(Boolean& done)
{
#ifdef DEBUG
    _arjuna_debug->set_all(FULL_DEBUGGING, FAC_ALL, VIS_ALL);
    debug_stream << CONSTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "NameServer::NameServer(Boolean& done)\n" << flush;
#endif

    db       = NULL;
    db       = dbm_open(NSDATABASE_Q, O_CREAT | O_RDWR, DBM_PERM);
    scanning = FALSE;

    done     = (db != NULL);
}

NameServer::~NameServer()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "NameServer::~NameServer()\n" << flush;
#endif

    if (db != NULL)
	dbm_close(db);
}

void NameServer::StoreName(Boolean& done, Buffer key_buff, Buffer data_buff)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "NameServer::StoreName(Buffer key_buff, Buffer data_buff)\n" << flush;
#endif

    done = FALSE;

    if (db != NULL)
    {
	datum  key_datum;
	datum  data_datum;


	key_datum.dptr   = key_buff.buffer();
	key_datum.dsize  = (int) key_buff.length();

	data_datum.dptr  = data_buff.buffer();
	data_datum.dsize = (int) data_buff.length();

	done = (dbm_store(db, key_datum, data_datum, DBM_REPLACE) == 0);

    }
    else
	done = FALSE;
}

void NameServer::FetchName(Boolean& done, Buffer key_buff, Buffer& data_buff)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "NameServer::FetchName(Buffer key_buff, Buffer& data_buff)\n" << flush;
#endif

    done = FALSE;

    if (db != NULL)
    {
	datum  key_datum, data_datum;

	key_datum.dptr  = key_buff.buffer();
	key_datum.dsize = (int) key_buff.length();

	data_datum = dbm_fetch(db, key_datum);
	done = (data_datum.dptr != NULL);

	if (done)
	{
            char *tmp_buffer = new char[data_datum.dsize];
            memcpy(tmp_buffer, data_datum.dptr, data_datum.dsize);

            data_buff.setBuffer(data_datum.dsize, tmp_buffer);
	}
    }
    else
	done = FALSE;
}

void NameServer::RemoveName(Boolean& done, Buffer key_buff)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "NameServer::RemoveName(Buffer key_buff)\n" << flush;
#endif

    done = FALSE;

    if (db != NULL)
    {
	datum  key_datum;

	key_datum.dptr  = key_buff.buffer();
	key_datum.dsize = (int) key_buff.length();

	done = (dbm_delete(db, key_datum) == 0);

#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
	debug_stream << "Remove Status : " << done << "\n" << flush;
#endif
    }
    else
	done = FALSE;
}

void NameServer::Restart(Boolean& done)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "NameServer::NameServer::Restart(Boolean& done)\n" << flush;
#endif

    scanning = FALSE;
    done     = TRUE;
}

void NameServer::GetNextName(Boolean& done, Buffer& key, Boolean& endofdata)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "NameServer::GetNextName(Boolean& done, Buffer& key, Boolean& endofdata)\n" << flush;
#endif

    done = FALSE;
    endofdata = FALSE;

    if (db != NULL)
    {
        datum  key_datum;

        if (scanning)
            key_datum = dbm_nextkey(db);
        else
            key_datum = dbm_firstkey(db);

        endofdata = (key_datum.dptr == NULL);

        if (! endofdata)
        {
            char *tmp_buffer = new char[key_datum.dsize];
            memcpy(tmp_buffer, key_datum.dptr, key_datum.dsize);

            key.setBuffer(key_datum.dsize, tmp_buffer);
            done     = TRUE;
            scanning = TRUE;
        }
        else
        {
            done     = TRUE;
            scanning = FALSE;
        }
    }
    else
        done = FALSE;
}
