/*
 * Copyright (C) 1994
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DBList.cc,v 1.1 1996/08/01 09:14:39 arjuna Exp $
 */

#include <System/iostream.h>
#include <System/strstream.h>
#include <System/fcntl.h>
#include <System/ndbm.h>
#include <System/string.h>

DBM* db = 0;


void getData ()
{
    datum sno, srec;
    
    for (sno = dbm_firstkey(db); sno.dptr != NULL; sno = dbm_nextkey(db))
    {
  	srec = dbm_fetch(db, sno);

	char* key = ::new char[sno.dsize+1];
	::memset(key, '\0', sno.dsize+1);
	::memcpy(key, sno.dptr, sno.dsize);

	char* record = ::new char[srec.dsize+1];
	::memset(record, '\0', srec.dsize+1);
	::memcpy(record, srec.dptr, srec.dsize);

	cout << "Key : " << key << " Record : " << record << endl;

	::delete [] key;
	::delete [] record;
    }
}
    
void usage (char* name)
{
    cout << "Usage: " << name << " -file <file> [-help]" << endl;    
}


int main (int argc, char** argv)
{
    char* fileName = 0;

    for (int i = 1; i < argc; i++)
    {
	if (::strcmp(argv[i], "-help") == 0)
	{
	    usage(argv[0]);
	    return 0;
	}
	else
	{
	    if (::strcmp(argv[i], "-file") == 0)
	    {
		fileName = argv[i+1];
		i = i+1;
	    }
	    else
	    {
		cerr << "Unknown option " << argv[i] << endl;
		usage(argv[0]);
		return -1;
	    }
	}
    }

    if (!fileName)
    {
	cout << "Error - no file name given." << endl;
	usage(argv[0]);
	return -1;
    }
    
    if ((db = dbm_open(fileName, O_RDONLY, 0)) == NULL)
    {
	cerr << "Unable to open key map database " << fileName << endl;
	return -1;
    }

    getData();
    dbm_close(db);
}
