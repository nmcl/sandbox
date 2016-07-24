/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: PrimaryObject.cc,v 1.10 1995/09/12 12:19:40 nmcl Exp $
 */

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef NETDB_H_
#  include <System/netdb.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef DEBUG_H_
#  include <Common/Debug.h>
#endif

#ifndef PRIMARYOBEJCT_H_
#  include <Replication/PrimaryObject.h>
#endif


PrimaryObject::PrimaryObject (int& res)
			     : AN(0),
			       Z(0),
			       NS(0),
			       NameList(0),
			       key(0),
			       gotPrimary(FALSE),
			       index(-1),
			       number(-1),
			       oldPrimary(-1)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "PrimaryObject::PrimaryObject (int& res)" << endl;
#endif

    int opcode = -1;
    NS = new NSInterface(opcode);

    if (opcode == 0)
	res = 0;
}

PrimaryObject::~PrimaryObject ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "PrimaryObject::~PrimaryObject ()" << endl;
#endif
    
    if (Z)
    {
	if (Z->status() == RUNNING)
	{
	    error_stream << WARNING << "No call to PrimaryObject End or Abort operation\n" << flush;
	    error_stream << WARNING << "Will abort operation\n" << flush;
	    Z->Abort();
	}

	delete Z;
    }

    if (AN)
    {
        for (int i = 0; i < number; i++)
	    delete AN[i];
	delete AN;
    }

    if (NS)
	delete NS;
    fixBuffers();
}

ActionStatus PrimaryObject::Begin ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ActionStatus PrimaryObject::Begin ()" << endl;
#endif    

    if (Z)
    {
	error_stream << WARNING << "Attempt to begin already running PrimaryObject\n" << flush;
	return Z->status();
    }

    Z = new AtomicAction;

    fixBuffers();
    initialize();
    
    return Z->Begin();
}

ActionStatus PrimaryObject::End ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ActionStatus PrimaryObject::End ()" << endl;
#endif

    ActionStatus result = INVALID;

    if (!Z)
	error_stream << WARNING << "Attempt to end non-running PrimaryObject\n" << flush;
    {
        result = Z->End();

	delete Z;
	Z = (AtomicAction*) 0;
    }

    return result;
}

ActionStatus PrimaryObject::Abort ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ActionStatus PrimaryObject::Abort ()" << endl;
#endif    

    ActionStatus result = INVALID;

    if (!Z)
	error_stream << WARNING << "Attempt to abort non-running PrimaryObject\n" << flush;
    else
    {
        result = Z->Abort();

	delete Z;
	Z = (AtomicAction*) 0;
    }

    return result;
}

void PrimaryObject::initialize ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PRIVATE;    
    debug_stream << "void PrimaryObject::initialize ()" << endl;
#endif

    index = -1;
    gotPrimary = FALSE;
    AN = (ArjunaName**) 0;
    number = oldPrimary = 0;
}

void PrimaryObject::fixBuffers ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PRIVATE;    
    debug_stream << "void PrimaryObject::fixBuffers ()" << endl;
#endif

    if (NameList)
    {
	delete NameList;
	NameList = (Buffer*) 0;
    }

    if (key)
    {
	delete key;
	key = (Buffer*) 0;
    }
}

/*
 * We do not write lock the db entry when manipulating it locally to the
 * user since we assume failures are rare. If we did then other users would
 * be unable to use the object until this user had activated the primary.
 * This does mean that if a new primary is elected it is possible for a user
 * to find that it is unable to register this at the db. It will therefore
 * be forced to abort and retry.
 */

Boolean PrimaryObject::fetchNameList (const Buffer& key_buff)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean PrimaryObject::fetchNameList (const Buffer& key_buff)" << endl;
#endif

    Boolean done = FALSE;

    fixBuffers();
    initialize();
    
    if (!Z)
    {
	error_stream << WARNING << "Must call PrimaryObject::Start before obtaining NameList\n" << flush;
	return done;
    }
   
    NameList = new Buffer;
    NS->fetchName(done, key_buff, *NameList);
    key = new Buffer(key_buff);
    if (done)
        extract();
    
    return done;
}

Boolean PrimaryObject::fetchNameList (const ArjunaName& scanFor)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean PrimaryObject::fetchNameList (const ArjunaName& scanFor)" << endl;
#endif
    
    Boolean result;

    if (!Z)
    {
	error_stream << WARNING << "Must call PrimaryObject::Start before obtaining NameList\n" << flush;

        return FALSE;
    }

    Buffer key_buff;

    scanFor.pack(key_buff);
    result = fetchNameList(key_buff);
    
    return result;
}

void PrimaryObject::extract ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PRIVATE;
    debug_stream << "void PrimaryObject::extract ()" << endl;
#endif

    ArjunaName* AN1 = new ArjunaName;
    Boolean IsPrimary = FALSE;

    // the first entry is special as it contains the number of group members.

    AN1->unpack(*NameList);
    NameList->unpack(IsPrimary);
    NameList->unpack(number);

    if (IsPrimary)
        oldPrimary = 0;

    AN = new ArjunaName* [number];
    AN[0] = AN1;

    for (int i = 1; i < number; i++)
    {
	AN1 = new ArjunaName;
	AN1->unpack(*NameList);
	NameList->unpack(IsPrimary);

	if (IsPrimary)
	    oldPrimary = i;

	AN[i] = AN1;
    }
}

// this routine searches the buffer for the marked server primary, if any.

ArjunaName* PrimaryObject::findPrimary ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PRIVATE;
    debug_stream << "ArjunaName* PrimaryObject::findPrimary ()" << endl;
#endif    

    if (index == -1)
    {
        if (oldPrimary)
	    index = oldPrimary;
	else
	    index = 0;
    }
    else
    {
        index++;

	if (index == number)
	    index = 0;

	if (index == oldPrimary)     // we have come full circle.
	    return (ArjunaName*) 0;
    }

    return AN[index];
}

/*
 * Note: it is the caller's responsibility to *not* delete the returned ArjunaName.
 */

ArjunaName* PrimaryObject::getPrimary ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ArjunaName* PrimaryObject::getPrimary ()" << endl;
#endif    

    if ((NameList == (Buffer*) 0) || (NameList->length() == 0) || (!Z))
	return (ArjunaName*) 0;

    if (number == 0)
        return (ArjunaName*) 0;

    gotPrimary = TRUE;

    return findPrimary();
}

Boolean PrimaryObject::registerPrimary ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean PrimaryObject::registerPrimary ()" << endl;
#endif
    
    Buffer newbuff;
    Boolean done = FALSE;

    if ((!gotPrimary) || (!Z))
	return FALSE;

    if (oldPrimary == index)  // using already marked primary
	return TRUE;

    for (int i = 0; i < number; i++)
    {
        AN[i]->pack(newbuff);
	if (index == i)
	    newbuff.pack(TRUE);
	else
	    newbuff.pack(FALSE);

	if (i == 0)
	    newbuff.pack(number);
    }

    NS->replaceName(done, *key, newbuff);

    /*
     * At this stage we must have a read lock on the db entry, so our
     * information is still valid even if the register operation failed.
     * As such we can continue to attempt to register this information at
     * the db until we succeed, i.e., there is no need to have to go back
     * to re-obtain the entry information as it cannot have changed.
     */

    return done;
}
