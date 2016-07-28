#ifndef IMPLEMENTATION_ARJUNA_BASICPDS_H_
#define IMPLEMENTATION_ARJUNA_BASICPDS_H_

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
#else
#  include <Interface/PackablePO.h>
#  include <Interface/CosPersistencePDS.h>
#endif

class ObjectStore;

class Arjuna_BasicPDS : public BOA_(CosPersistencePDS::PDS)
{
public:
    Arjuna_BasicPDS ();
    virtual ~Arjuna_BasicPDS ();
    
    CosPersistencePDS::PDS_ptr connect (CORBA::Object_ptr obj, CosPersistencePID::PID_ptr pid, CORBA_(Environment& env));
    
    void disconnect  (CORBA::Object_ptr obj, CosPersistencePID::PID_ptr pid, CORBA_(Environment& env));
    void store       (CORBA::Object_ptr obj, CosPersistencePID::PID_ptr pid, CORBA_(Environment& env));
    void restore     (CORBA::Object_ptr obj, CosPersistencePID::PID_ptr pid, CORBA_(Environment& env));
    void deleteState (CORBA::Object_ptr obj, CosPersistencePID::PID_ptr pid, CORBA_(Environment& env));

    static CosPersistencePDS::PDS_ptr create ();

    static const char* pdsName ();
    
protected:
    void setupStore   (CosPersistencePID::PID_ptr pid);
    
private:
    void resetStore ();

    ObjectStatus   _currentStatus;
    ObjectStore*   _objectStore;
    Uid*           _objectUid;
    TypeName       _typeName;
    PackablePO_ptr _theRealObject;
};

class Arjuna_BasicPDS_Init
{
public:
    Arjuna_BasicPDS_Init ();
    ~Arjuna_BasicPDS_Init ();

private:
    static Boolean added;
};

static Arjuna_BasicPDS_Init initBasicPDS;

#endif
