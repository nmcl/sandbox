#ifndef DEPENDENCYSTORE_H_
#define DEPENDENCYSTORE_H_

#ifndef BOOLEAN_H_
#include <Common/Boolean.h>
#endif

#ifndef UID_H_
#include <Common/Uid.h>
#endif

#ifndef BUFFER_H_
#include <Common/Buffer.h>
#endif

#ifndef OBJSTATE_H_
#include <Arjuna/ObjState.h>
#endif

#ifndef LOCKMAN_H_
#include <Arjuna/LockMan.h>
#endif

enum DependencyLevel
{
    NONE, SLIGHT, MINOR,
    AVERAGE, MAJOR, CRITICAL
};

class isDependent
{
public:
    isDependent ();
    ~isDependent ();

    char* hostName;
    DependencyLevel level;
    int noDeps, isDeps, numbChecks;
    isDependent* next;

    static char* levelToString (DependencyLevel);

    Boolean pack (Buffer&) const;
    Boolean unpack (Buffer);

    virtual ostream& print (ostream&) const;
};


class DependencyStore : public LockManager
{
public:
    DependencyStore (char*);
    DependencyStore (const Uid&);
    ~DependencyStore ();

    Boolean addDependency (char*, DependencyLevel);
    Boolean changeDependency (char*, DependencyLevel);
    Boolean deleteDependency (char*, int&);

    Boolean isSame (char*);
    Boolean numberDeps (int&);

    char* myName () const;
    isDependent* DepList ();

    virtual Boolean save_state (ObjectState&, ObjectType);
    virtual Boolean restore_state (ObjectState&, ObjectType);
    virtual const TypeName type () const;

    virtual ostream& my_print (ostream&) const;

private:
    Boolean findConflict (char*, isDependent*);
    Boolean addName (char*, DependencyLevel);
    void deleteName (char*, int&);

    void deleteList();

    DependencyLevel calcDep (isDependent*);

    char* hostName;
    isDependent* dependencyList;
    int numberDependents;
};

extern ostream& operator<< (ostream& strm, const DependencyStore& ds);

extern ostream& operator<< (ostream& strm, const isDependent& id);

#endif
