/*
 * Copyright (C) 1994, 1995, 1996
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: MapSetup.cc,v 1.1 1996/08/01 09:14:45 arjuna Exp $
 */

#include <System/ndbm.h>
#include <System/fstream.h>
#include <System/sys/file.h>
#include <System/iostream.h>
#include <System/string.h>
#include <System/sys/types.h>
#include <System/sys/stat.h>
#include <System/fcntl.h>
#include <System/string.h>

#include <arjuna.h>

int main (int argc, char** argv)
{
    char* inputFileName = (char*) 0;

    for (int i = 0; i < argc; i++)
    {
	if (::strcmp(argv[i], "-help") == 0)
	{
	    cout << "Usage: " << argv[0] << " -file <data file> [-help]" << endl;
	    return 0;
	}
	if (::strcmp(argv[i], "-file") == 0)
	    inputFileName = argv[i+1];
    }

    if (!inputFileName)
    {
	cout << "Error - no data file given." << endl;
	return -1;
    }
    
    DBM* dbFile = dbm_open(staticKeyMap, O_RDWR | O_CREAT, 0666);

    if (!dbFile)
    {
	cout << "Could not create/open " << staticKeyMap << " file." << endl;
	return 1;
    }

    ifstream input(inputFileName);

    if (!input)
    {
	cout << "Could not open file " << inputFileName << endl;
	dbm_close(dbFile);
	return 1;
    }

    while (input.peek() != EOF)
    {
	datum key, record;
	char number[20], uid[128];

	::memset(number, '\0', 20);
	::memset(uid, '\0', 128);

	input >> number >> uid;

	if ((::strlen(number) > 0) && (::strlen(uid) > 0))
	{
	    key.dptr = number;
	    key.dsize = ::strlen(number);

	    record.dptr = uid;
	    record.dsize = ::strlen(uid)+1;

	    if (dbm_store(dbFile, key, record, DBM_REPLACE) != 0)
		cout << "An error occured while inserting." << endl;
	    else
		cout << "Record inserted successfully." << endl;
	}
    }
    
    dbm_close(dbFile);

    return 0;
}
