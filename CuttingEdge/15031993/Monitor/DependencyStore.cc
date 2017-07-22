#ifndef IOSTREAM_H_
#include <System/iostream.h>
#endif

#ifndef SYS_PARAM_H_
#include <System/sys/param.h>
#endif

#ifndef MEMORY_H_
#include <System/memory.h>
#endif

#ifndef STRING_H_
#include <System/string.h>
#endif

#ifndef PROTOS_H_
#include <System/protos.h>
#endif

#ifndef UNISTD_H_
#include <System/unistd.h>
#endif

#ifndef ATOMICA_H_
#include <Arjuna/AtomicA.h>
#endif

#ifndef DEPENDENCYSTORE_H_
#include "DependencyStore.h"
#endif

isDependent::isDependent ()
                          : hostName (0),
			    level(NONE),
			    next(0),
			    noDeps(0),
			    isDeps(0),
			    numbChecks(0)
{
#ifdef DEBUG
    cout << "isDependent::isDependent ()" << endl;
#endif    
}

isDependent::~isDependent ()
{
#ifdef DEBUG
    cout << "isDependent::~isDependent ()" << endl;
#endif    
}

Boolean isDependent::pack (Buffer& buff) const
{
    Boolean result;

    result = buff.pack(hostName) && buff.pack(level);
    result = result && buff.pack(noDeps);
    result = result && buff.pack(isDeps);
    result = result && buff.pack(numbChecks);

    return result;
}

Boolean isDependent::unpack (Buffer buff)
{
    int tempLevel;
    Boolean result;

    result = buff.unpack(hostName) && buff.unpack(tempLevel);

    if (result)
	level = (DependencyLevel) tempLevel;

    result = result && buff.unpack(noDeps);
    result = result && buff.unpack(isDeps);
    result = result && buff.unpack(numbChecks);

    return result;
}

char* isDependent::levelToString (DependencyLevel temp)
{
    char* tempString = 0;
    
    switch (temp)
    {
    case NONE:
	tempString = "none";
	break;
    case SLIGHT:
	tempString = "slight";
	break;
    case MINOR:
	tempString = "minor";
	break;
    case AVERAGE:
	tempString = "average";
	break;
    case MAJOR:
	tempString = "high";
	break;
    case CRITICAL:
	tempString = "very high";
	break;
    }
    
    return tempString;
}

ostream& isDependent::print (ostream& strm) const
{
    strm << "host name : " << hostName << endl;
    strm << "probability of dependence : " << isDependent::levelToString(level) << endl;
    strm << "number of checks : " << numbChecks << endl;
    strm << "number of presumed dependencies : " << isDeps << endl;
    strm << "number of presumed non-dependencies : " << noDeps << endl;
    
    if (next)
	strm << *next;
    else
	strm << "End of list" << endl;

    return strm;
}

ostream& operator<< (ostream& strm, const isDependent& id)
{
    return id.print(strm);
}



DependencyStore::DependencyStore (char* name) : LockManager(ANDPERSISTENT)
{
#ifdef DEBUG
    cout << "DependencyStore::DependencyStore ( " << name << " )" << endl;
#endif

    hostName = new char[strlen(name)+1];
    memset(hostName, '\0', strlen(name)+1);
    memcpy(hostName, name, strlen(name));

    dependencyList = 0;
    numberDependents = 0;
}

DependencyStore::DependencyStore (const Uid& u) : LockManager(u)
{
#ifdef DEBUG
    cout << "DependencyStore::DependencyStore (const Uid& u)" << endl;
#endif
    AtomicAction A;
    
    hostName = 0;
    dependencyList = 0;
    numberDependents = 0;
    
    A.Begin();
    
    if (setlock(new Lock(READ), 0) == GRANTED)
	A.End();
    else
	A.Abort();
}

DependencyStore::~DependencyStore ()
{
#ifdef DEBUG
    cout << "DependencyStore::~DependencyStore ( " << myName() << " )" << endl;
#endif

    terminate();

    if (dependencyList)
	deleteList();
}

