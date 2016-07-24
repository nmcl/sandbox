/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: StateDaemon.cc,v
 */

#define STATEMANAGER_SERVER
#define LOCKMANAGER_SERVER
#define STATEDAEMON_SERVER

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef STRSTREAM_H_
#  include <System/strstream.h>
#endif

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef SYS_STAT_H_
#  include <System/sys/stat.h>
#endif

#ifndef ERRNO_H_
#  include <System/errno.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef CTYPE_H_
#  include <System/ctype.h>
#endif

#ifndef DIRENT_H_
#  include <System/dirent.h>
#endif

#ifndef FCNTL_H_
#  include <System/fcntl.h>
#endif

#ifndef SYS_FILE_H_
#  include <System/sys/file.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef OBJSTATE_H_
#  include <Arjuna/ObjState.h>
#endif

#ifndef OBJSTORE_H_
#  include <ObjectStore/ObjStore.h>
#endif

#ifndef LVOBJSTORE_H_
#  include <ObjectStore/LVObjStore.h>
#endif

#ifndef LPOBJSTORE_H_
#  include <ObjectStore/LPObjStore.h>
#endif

#ifndef CONFIGURE_H_
#  include <Configure.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#include "StateDaemon_stub.h"

#ifndef DEBUG_H_
#  include <Common/Debug.h>
#endif


ObjectStore* StateDaemon::OS = 0;



StateDaemon::StateDaemon (int& res, Boolean isVolatile)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "StateDaemon::StateDaemon\n" << flush;
#endif

    if (!StateDaemon::OS)
    {
	if (isVolatile)
	    StateDaemon::OS = ObjectStore::create("LocalVolatileObjectStore");
	else
	    StateDaemon::OS = ObjectStore::create("LocalPersistentObjectStore");
    }

    res = 0;
}

StateDaemon::~StateDaemon ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "StateDaemon::~StateDaemon\n" << flush;
#endif

    if (StateDaemon::OS)
    {
        ObjectStore::destroy(StateDaemon::OS);
	StateDaemon::OS = 0;
    }
}

/*
 * Public non-virtual functions
 */

void StateDaemon::areYouAlive (Boolean& result) const
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "void StateDaemon::areYouAlive (Boolean& result) const" << endl;
#endif

    result = TRUE;
}

/*
 * Given a type name return an objectstate that contains all of the
 * uids of objects of that type
 */

Buffer StateDaemon::allObjUids (const TypeName tName, int& res) const
{
    Buffer buff;
    ObjectState *objstate = 0;

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "StateDaemon::allObjUids (" << tName << ")\n" << flush;
#endif

    res = 0;
    objstate = StateDaemon::OS->allObjUids(tName);

    if (objstate)
    {
	objstate->packInto(buff);
	delete objstate;
    }
    else
	res = -1;
    
    return buff;
}

/*
 * The following operation commits a previous write_state operation which
 * was made with the SHADOW FileType argument. This is achieved by
 * renaming the shadow and removing the hidden version.
 */

Boolean StateDaemon::commitState (const Uid& objUid, const TypeName tName, int& res) const
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "StateDaemon::commitState (" << objUid << ", " << tName << ")\n" << flush;
#endif

    res = 0;
    Boolean result = StateDaemon::OS->commit_state(objUid, tName);
    
    return result;
}

Boolean StateDaemon::removeState (const Uid& u, const TypeName name,
				  int& res) const
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "Boolean StateDaemon::removeState\n" << flush;
#endif

    res = 0;

    return StateDaemon::OS->remove_state(u, name);
}

Boolean StateDaemon::removeUnCommitted (const Uid& u, const TypeName name, int& res) const
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "Boolean StateDaemon::removeUnCommitted\n" << flush;
#endif

    res = 0;

    return StateDaemon::OS->remove_uncommitted(u, name);
}

Buffer StateDaemon::readState (const Uid& objUid, const TypeName tName, int& res) const
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "StateDaemon::readState\n" << flush;
#endif

    Buffer buff;
    ObjectState* new_image = 0;

    res = 0;
    new_image = StateDaemon::OS->read_state(objUid, tName);

    if (new_image)
    {
	new_image->packInto(buff);
	delete new_image;
    }
    else
	res = -1;

    return buff;
}

Buffer StateDaemon::readUnCommitted (const Uid& objUid, const TypeName tName, int& res) const
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "Buffer StateDaemon::readUnCommitted\n" << flush;
#endif

    Buffer buff;
    ObjectState* new_image = 0;

    res = 0;
    new_image = StateDaemon::OS->read_uncommitted(objUid, tName);

    if (new_image)
    {
	new_image->packInto(buff);
	delete new_image;
    }
    else
	res = -1;

    return buff;
}

/*
 * write_state saves the ObjectState in a file named by the type and Uid
 * of the ObjectState. If the second argument is SHADOW, then the file name
 * is different so that a subsequent commit_state invocation will rename
 * the file. If writing a shadow the original file is hidden.
 */

Boolean StateDaemon::writeState (const Uid& objUid, const TypeName tName,
				 Buffer state, int& res) const
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "StateDaemon::writeState\n" << flush;
#endif

    res = 0;

    ObjectState i;
    i.unpackFrom(state);

    return StateDaemon::OS->write_state(objUid, tName, i);
}

Boolean StateDaemon::writeCommitted (const Uid& objUid, const TypeName tName,
				     Buffer state, int& res) const
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "Boolean StateDaemon::writeCommitted\n" << flush;
#endif

    res = 0;
    ObjectState i;
    i.unpackFrom(state);

    return StateDaemon::OS->write_committed(objUid, tName, i);
}
