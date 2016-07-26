/*
 * Copyright (C) 1994
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DBEdit.cc,v 1.2 1996/09/02 13:25:01 arjuna Exp $
 */

#include <System/iostream.h>
#include <System/strstream.h>
#include <System/fcntl.h>
#include <System/ndbm.h>
#include <System/string.h>

DBM* db = 0;


void removeData (char* number)
{
    datum sno;

    sno.dptr = number;
    sno.dsize = strlen(number);

    if (dbm_delete(db, sno) < 0)
	cout << "Delete error." << endl;
    else
	cout << "Deleted successfully." << endl;
}
    

void usage (char* name)
{
    cout << "Usage: " << name << " -file <file> -number <student id> [-help]" << endl;
}

int main (int argc, char** argv)
{
    char* fileName = 0;
    char* studNumb = 0;

    for (int i = 1; i < argc; i++)
    {
	if (::strcmp(argv[i], "-help") == 0)
	{
	    usage(argv[0]);
	    return 0;
	}
	else
	{
	    if (::strcmp(argv[i], "-number") == 0)
	    {
		studNumb = ::new char[::strlen(argv[i+1])+1];
		::strcpy(studNumb, argv[i+1]);
		i = i+1;
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
    }

    if (!fileName)
    {
	cout << "Error - no file name given." << endl;
	usage(argv[0]);
	return -1;
    }

    if (!studNumb)
    {
	cout << "Error - no student number given." << endl;
	usage(argv[0]);
	return -1;
    }
    
    if ((db = dbm_open(fileName, O_RDWR, 0)) == NULL)
    {
	cerr << "Unable to open key map database " << fileName << endl;
	return -1;
    }

    removeData(studNumb);
    
    dbm_close(db);
}
