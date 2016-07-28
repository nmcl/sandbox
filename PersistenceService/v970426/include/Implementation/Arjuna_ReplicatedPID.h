#ifndef IMPLEMENTATION_ARJUNA_REPLICATEDPID_H_
#define IMPLEMENTATION_ARJUNA_REPLICATEDPID_H_

#ifndef COMMON_UID_H_
#  include <Common/Uid.h>
#endif

#ifndef COMMON_NAMESPACE_H_
#  include <Common/nameSpace.h>
#endif

#ifdef HAVE_CH_EXTENSION
#  include <Interface/Replicated_PID.ch>
#else
#  include <Interface/Replicated_PID.h>
#endif

/*
 * Format of PID name:
 *
 * <PID key>:<Number>:ListOf<RemotePID>
 */

class Arjuna_ReplicatedPID : public BOA_(Replicated_PID)
{
public:
    Arjuna_ReplicatedPID ();
    Arjuna_ReplicatedPID (const char* string);
    Arjuna_ReplicatedPID (const char* storeType, const char* type, const char* id);
    virtual ~Arjuna_ReplicatedPID ();

    void numberOfReplicas (long numberOfReplicas, CORBA_(Environment& env));
    long numberOfReplicas (CORBA_(Environment& env));
    
    CosPersistencePID::PID_ptr getPid (long index, CORBA_(Environment& env));

    static CosPersistencePID::PID_ptr create ();    
    static CosPersistencePID::PID_ptr create (const char* string);
    
    static const char* pidName ();
    
protected:
    long _numberOfReplicas;
    CosPersistencePID::PID_ptr** _thePIDs;

private:
    void setDefault ();
};

class Arjuna_ReplicatedPID_Init
{
public:
    Arjuna_ReplicatedPID_Init ();
    virtual ~Arjuna_ReplicatedPID_Init ();

private:
    static Boolean added;
};

static Arjuna_ReplicatedPID_Init initReplicatedPID;

#endif
