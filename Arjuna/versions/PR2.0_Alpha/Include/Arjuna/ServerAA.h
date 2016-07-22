/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: ServerAA.h,v 1.1 1993/04/05 13:23:24 ngdp Exp $
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


class ServerAtomicAction : public AtomicAction
{
public:
    /* Constructors and destructor */

    ServerAtomicAction (const Uid&, ActionType);
    virtual ~ServerAtomicAction ();

    static ServerAtomicAction *Current();

    /* User Atomic action operations */
 
    virtual ActionStatus Begin ();
    virtual ActionStatus End ();
    virtual ActionStatus Abort ();

    virtual Uid get_saving_uid () const;

    /* NOTE static operations */

    static void  set_coordin_nodename (const char* name);
    static char* get_coordin_nodename ();
  
    /* non-virtual member functions and operators */

    void Finish ();

    ServerAtomicAction *parent ();

    /* Primitive Atomic action operations */

    virtual Boolean DoNested_Abort (Uid&);
    virtual Boolean DoNested_Commit (Uid&);
    virtual Boolean DoNested_Prepare (Uid&, PrepareOutcome&);
    virtual Boolean DoTopLevel_Abort (Uid&);
    virtual Boolean DoTopLevel_Commit (Uid&);
    virtual Boolean DoTopLevel_Prepare (Uid&, PrepareOutcome&);

    /* inherited public virtual functions */

    virtual Boolean restore_state (ObjectState&, ObjectType);
    virtual Boolean save_state (ObjectState&, ObjectType);
    virtual const TypeName type () const;

protected:

    /* note the static declaration of the following */

    static ServerAtomicAction *current_svr_act;

private:
    static char*               coordin_nodename;

    Uid                 saving_uid;
    ServerAtomicAction* parent_server_action;
    PrepareOutcome      previousPrepareOutcome;
    Boolean             saved_intention_list;
};

#endif
