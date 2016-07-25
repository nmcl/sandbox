/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: UidConverter.cc,
 */

#include <Configure.h>

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef STDIO_H_
#  include <System/stdio.h>
#endif

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif


#if RPC_SYSTEM_TYPE == RPC_ANSAAction_ || RPC_SYSTEM_TYPE == RPC_ANSA_
int mainbody(int argc, char** argv)
#else
int main(int argc, char** argv)
#endif
{
    Buffer packInto;
    char oldFileName[80], newFileName[80];
    char* type = 0;
    char* dirName = 0;
    Uid* oldFormat = 0;
    long hostAddr = 0, process = 0, sec = 0, other = 0;
    Boolean res = FALSE;

    ::memset(oldFileName, '\0', 80);
    ::memset(newFileName, '\0', 80);
    
    for (int i = 0; i < argc; i++)
    {
	if (::strcmp(argv[i], "-help") == 0)
	{
	    cout << "Usage: UidConverter -TypeName type -oldUid uid" << endl;
	    exit(0);
	}
	if (::strcmp(argv[i], "-oldUid") == 0)
	{
	    res = TRUE;
	    ::strcpy(oldFileName, argv[i+1]);
	}
	if (::strcmp(argv[i], "-TypeName") == 0)
	    type = argv[i+1];
    }
    
    if ((!res) || (type == 0))
    {
	cout << "Usage: UidConverter -TypeName type -oldUid uid" << endl;
	exit(-1);
    }

    dirName = ::new char[::strlen(OBJECTSTOREDIR_Q)+::strlen(type)+2];
    ::memset(dirName, '\0', ::strlen(OBJECTSTOREDIR_Q)+::strlen(type)+2);
    ::strcpy(dirName, OBJECTSTOREDIR_Q);
    ::strcpy(dirName+::strlen(OBJECTSTOREDIR_Q), "/");
    ::strcpy(dirName+::strlen(OBJECTSTOREDIR_Q)+1, type);
    
    if (chdir(dirName) == -1)
    {
	cout << "Error - could not chdir to " << dirName << endl;
	exit(-1);
    }

    oldFormat = new Uid(oldFileName);
    oldFormat->pack(packInto);
    res = packInto.unpack(hostAddr) && packInto.unpack(process)
	  && packInto.unpack(sec) && packInto.unpack(other);
    
    if (!res)
    {
	cout << "Error when unpacking new Uid" << endl;
	exit(-1);
    }

    sprintf(newFileName, "%x:%x:%x:%x", hostAddr, process, sec, other);

#ifdef DEBUG
    cout << "Attempting to rename " << oldFileName << " to " << newFileName << endl;
#endif

    if (rename(oldFileName, newFileName) == 0)
    {
	cout << "Rename successful" << endl;
	return 0;
    }
    else
	cout << "Rename unsuccessful" << endl;

    return -1;
}

    
    