Boolean DependencyStore::isSame (char* name)
{
#ifdef DEBUG
    cout << "Boolean DependencyStore::isSame" << endl;
#endif
    
    Boolean same = FALSE;
    AtomicAction A;
    
    A.Begin();
    
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (!name)
	{
	    A.Abort();
	    return FALSE;
	}

	if (strcmp(hostName, name) == 0)
	    same = TRUE;

	if (A.End() != COMMITTED)
	    same = FALSE;
    }
    else
	A.Abort();

    return same;
}

void DependencyStore::deleteList ()
{
#ifdef DEBUG
    cout << "void DependencyStore::deleteList ()" << endl;
#endif
    
    isDependent *ptr = dependencyList, *marker;

    while (ptr)
    {
	marker = ptr->next;
	delete ptr;
	ptr = marker;
    }

    dependencyList = 0;
}

char* DependencyStore::myName () const { return hostName; }

isDependent* DependencyStore::DepList ()
{
#ifdef DEBUG
    cout << "isDependent* DependencyStore::DepList ()" << endl;
#endif
    
    AtomicAction A;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (A.End() == COMMITTED)
	    return dependencyList;
    }
    else
	A.Abort();

    return 0;
}

Boolean DependencyStore::findConflict (char* name, isDependent* ptr)
{
#ifdef DEBUG
    cout << "Boolean DependencyStore::findConflict" << endl;
#endif
    
    AtomicAction A;
    Boolean found = FALSE;

    A.Begin();
    
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	while ((ptr != 0) && (!found))
	{
	    if (strcmp(ptr->hostName, name) == 0)
		found = TRUE;
	    else
		ptr = ptr->next;
	}

	if (A.End() != COMMITTED)
	    found = FALSE;
    }
    else
	A.Abort();

    return found;
}

Boolean DependencyStore::addName (char* name, DependencyLevel level)
{
#ifdef DEBUG
    cout << "Boolean DependencyStore::addName" << endl;
#endif
    
    AtomicAction A;
    isDependent* ptr;
    Boolean result = FALSE;

    A.Begin();
    
    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	if (!dependencyList)
	{
	    dependencyList = new isDependent;
	    ptr = dependencyList;
	}
	else
	{
	    ptr = dependencyList;
	    while (ptr->next)
		ptr = ptr->next;
	
	    ptr->next = new isDependent;
	    ptr = ptr->next;
	}

	ptr->hostName = name;
	ptr->level = level;
	ptr->numbChecks = 1;
	if (level)
	    ptr->isDeps = 1;
	else
	    ptr->noDeps = 1;
	
	numberDependents++;

	if (A.End() == COMMITTED)
	    result = TRUE;
    }
    else
	A.Abort();
    
    return result;
}
	    
Boolean DependencyStore::addDependency (char* name, DependencyLevel level)
{
#ifdef DEBUG
    cout << "Boolean DependencyStore::addDependency" << endl;
#endif
    
    AtomicAction A;
    Boolean result = FALSE, done = FALSE;

    A.Begin();
  
    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	isDependent* ptr = dependencyList;

	if (findConflict(name, ptr))
	{
	    ptr->numbChecks++;
	    if (level == NONE)
		ptr->noDeps++;
	    else
		ptr->isDeps++;

	    ptr->level = calcDep(ptr);
	    done = TRUE;
	}
	else
	    done = addName(name, level);

	if (done)
	{
	    if (A.End() == COMMITTED)
		result = TRUE;
	}
	else
	    A.Abort();
    }
    else
	A.Abort();

    return result;
}

