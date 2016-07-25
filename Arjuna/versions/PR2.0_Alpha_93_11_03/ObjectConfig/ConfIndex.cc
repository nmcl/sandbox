/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ConfIndex.cc,v 1.1 1993/11/03 14:35:16 nmcl Exp $
 */

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef CONFINDEX_H_
#  include <ObjectConfig/ConfIndex.h>
#endif

#ifndef LOCK_H_
#  include <Arjuna/Lock.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef MANAGER_H_
#  include <ObjectConfig/Manager.h>
#endif

#ifndef MULTIMAN_H_
#  include <ObjectConfig/MultiMan.h>
#endif

#ifdef DEBUG
#  ifndef DEBUG_H_
#      include <Common/Debug.h>
#  endif
#endif

ConfigIndex *ConfigIndex::listHead = NULL;

ConfigIndex *ConfigIndex::Create()
{
    ConfigIndex *tempPtr = new ConfigIndex;

    tempPtr->nextElem = listHead;
    listHead          = tempPtr;

    return tempPtr;
}

ConfigIndex *ConfigIndex::Create(const Uid& uid)
{
    Boolean done = FALSE;

    ConfigIndex *tempPtr = listHead;
    while ((! done) && (tempPtr != NULL))
    {
        if (uid == tempPtr->get_uid())
            done = TRUE;
        else
            tempPtr = tempPtr->nextElem;
    }

    if (tempPtr == NULL)
    {
        tempPtr           = new ConfigIndex(uid);
        tempPtr->nextElem = listHead;
        listHead          = tempPtr;
    }

    return tempPtr;
}

void ConfigIndex::Destroy()
{
    ConfigIndex *tempPtr = listHead;

    while (tempPtr != NULL)
    {
        tempPtr = listHead->nextElem;
        delete listHead;
        listHead = tempPtr;
    }
}

Boolean ConfigIndex::add(const Uid& objUid, const MultiManager *multiMan)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "ConfigIndex::add(const Uid& objUid, const MultiManager *multiMan)\n" << flush;
#endif

    Boolean res = FALSE;
    AtomicAction A;

    A.Begin();

    if ((multiMan != NULL) && (setlock(new Lock(WRITE), 0) == GRANTED))
    {
	if (objUidList[objectNumber] == NULL)
	    objUidList[objectNumber]  = new Uid(objUid);
        else
            *objUidList[objectNumber] = objUid;

        if (mmUidList[objectNumber] == NULL)
	    mmUidList[objectNumber]  = new Uid(multiMan->get_uid());
        else
            *mmUidList[objectNumber] = multiMan->get_uid();

	objectNumber++;

	res = TRUE;
    }
    
    if (res)
    {
	if (A.End() != COMMITTED)
	    res = FALSE;
    }
    else
	A.Abort();

    return res;
}

Boolean ConfigIndex::remove(const Uid& objUid)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "ConfigIndex::remove(const Uid& objUid)\n" << flush;
#endif

    Boolean res   = FALSE;

    AtomicAction A;

    A.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
        int i;

	for (i = 0; (i < objectNumber) && (! (*objUidList[i] == objUid)); i++);

        if (i < objectNumber)
        {
            mmUidList[i]  = NULL;
            objUidList[i] = NULL;

            res = TRUE;
        }
    }

    if (res)
    {
	if (A.End() != COMMITTED)
	    res = FALSE;
    }
    else
	A.Abort();

    return res;
}

Boolean ConfigIndex::reconfig(Manager* man, MultiManager* dstMultiMan)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "ConfigIndex::reconfig(Manager *man, MultiManager *dstMultiMan)\n" << flush;
#endif

    Boolean res = FALSE;

    AtomicAction A;

    A.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
        Uid          objUid(NIL_UID);	
        MultiManager *srcMultiMan;
	
        res = FALSE;
        objUid = man->get_uid();
        
        if (find(objUid, srcMultiMan))
        {
            int i;

	    for (i = 0; (i < objectNumber) && (! (*objUidList[i] == objUid)); i++);
	
            if (*objUidList[i] == objUid)
            {
                Manager     *manager     = NULL;
                ObjectState *objectState = NULL;

                if (mmUidList[i] == NULL)
                    mmUidList[i] = new Uid(dstMultiMan->get_uid());
                else
                    *mmUidList[i] = dstMultiMan->get_uid();

                res = srcMultiMan->do_remove(man, manager, objectState);
                if (res)
                    res = dstMultiMan->do_add(objUid, manager, objectState);
            }
        }
    }
    
    if (res)
    {
	if (A.End() != COMMITTED)
	    res = FALSE;
    }
    else
	A.Abort();

    return res;
}

