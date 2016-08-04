/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: UidConverter.cc,v 1.5 1994/08/10 15:12:23 nmcl Exp $
 */

#include <Config/Configure.h>

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

#ifndef LOCALOSTYPES_H_
#  include <ObjectStore/LocalOSTypes.h>
#endif

#ifndef OBJSTORE_H_
#  include <ObjectStore/ObjStore.h>
#endif


const TypeName defaultStore = FragmentedStoreType;


#if RPC_SYSTEM_TYPE == RPC_ANSAAction_ || RPC_SYSTEM_TYPE == RPC_ANSA_
int mainbody (int argc, char** argv)
#else
int main (int argc, char** argv)
#endif
{
    Buffer packInto;
    char oldFileName[80], newFileName[80];
    char* type = 0;
    char* dirName = 0;
    Uid* oldFormat = 0;
    long hostAddr = 0, process = 0, sec = 0, other = 0;
    Boolean res = FALSE;
    char* storeRoot = "";
    TypeName storeName = defaultStore;
    ObjectStore* toUse = (ObjectStore*) 0;

    ::memset(oldFileName, '\0', 80);
    ::memset(newFileName, '\0', 80);
    
    for (int i = 0; i < argc; i++)
    {
	if (::strcmp(argv[i], "-help") == 0)
	{
	    cout << "Usage: UidConverter -TypeName type -oldUid uid [-storepath <store path>] [-storename <store type>]" << endl;
	    exit(0);
	}
	if (::strcmp(argv[i], "-oldUid") == 0)
	{
	    res = TRUE;
	    ::strcpy(oldFileName, argv[i+1]);
	}
	if (::strcmp(argv[i], "-TypeName") == 0)
	    type = argv[i+1];
	if (::strcmp(argv[i], "-storepath") == 0)
	    storeRoot = argv[i+1];
	if (::strcmp(argv[i], "-storename") == 0)
	    storeName = argv[i+1];
    }
    
    if ((!res) || (type == 0))
    {
	cout << "Usage: UidConverter -TypeName type -oldUid uid [-store <store path>] [-storename <store type>]" << endl;
	exit(-1);
    }

    toUse = ObjectStore::create(storeName, storeRoot);
    char* tmpName = toUse->locateStore(storeRoot);
    dirName = ::new char[::strlen(tmpName)+::strlen(type)+1];
    ::memset(dirName, '\0', ::strlen(tmpName)+::strlen(type)+1);
    ::memcpy(dirName, tmpName, ::strlen(tmpName));
    ::memcpy(dirName+::strlen(dirName), type, ::strlen(type));
    ObjectStore::destroy(toUse);

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

    sprintf(newFileName, "%lx:%lx:%lx:%lx", hostAddr, process, sec, other);

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

    
    
