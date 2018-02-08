/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Queue.h,v 1.2 1994/07/08 15:27:53 nmcl Exp $
 */


#ifndef QUEUE_H_
#define QUEUE_H_

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef STATEMAN_H_
#  include <Arjuna/StateMan.h>
#endif

#ifndef OBJSTATE_H_
#  include <Arjuna/ObjState.h>
#endif

#ifndef QUEUEELEMENT_H_
#  include "QueueElement.h"
#endif


const unsigned int QUEUE_SIZE = 40;

enum QueueStatus { DONE, NOT_DONE, UNDER_FLOW, OVER_FLOW };


class QueueLinks
{
public:
    QueueLinks ();
    ~QueueLinks ();

    QueueElement* data;
    QueueLinks* next;
    QueueLinks* prev;
};


class Queue : public StateManager
{
public:    
    Queue (const Uid& u, Boolean& res);
    Queue (Boolean& res);
    ~Queue ();
    
    QueueStatus  enqueue (int);
    QueueStatus  dequeue (int&);
    
    unsigned int queueSize ();
    Boolean      inspectValue (unsigned int, int&);
    Boolean      setValue (unsigned int, int);
    
    virtual Boolean save_state (ObjectState&, ObjectType);
    virtual Boolean restore_state (ObjectState&, ObjectType);
    virtual const TypeName type () const;
    
private:
    QueueLinks* headOfList;
    QueueLinks* tailOfList;
    unsigned int  numberOfElements;
};

#endif
