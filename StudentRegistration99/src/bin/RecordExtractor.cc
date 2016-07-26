/*
 * Copyright (C) 1994, 1995, 1996
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RecordExtractor.cc,v 1.1 1996/08/01 09:14:48 arjuna Exp $
 */

#include <System/strstream.h>
#include <System/fcntl.h>
#include <System/ndbm.h>
#include <System/iostream.h>
#include <System/string.h>

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#include "LocalStudentRecord.h"

#include <local.h>
#include <arjuna.h>

static DBM *db = NULL;


Boolean getData ()
{
    StudentRecord* sr = NULL;
    Uid* id = NULL;
    datum sno, srec;
    SOutCome result = UNKNOWN;
    Buffer *record = NULL;
    void* wholeLine = NULL;
    char uidBuff[UidSize];
    
    for (sno = dbm_firstkey(db); sno.dptr != NULL; sno = dbm_nextkey(db))
    {
  	srec = dbm_fetch(db, sno);

	::memset(uidBuff, '\0', UidSize);
	::strncpy (uidBuff, srec.dptr, srec.dsize);
	id = new Uid (uidBuff);
	sr = new StudentRecord(result, *id);
	if (result == DONE)
	{
	    ostrstream x(uidBuff, UidSize);
	    x << sr->get_uid ();
	}
    
	if (id)
        {
	    delete id;
            id = NULL;
        }

	if (result != DONE)
	{
	    cerr << "Error " << result << " occurred while creating student record." << endl;
	    if (sr)
            {
		delete sr;
                sr = NULL;
            }
	    dbm_close(db);
	    return FALSE;
	}
    
	if (record)
        {
	    delete record;
            record = NULL;
        }

	record = new Buffer;
	
	result = UNKNOWN;

	sr->fetchRecord (result, *record);
	if (result == DONE)
	{
	    wholeLine = NULL;
	    size_t length = 0;

	    if (record->unpack(wholeLine, length))
	    {
		char* ptr = ::new char[length+1];
		::memset(ptr, '\0', length+1);
		::memcpy(ptr, wholeLine, length);
		cout << ptr << endl;
		::delete [] ptr;
	    }
	    else
		cerr << "**STUDENT RECORD EXTRACT ERROR FOR "
		     << sr->get_uid() << "**" << endl;

	    if (wholeLine)
		delete wholeLine;
	}
	else
	{
	    if (result == EMPTY)
		cerr << "Record " << sr->get_uid() << " is empty." <<endl;
	    else
		cerr << "An error occurred while fetching record for " << sr->get_uid() << endl;
	}

	if (record)
        {
	    delete record;
            record = NULL;
        }

	if (sr)
        {
	    delete sr;
            sr = NULL;
        }
    }

    if (sr)
	delete (sr);

    if (record)
	delete (record);
    
    return TRUE;
}
    
    
int main ()
{
    if ((db = dbm_open(staticKeyMap, O_RDONLY, 0)) == NULL)
    {
	cerr << "Unable to open key map database " << staticKeyMap << endl;
	return -1;
    }

    getData();
    dbm_close(db);

    if ((db = dbm_open(overflowKeyMap, O_RDONLY, 0)) == NULL)
    {
	cerr << "Unable to open key map database " << overflowKeyMap << endl;
	return -1;
    }

    getData();
    dbm_close(db);

    return 0;
}
