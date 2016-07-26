#ifndef IMPLEMENTATION_ARJUNA_REMOTEPDS_H_
#define IMPLEMENTATION_ARJUNA_REMOTEPDS_H_

#ifndef COMMON_KUBERA_H_
#  include <Common/Kubera.h>
#endif

#ifndef COMMON_NAMESPACE_H_
#  include <Common/nameSpace.h>
#endif

#ifndef COMMON_COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef COMMON_UID_H_
#  include <Common/Uid.h>
#endif

#ifdef HAVE_CH_EXTENSION
#  include <Interface/PackablePO.ch>
#  include <Interface/CosPersistencePDS.ch>
#  include <Interface/PDSFactory.ch>
#else
#  include <Interface/PackablePO.h>
#  include <Interface/CosPersistencePDS.h>
#  include <Interface/PDSFactory.h>
#endif

class Arjuna_RemotePDS : public BOA_(CosPersistencePDS::PDS)
{
public:
    Arjuna_RemotePDS ();
    virtual ~Arjuna_RemotePDS ();
    
    CosPersistencePDS::PDS_ptr connect (CORBA::Object_ptr obj, CosPersistencePID::PID_ptr pid, CORBA_(Environment& env));
    
    void disconnect  (CORBA::Object_ptr obj, CosPersistencePID::PID_ptr pid, CORBA_(Environment& env));
    void store       (CORBA::Object_ptr obj, CosPersistencePID::PID_ptr pid, CORBA_(Environment& env));
    void restore     (CORBA::Object_ptr obj, CosPersistencePID::PID_ptr pid, CORBA_(Environment& env));
    void deleteState (CORBA::Object_ptr obj, CosPersistencePID::PID_ptr pid, CORBA_(Environment& env));

    static CosPersistencePDS::PDS_ptr create ();

    static const char* pdsName ();
    
protected:
    void resetStore   ();
    void setupStore   (CosPersistencePID::PID_ptr pid);
    
private:
    ObjectStatus               _currentStatus;
    CosPersistencePDS::PDS_ptr _theRealObjectStore;
    PDSFactory_ptr             _thePDSFactory;
    CosPersistencePID::PID_ptr _theRemotePID;
};

class Arjuna_RemotePDS_Init
{
public:
    Arjuna_RemotePDS_Init ();
    ~Arjuna_RemotePDS_Init ();

private:
    static Boolean added;
};

static Arjuna_RemotePDS_Init initRemotePDS;

#endif
