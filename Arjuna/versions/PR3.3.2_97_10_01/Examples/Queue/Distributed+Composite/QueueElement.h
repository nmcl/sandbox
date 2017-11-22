/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: QueueElement.h,v 1.2 1994/01/22 12:17:59 nmcl Exp $
 */

#ifndef QUEUEELEMENT_H_
#define QUEUEELEMENT_H_


#ifndef LOCKMAN_H_
#  include <Arjuna/LockMan.h>
#endif


class QueueElement : public LockManager
{
public:
    QueueElement (const Uid&, Boolean&);
    QueueElement (int, Boolean&);
    ~QueueElement ();
    
    Boolean getValue (int&, int&);
    Boolean setValue (int, int&);
    
    virtual Boolean save_state (ObjectState&, ObjectType);
    virtual Boolean restore_state (ObjectState&, ObjectType);
    virtual const TypeName type () const;

private:
    int           value;
};

#endif

    
