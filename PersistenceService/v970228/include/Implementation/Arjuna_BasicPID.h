#ifndef IMPLEMENTATION_ARJUNA_BASICPID_H_
#define IMPLEMENTATION_ARJUNA_BASICPID_H_

#ifndef COMMON_UID_H_
#  include <Common/Uid.h>
#endif

#ifndef COMMON_NAMESPACE_H_
#  include <Common/nameSpace.h>
#endif

#ifdef HAVE_CH_EXTENSION
#  include <Interface/PID_Uid.ch>
#else
#  include <Interface/PID_Uid.h>
#endif

/*
 * Format of PID name:
 *
 * <PID key>:<Store type>:<PID data>
 *
 * Implementation:
 * <Arjuna_PID>:<Store type>:<TypeName>:<Uid>
 */

class Arjuna_BasicPID : public BOA_(PID_Uid)
{
public:
    Arjuna_BasicPID ();
    Arjuna_BasicPID (const char* string);
    Arjuna_BasicPID (const char* storeType, const char* type, const Uid& u);
    virtual ~Arjuna_BasicPID ();

    void  uid (const char* id, CORBA_(Environment& env));
    char* uid (CORBA_(Environment& env));

    void  typeName (const char* name, CORBA_(Environment& env));
    char* typeName (CORBA_(Environment& env));

    void  datastore_type (const char* type, CORBA_(Environment& env));
    char* datastore_type (CORBA_(Environment& env));

    void  objectStore (const char* type, CORBA_(Environment& env));
    char* objectStore (CORBA_(Environment& env));    

    char* get_PIDString (CORBA_(Environment& env));

    static CosPersistencePID::PID_ptr create ();    
    static CosPersistencePID::PID_ptr create (const char* string);
    
protected:
    Uid   _uid;
    char* _name;
    char* _objectStoreType;

private:
    void setDefault ();
};

class Arjuna_BasicPID_Init
{
public:
    Arjuna_BasicPID_Init ();
    virtual ~Arjuna_BasicPID_Init ();

private:
    static Boolean added;
};

static Arjuna_BasicPID_Init initBasicPID;

#endif
