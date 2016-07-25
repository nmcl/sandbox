/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: MultiMan.cc,v 1.1 1993/11/03 14:35:20 nmcl Exp $
 */

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef MULTIMAN_H_
#  include <ObjectConfig/MultiMan.h>
#endif

#ifndef MANAGER_H_
#  include <ObjectConfig/Manager.h>
#endif

#ifndef LOCK_H_
#  include <Arjuna/Lock.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifdef DEBUG
#  include <Common/Debug.h>
#endif

MultiManager *MultiManager::listHead = NULL;

MultiManager *MultiManager::Create()
{
    MultiManager *tempPtr;

    tempPtr = new MultiManager;

    tempPtr->nextElem = listHead;
    listHead          = tempPtr;

    return tempPtr;
}

MultiManager *MultiManager::Create(const Uid& uid)
{
    Boolean done = FALSE;

    MultiManager *tempPtr;

    tempPtr = listHead;
    while ((! done) && (tempPtr != NULL))
    {
        if (uid == tempPtr->get_uid())
            done = TRUE;
        else
            tempPtr = tempPtr->nextElem;
    }

    if (tempPtr == NULL)
    {
        tempPtr           = new MultiManager(uid);
        tempPtr->nextElem = listHead;
        listHead          = tempPtr;
    }

    return tempPtr;
}

void MultiManager::Destroy()
{
    MultiManager *tempPtr = listHead;

    while (tempPtr != NULL)
    {
        tempPtr = listHead->nextElem;
        delete listHead;
        listHead = tempPtr;
    }
}

Boolean MultiManager::do_setlock (Manager* manager, LockResult& lockRes, Lock *lock, int retry)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "MultiManager::do_setlock (Lock *lock, int retry)\n" << flush;
#endif

    Boolean res = FALSE;

    AtomicAction A;

    A.Begin();

    if ((manager != NULL) && (FindUid(manager->get_uid()) != -1))
    {
        lockRes = LockManager::setlock(lock, retry);
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

int MultiManager::FindUid(const Uid& uid)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "MultiManager::FindUid(const Uid& uid)\n" << flush;
#endif

    int index = 0;

    AtomicAction A;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
        Boolean done = FALSE;

        while ((! done) && (index < mmListSize) && (index < numObjects))
        {
	    if ((uidList[index] != NULL) && (*uidList[index] == uid))
	        done = TRUE;
	    else
	        index++;
        }
        if (! done)
	    index = -1;
    }

    if (index != -1)
    {
	if (A.End() != COMMITTED)
	    index = -1;
    }
    else
	A.Abort();

    return index;
}

