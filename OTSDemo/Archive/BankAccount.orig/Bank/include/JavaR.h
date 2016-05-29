/*
 * Copyright (C) 1995-1997,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: JavaR.h,v 1.5 1995/02/23 15:09:31 nmcl Exp $
 */

/*
 *
 * Java Record Class
 *
 */

#ifndef JAVAR_H_
#define JAVAR_H_

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef RECORDT_H_
#  include <Arjuna/RecordT.h>
#endif

#ifndef ABSTRACTR_H_
#  include <Arjuna/AbstractR.h>
#endif

#include <Common/Orb.h>

#include <idl/CosTransactions_H.h>

#include <Common/ORBInterface.h>
#include <Common/nameSpace.h>
#include <CosTransactions/CosTranNames.h>

#include <PortAddress.h>
#include <RelPort.h>

class Applet;
class ostream;

/*
 * Pass in the socket which the Arjuna process uses for
 * communicating with the outside (Java) world. We will
 * also use this for sending the outcome of the action.
 * If the Java user crashes then we either abort, or
 * store the outcome locally and assume he will try again
 * later, at which point we can resend him the outcome.
 *
 * Each transaction has a unique tid and we assume the user
 * will retransmit this upon crash recovery.
 */

class JavaRecord : public SCOPED_BOA_(CosTransactions,Resource)
{
public:
    /* Constructors and destructor */

    JavaRecord (const Uid& javaUid, int amount, int outcome,
		const char* name, ReliablePort& port);
    virtual ~JavaRecord ();

    virtual Vote prepare  (CORBA_(Environment)& IT_env) THROWS((SystemException));
    virtual void rollback (CORBA_(Environment)& IT_env) THROWS((SystemException, HeuristicCommit, HeuristicMixed, HeuristicHazard));
    virtual void commit (CORBA_(Environment)& IT_env) THROWS((SystemException, NotPrepared, HeuristicRollback, HeuristicMixed, HeuristicHazard));
    virtual void forget (CORBA_(Environment)& IT_env) THROWS((SystemException));
    virtual void commit_one_phase (CORBA_(Environment)& env) THROWS((SystemException, HeuristicHazard));

    virtual Vote prepare  () THROWS((SystemException));
    virtual void rollback () THROWS((SystemException, HeuristicCommit, HeuristicMixed, HeuristicHazard));
    virtual void commit () THROWS((SystemException, NotPrepared, HeuristicRollback, HeuristicMixed, HeuristicHazard));
    virtual void forget () THROWS((SystemException));
    virtual void commit_one_phase () THROWS((SystemException, HeuristicHazard));
    
    virtual Boolean restore_state (ObjectState& os, ObjectType ot);
    virtual Boolean save_state (ObjectState& os, ObjectType ot);

private:
    /* private state variables */
    
    char* id;
    int cashAmount;
    Uid tid;
    Applet* applet;
};


#endif





























