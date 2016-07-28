/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Queue.h,v 1.2 1994/07/08 15:28:00 nmcl Exp $
 */


#ifndef QUEUE_H_
#define QUEUE_H_

#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#ifndef NETDB_H_
#  include <System/netdb.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef LOCKMAN_H_
#  include <Arjuna/LockMan.h>
#endif

#ifndef OBJSTATE_H_
#  include <Arjuna/ObjState.h>
#endif

#ifndef QUEUEELEMENT_H_
#  include "QueueElement.h"
#endif


const unsigned int QUEUE_SIZE = 40;

enum QueueStatus { DONE, NOT_DONE, UNDER_FLOW, OVER_FLOW };


//@NoRemote, @NoMarshall
class QueueLinks
{
public:
    QueueLinks ();
    ~QueueLinks ();

    QueueElement* data;
    QueueLinks* next;
    QueueLinks* prev;
};


class Queue : public LockManager
{
public:    
    Queue (const Uid& u, Boolean& res);
    Queue (Boolean& res);
    ~Queue ();
    
    QueueStatus  enqueue (int, int&);
    QueueStatus  dequeue (int&, int&);
    
    int          queueSize (int&);
    Boolean      inspectValue (unsigned int, int&, int&);
    Boolean      setValue (unsigned int, int, int&);

    Boolean hasRemoteState () const;

    virtual Boolean save_state (ObjectState&, ObjectType);
    virtual Boolean restore_state (ObjectState&, ObjectType);
    virtual const TypeName type () const;
    
private:
    char hostName[MAXHOSTNAMELEN+1];

    QueueLinks*   headOfList;
    QueueLinks*   tailOfList;
    unsigned int  numberOfElements;
};

#endif
