/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: ServerAA.h,v 1.2 1994/10/10 11:13:06 ngdp Exp $
 */

#ifndef SERVERAA_H_
#define SERVERAA_H_

/*
 *
 * ServerAtomicAction class
 *
 */

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

class ActionHierarchy;

class ServerAtomicAction : public AtomicAction
{
    friend class ActionHierarchy;

public:
    /* Constructors and destructor */

    ServerAtomicAction (const Uid&, ActionType = NESTED);
    virtual ~ServerAtomicAction ();

    static ServerAtomicAction *currentSAA();

    /* User Atomic action operations */
 
    virtual ActionStatus Begin ();
    virtual ActionStatus End ();
    virtual ActionStatus Abort ();

    virtual Uid getSavingUid () const;

    /* NOTE static operations */

    static void  setCoordinNodename (const char*);
    static char* getCoordinNodename ();
  
    /* non-virtual member functions and operators */

    void finish ();

    ServerAtomicAction *parentSAA ();

    /* Primitive Atomic action operations */

    virtual Boolean doNestedAbort (Uid&);
    virtual Boolean doNestedCommit (Uid&);
    virtual Boolean doNestedPrepare (Uid&, PrepareOutcome&);
    virtual Boolean doTopLevelAbort (Uid&);
    virtual Boolean doTopLevelCommit (Uid&);
    virtual Boolean doTopLevelPrepare (Uid&, PrepareOutcome&);

    /* inherited public virtual functions */

    virtual Boolean restore_state (ObjectState&, ObjectType);
    virtual Boolean save_state (ObjectState&, ObjectType);
    virtual const TypeName type () const;

protected:

    /* note the static declaration of the following */

    static ServerAtomicAction *currentSvrAct;

private:
    static char*        coordinNodename;

    ServerAtomicAction* parentServerAction;
    PrepareOutcome      previousPrepareOutcome;
    Uid                 savingUid;
};

#endif
