#ifndef DEPENDENCY_H_
#define DEPENDENCY_H_

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

#ifndef DEPENDENCYSTORE_H_
#include "DependencyStore.h"
#endif

class listElement
{
public:
    listElement ();
    ~listElement ();
    
    DependencyStore* element;
    listElement* next;
};


class toDelete
{
public:
    toDelete ();
    ~toDelete ();
    
    void addElement (listElement*);
    void purgeList ();

private:
    listElement* head;
};


class Dependency : public LockManager
{
public:
    Dependency (int&);
    Dependency (Uid&, int&);
    ~Dependency ();

    Boolean assignDependency (char*, char*, DependencyLevel);
    Boolean deleteDependency (char*, char*);
    Boolean getDependency (char*, isDependent**);

    virtual ostream& my_print (ostream&) const;

    virtual Boolean save_state (ObjectState&, ObjectType);
    virtual Boolean restore_state (ObjectState&, ObjectType);
    virtual const TypeName type () const;

private:
    Boolean scanForName (char*, listElement*&);
    DependencyStore* createNewName (char*);

    void deleteElement (DependencyStore*);
    void deleteList ();

    int numberElements;
    listElement* head;
    toDelete deletedObjects;
};

extern ostream& operator<< (ostream& strm, const Dependency& d);

#endif
