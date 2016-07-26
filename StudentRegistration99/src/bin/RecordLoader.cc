/*
 * Copyright (C) 1994, 1995, 1996-1998,
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RecordLoader.cc,v 1.2 1996/09/06 14:52:22 arjuna Exp $
 */

#include <System/fcntl.h>
#include <System/ndbm.h>
#include <System/strstream.h>
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
#include <Requests.h>

int main (int, char**)
{
    Uid* id = NULL;
    StudentRecord* sr = NULL;
    SOutCome result = UNKNOWN;
    Boolean finished = FALSE;
    Buffer *record = NULL;
    DBM *db;
    datum sno, srec;
    char stuno[STUDENTNUMBERSIZE+1], sturec[STUDATASIZE+1];
    char wholeLine[RECSIZE+2];
    char uidBuff[UidSize];
    int index = 0;

    if ((db = dbm_open ((char*) staticKeyMap, O_RDWR | O_CREAT, 0777)) == NULL)
    {
	cerr << "Unable to open key map database" << endl;
	return 1;
    }

    for (;;)
    {
	if (cin.peek() == EOF)
	{
	    cerr << "End of data reached" << endl;
	    break;
	}

	cin.getline(wholeLine, RECSIZE+2);

	::memcpy(stuno, wholeLine, STUDENTNUMBERSIZE);
	stuno[STUDENTNUMBERSIZE] = '\0';

	if (::strlen(stuno) != STUDENTNUMBERSIZE)
	{
	    cerr << "Failed to read student number." << endl;
	    dbm_close(db);
	    return 1;
	}

	::memset(uidBuff, '\0', UidSize);
	sr = new StudentRecord(result);
	if (result == DONE)
	{
	    ostrstream x(uidBuff, UidSize);
	    x << sr->get_uid ();
	}
	else
	{
	    cerr << "Error " << result << " occurred while creating student record." << endl;
	    if (sr)
		delete sr;
	    dbm_close(db);	    
	    return -1;
	}
    
	if (record)
	    delete record;

	record = new Buffer;
	
	result = UNKNOWN;
	
	AtomicAction A;
		
	stuno[STUDENTNUMBERSIZE] = '\0';

	sno.dsize = STUDENTNUMBERSIZE;
	sno.dptr = stuno;
	
	::memcpy(sturec, wholeLine+STUDENTNUMBERSIZE, STUDATASIZE);
	sturec[STUDATASIZE] = '\0';

	if (::strlen(sturec) != STUDATASIZE)
	{
	    cerr << "Failed to read student record" << endl;
	    dbm_close(db);
	    delete sr;
	    delete record;
	    return 1;
	}

	record->packNonMappedBlock(wholeLine, RECSIZE);
	srec.dptr = uidBuff;
	srec.dsize = ::strlen(uidBuff);
	A.Begin();

	sr->storeRecord(result, *record);

	if (result == DONE)
	{
	    if (A.End() == COMMITTED)
	    {
		index++;
		cout << sno.dptr << " " << srec.dptr << endl;
		if (dbm_store (db, sno, srec, DBM_INSERT) < 0)
		{
		    cerr << "Unable to record key in map" << endl;
		    cerr << sno.dptr << " " << srec.dptr << endl;
		}
	    }
	    else
		cerr << "An error occurred while committing action" << endl;
	}
	else
	{
	    A.Abort();
	    cerr << "An error occurred while inserting record" << endl;
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

    dbm_close(db);

    cerr << index << " records inserted." << endl;
    
    return 0;
}