DependencyLevel DependencyStore::calcDep (isDependent* ptr)
{
#ifdef DEBUG
    cout << "DependencyLevel DependencyStore::calcDep" << endl;
#endif

    DependencyLevel calcLevel;
    double percent = ((double)ptr->isDeps/(double)ptr->numbChecks) * 100;

    if (percent == 0)
	calcLevel = NONE;
    else if ((percent > 0) && (percent < 10))
	calcLevel = SLIGHT;
    else if ((percent >= 10) && (percent < 30))
	calcLevel = MINOR;
    else if ((percent >= 30) && (percent < 50))
	calcLevel = AVERAGE;
    else if ((percent >= 50) && (percent < 80))
	calcLevel = MAJOR;
    else if (percent >= 80)
	calcLevel = CRITICAL;

    return calcLevel;
}

Boolean DependencyStore::changeDependency (char* name, DependencyLevel level)
{
#ifdef DEBUG
    cout << "Boolean DependencyStore::changeDependency" << endl;
#endif
    
    return addDependency(name, level);
}

Boolean DependencyStore::deleteDependency (char* name, int& number)
{
#ifdef DEBUG
    cout << "Boolean DependencyStore::deleteDependency" << endl;
#endif
    
    Boolean result = FALSE;
    AtomicAction A;

    A.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	deleteName(name, number);

	if (A.End() == COMMITTED)
	    result = TRUE;
    }
    else
	A.Abort();

    return result;
}

void DependencyStore::deleteName (char* name, int& number)
{
#ifdef DEBUG
    cout << "void DependencyStore::deleteName" << endl;
#endif
    
    Boolean found = FALSE;
    AtomicAction A;
    
    A.Begin();
    
    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	isDependent *ptr = dependencyList, *marker = 0;

	while ((ptr) && (!found))
	{
	    if (strcmp(ptr->hostName, name) == 0)
		found = TRUE;
	    else
	    {
		marker = ptr;
		ptr = ptr->next;
	    }
	}

	if (found)
	{
	    marker->next = ptr->next;
	    delete ptr;
	    numberDependents--;
	    number = numberDependents;
	}

	A.End(); // ??!!
    }
    else
	A.Abort();
}

Boolean DependencyStore::numberDeps (int& number)
{
#ifdef DEBUG
    cout << "Boolean DependencyStore::numberDeps" << endl;
#endif
    
    AtomicAction A;
    Boolean result = FALSE;
    
    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (A.End() == COMMITTED)
	{
	    result = TRUE;
	    number = numberDependents;
	}
    }
    else
	A.Abort();

    return result;
}

ostream& DependencyStore::my_print (ostream& strm) const
{
    strm << "****" << endl;
    strm << "Host " << myName() << " has the following dependency probabilities" << endl;

    if (dependencyList)
	strm << *dependencyList;
    else
	strm << "No observed dependencies" << endl;
    
    return strm;
}

ostream& operator<< (ostream& strm, const DependencyStore& ds)
{
    return ds.my_print(strm);
}

Boolean DependencyStore::save_state (ObjectState& os, ObjectType)
{
#ifdef DEBUG
    cout << "Boolean DependencyStore::save_state" << endl;
#endif
    
    Boolean result;
    isDependent* ptr = dependencyList;

    result = os.pack(hostName) && os.pack(numberDependents);
    while (ptr)
    {
	result = result && ptr->pack(os);
	ptr = ptr->next;
    }
    
    return result;
}

Boolean DependencyStore::restore_state (ObjectState& os, ObjectType)
{
#ifdef DEBUG
    cout << "Boolean DependencyStore::restore_state" << endl;
#endif
    
    Boolean result;
    
    if (dependencyList)
	deleteList();

    result = os.unpack(hostName) && os.unpack(numberDependents);

    isDependent* ptr = 0;

    for (int i = 0; i < numberDependents; i++)
    {
	if (!ptr)
	{
	    dependencyList = new isDependent;
	    ptr = dependencyList;
	}
	else
	{
	    ptr->next = new isDependent;
	    ptr = ptr->next;
	}

	result = result && ptr->unpack(os);
    }

    return result;
}

const TypeName DependencyStore::type () const
{
    return "/StateManager/LockManager/DependencyStore";
}
