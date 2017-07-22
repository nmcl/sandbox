#ifndef TRACKER_H_
#define TRACKER_H_

#ifndef SYS_TIME_H_
#include <System/sys/time.h>
#endif

#ifndef BOOLEAN_H_
#include <Common/Boolean.h>
#endif

#ifndef BUFFER_H_
#include <Common/Buffer.h>
#endif

#ifndef UID_H_
#include <Common/Uid.h>
#endif

#ifndef OBJSTATE_H_
#include <Arjuna/ObjState.h>
#endif

#ifndef LOCKMAN_H_
#include <Arjuna/LockMan.h>
#endif

#ifndef DEPENDENCY_H_
#include "Dependency.h"
#endif

class NodeData
{
public:
    NodeData ();
    ~NodeData ();

    Boolean pack(Buffer&) const;
    Boolean unpack(Buffer&);

    virtual ostream& print (ostream&) const;

    char* nodeName;
    timeval TTF, TTR;
    NodeData* next;
};

class Tracker : public LockManager
{
public:
    Tracker (int&);
    Tracker (const Uid&, int&);
    ~Tracker ();

    Boolean registerNode (char*, timeval, timeval);
    Boolean unregisterNode (char*);

    Boolean modifyNode (char*, timeval, timeval);
    Boolean updateNode (char*, timeval, timeval);

    Boolean listNodes (NodeData*&);
    int getNumberOfNodes ();
    
    Boolean doTrack ();

    virtual ostream& my_print (ostream&) const;
    
    virtual Boolean save_state (ObjectState&, ObjectType);
    virtual Boolean restore_state (ObjectState&, ObjectType);
    virtual const TypeName type () const;

private:
    Boolean hasDependency (NodeData*, NodeData*);

    void deleteList ();

    Dependency* depList;
    NodeData* head;
    int numberOfNodes;
};

extern ostream& operator<< (ostream& strm, const NodeData& nd);

extern ostream& operator<< (ostream& strm, const Tracker& t);

#endif