Boolean ConfigIndex::find(const Uid& uid, MultiManager*& multiMan)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "ConfigIndex::find(const Uid& Uid, MultiManager*& multiMan)\n" << flush;
#endif

    int     index = -1;
    Boolean res   = FALSE;

    AtomicAction A;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
        int i;

	for (i = 0; (i < objectNumber) && (! (*objUidList[i] == uid)); i++);
	
        if (*objUidList[i] == uid)
        {
            multiMan = MultiManager::Create(*mmUidList[i]);
            res = (multiMan != NULL);
        }
        else
           res = FALSE;
    }

    if (res)
    {
	if (A.End() != COMMITTED)
	    res = FALSE;
    }
    else
	A.Abort();

    return res;
}

Boolean ConfigIndex::save_state (ObjectState& os, ObjectType)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "ConfigIndex::save_state(ObjectState& os, ObjectType)\n" << flush;
#endif

    Boolean res;

    res = os.pack(objectNumber);

    for (int index = 0; res && (index < objectNumber); index++)
	res = objUidList[index]->pack(os) && mmUidList[index]->pack(os);

    return res;
}

Boolean ConfigIndex::restore_state (ObjectState& os, ObjectType)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "ConfigIndex::restore_state(ObjectState& os, ObjectType)\n";
#endif

    Boolean res;

    res = os.unpack(objectNumber);

    for (int index = 0; res && (index < objectNumber); index++)
    {
	if (objUidList[index] == NULL)
	    objUidList[index] = new Uid(NIL_UID);

	if (mmUidList[index] == NULL)
	    mmUidList[index] = new Uid(NIL_UID);

	res = objUidList[index]->unpack(os) && mmUidList[index]->unpack(os);
    }

    return res;
}

const TypeName ConfigIndex::type () const
{
    return "/StateManager/LockManager/ConfigIndex";
}

ConfigIndex::ConfigIndex () : LockManager(ANDPERSISTENT)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "ConfigIndex::ConfigIndex ()\n" << flush;
#endif

    AtomicAction A;

    A.Begin();

    objectNumber  = 0;
    for (int i = 0; i < cIListSize; i++)
    {
	objUidList[i] = NULL;
	mmUidList[i] = NULL;
    }

    if (setlock(new Lock(WRITE), 0) == GRANTED)
	A.End();
    else
	A.Abort();
}

ConfigIndex::ConfigIndex (const Uid& uid): LockManager(uid)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "ConfigIndex::ConfigIndex (const Uid& uid)\n" << flush;
#endif

    objectNumber  = 0;
    for (int i = 0; i < cIListSize; i++)
    {
	objUidList[i] = NULL;
	mmUidList[i] = NULL;
    }
}

ConfigIndex::~ConfigIndex ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "ConfigIndex::~ConfigIndex ()\n" << flush;
#endif
    
    StateManager::terminate();
    
    for (int i = 0; i < cIListSize; i++)
    {
	if (objUidList[i] != NULL)
	    delete objUidList[i];
	
	if (mmUidList[i] != NULL)
	    delete mmUidList[i];
    }
}

ostream& ConfigIndex::PrintOn (ostream& str) const
{
    for (int index = 0; index < cIListSize; index++)
    {
	if ((objUidList[index] != NULL) || (mmUidList[index] != NULL))
        {
	    if (objUidList[index] == NULL)
	        str << "NULL ";
	    else
	        str << *objUidList[index] << " ";
	    if (mmUidList[index] == NULL)
	        str << "NULL" << endl;
	    else
	        str << *mmUidList[index] << endl;
        }
    }

    str << flush;

    return str;
}