Boolean MultiManager::do_add(Uid uid, Manager *manager, ObjectState* objectState)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "MultiManager::do_add(Manager *manager, ObjectState* objectState)\n" << flush;
#endif

    Boolean res = FALSE;
    AtomicAction A;

    A.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
        Boolean done = FALSE;
	int index = 0;

        while ((! done) && (index < mmListSize))
        {
	    if (uidList[index] == NULL)
	        done = TRUE;
	    else
	        index++;
        }

        if (done)
        {
             uidList[index]         = new Uid(uid);
             objectStateList[index] = objectState;
	     managerList[index]     = manager;

             if (index >= numObjects)
                 numObjects = index + 1;

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

Boolean MultiManager::do_remove(Manager* man, Manager*& manager, ObjectState*& objectState)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "MultiManager::do_remove(Manager* man, Manager*& manager, ObjectState*& objectState)\n" << flush;
#endif

    Boolean res   = FALSE;

    AtomicAction A;

    A.Begin();

    if ((man != NULL) && (setlock(new Lock(WRITE), 0) == GRANTED))
    {
	int index = -1;

        index = FindUid(man->get_uid());

        if (index != -1)
        {
             if (uidList[index] != NULL)
	         delete uidList[index];
             uidList[index] = NULL;

             manager = managerList[index];

             managerList[index] = NULL;
 
             objectState = objectStateList[index];
             objectStateList[index] = NULL;

             while ((numObjects > 0) && (uidList[numObjects - 1] == NULL))
                 numObjects--;

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

Boolean MultiManager::do_register(Manager *manager)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "MultiManager::do_register(Manager *manager)\n" << flush;
#endif

    int     index = -1;
    Boolean res   = FALSE;

    AtomicAction A;

    A.Begin();

    if ((manager != NULL) && (setlock(new Lock(READ), 0) == GRANTED))
    {
        index = FindUid(manager->get_uid());

	if (index != -1)
	{
            managerList[index] = manager;

	    if (objectStateList[index] != NULL)
            {
                objectStateList[index]->reread();
                res = managerList[index]->restore_state(*objectStateList[index], ANDPERSISTENT);
            }
            else
                res = TRUE;
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

Boolean MultiManager::do_unregister(Manager *manager)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "MultiManager::do_unregister(Manager *manager)\n" << flush;
#endif

    int     index = -1;
    Boolean res   = FALSE;

    AtomicAction A;

    A.Begin();

    if ((manager != NULL) && (setlock(new Lock(READ), 0) == GRANTED))
    {
        index = FindUid(manager->get_uid());

	if (index != -1)
	{
	    managerList[index] = NULL;

            res = TRUE;
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

Boolean MultiManager::save_state (ObjectState& os, ObjectType ot)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "MultiManager::save_state(ObjectState& os, ObjectType)\n" << flush;
#endif

    Boolean res = TRUE;
    int index = 0;

    res = os.pack(numObjects);
    for (index = 0; res && (index < mmListSize) && (index < numObjects); index++)
    {
        if (uidList[index] == NULL)
            managerList[index] = NULL;

        if (managerList[index] != NULL )
        {
            if (objectStateList[index] == NULL)
                objectStateList[index] = new ObjectState;
            else
	        objectStateList[index]->rewrite();
	    res = managerList[index]->save_state(*objectStateList[index], ot);
        }
    }

    for (index = 0; res && (index < mmListSize); index++)
	if (objectStateList[index] == NULL)
            res = os.pack(0);
        else
            res = os.pack(1) && uidList[index]->pack(os) && objectStateList[index]->packInto(os);

    return res;
}

Boolean MultiManager::restore_state (ObjectState& os, ObjectType ot)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "MultiManager::restore_state(ObjectState& os, ObjectType)\n";
#endif

    Boolean res;
    int index = 0;

    res = os.unpack(numObjects);
    for (index = 0; res && (index < mmListSize) && (index < numObjects); index++)
    {
        int nullPtr;

        res = os.unpack(nullPtr);
        if (res)
        {
            if (nullPtr == 1)
            {
	        if (uidList[index] == NULL)
	            uidList[index] = new Uid(NIL_UID);

	        if (objectStateList[index] == NULL)
	            objectStateList[index] = new ObjectState;

	        res = uidList[index]->unpack(os) && objectStateList[index]->unpackFrom(os);

                if (res)
                {
                    if ((managerList[index] != NULL) && (managerList[index]->get_uid() != *uidList[index]))
                        managerList[index] = NULL;
                }
            }
            else
            {
	        if (uidList[index] != NULL)
                {
	            delete uidList[index];
                    uidList[index] = NULL;
                }
	
	        if (objectStateList[index] != NULL)
                {
	            delete objectStateList[index];
                    objectStateList[index] = NULL;
                }

                managerList[index] = NULL;
            }
        }
    }

    for (index = numObjects; res && (index < mmListSize); index++)
    {
	 if (uidList[index] != NULL)
         {
	      delete uidList[index];
              uidList[index] = NULL;
         }
	
	 if (objectStateList[index] != NULL)
         {
	      delete objectStateList[index];
              objectStateList[index] = NULL;
         }

         managerList[index] = NULL;
    }

    for (index = 0; res && (index < mmListSize) && (index < numObjects); index++)
	if (managerList[index] != NULL)
	{
	    objectStateList[index]->reread();
	    res = managerList[index]->restore_state(*objectStateList[index], ot);
	}

    return res;
}

const TypeName MultiManager::type () const
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "MultiManager::type()\n" << flush;
#endif

    return "/StateManager/LockManager/MultiManager";
}

MultiManager::MultiManager () : LockManager(ANDPERSISTENT)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "MultiManager::MultiManager ()\n" << flush;
#endif

    numObjects = 0;
    for (int i = 0; i < mmListSize; i++)
    {
	uidList[i]         = NULL;
	objectStateList[i] = NULL;
	managerList[i]     = NULL;
    }

    AtomicAction A;

    A.Begin();

    if (LockManager::setlock(new Lock(WRITE), 0) == GRANTED)
	A.End();
    else
	A.Abort();
}

MultiManager::MultiManager (const Uid& uid): LockManager(uid)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "MultiManager::MultiManager (const Uid& uid)\n" << flush;
#endif

    numObjects = 0;
    for (int i = 0; i < mmListSize; i++)
    {
	uidList[i]         = NULL;
	objectStateList[i] = NULL;
	managerList[i]     = NULL;
    }
}

MultiManager::~MultiManager ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "MultiManager::~MultiManager ()\n" << flush;
#endif
    
    StateManager::terminate();
    
    for (int i = 0; i < mmListSize; i++)
    {
	if (uidList[i] != NULL)
        {
	    delete uidList[i];
            uidList[i] = NULL;
        }
	
	if (objectStateList[i] != NULL)
        {
	    delete objectStateList[i];
            objectStateList[i] = NULL;
        }

        managerList[i] = NULL;
    }
}

ostream& MultiManager::PrintOn (ostream& str) const
{
    for (int index = 0; index < mmListSize; index++)
    {
        str << "Number of objects: " << numObjects << endl;
	if ((managerList[index] != NULL) || (uidList[index] != NULL) || (objectStateList[index] != NULL))
        {
            str << index << " : [" << (void *) managerList[index] << ", ";
	    if (uidList[index] == NULL)
	        str << "NULL ";
	    else
	        str << *uidList[index] << " ";
	    if (objectStateList[index] == NULL)
	        str << "NULL" << endl;
	    else
	        str << *objectStateList[index] << endl;
        }
    }

    str << flush;

    return str;
}
