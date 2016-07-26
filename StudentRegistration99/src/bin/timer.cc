/*
 * Copyright (C) 1994, 1995, 1996
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RecordInspector.cc,v 1.2 1996/11/25 15:39:47 snowman Exp $
 */

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

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

#include <System/sys/time.h>

int main (int argc, char** argv)
{
    char* uid = 0;

    for (int i = 0; i < argc; i++)
    {
	if (::strcmp(argv[i], "-help") == 0)
	{
	    cout << "Usage: " << argv[0] << " -uid <uid> [-help]" << endl;
	    return 0;
	}
	if (::strcmp(argv[i], "-uid") == 0)
	    uid = argv[i+1];
    }

    if (!uid)
    {
	cout << "Error - no uid given." << endl;
	return -1;
    }

    Uid u(uid);
    SOutCome res = UNKNOWN;

    Buffer buff;

    AtomicAction A;

    A.Begin();

    struct timeval stime, ftime;

    gettimeofday(&stime, NULL);

    for (int ii = 0; ii < 126; ii++)
      {
    StudentRecord stud(res, u);
    
    if (res != DONE)
    {
	cout << "Error in constructor." << endl;
	return -1;
    }

    res = UNKNOWN;

    stud.fetchRecord(res, buff);

    if (res == DONE)
    {
	void* ptr = 0;
	size_t length = 0;

	if (buff.unpack(ptr, length))
	{
	    char* data = new char[length+1];
	    ::memset(data, '\0', length+1);
	    ::memcpy(data, ptr, length);
	    
	    //	    cout << data << endl;
	    ::delete [] data;
	}
	else
	    cout << "Unpack error." << endl;

	if (ptr)
	    ::delete [] ptr;
    }
      }

    if (A.End() != COMMITTED)
      cout << "Commit error." << endl;

    gettimeofday(&ftime, NULL);

    cout << "Time: " << (ftime.tv_sec*1000000 + ftime.tv_usec) -
      (stime.tv_sec*1000000 + stime.tv_usec) << endl;

    return 0;
}

    
    
