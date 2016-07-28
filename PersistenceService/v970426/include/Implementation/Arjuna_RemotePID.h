#ifndef IMPLEMENTATION_ARJUNA_REMOTEPID_H_
#define IMPLEMENTATION_ARJUNA_REMOTEPID_H_

#ifndef COMMON_UID_H_
#  include <Common/Uid.h>
#endif

#ifndef COMMON_NAMESPACE_H_
#  include <Common/nameSpace.h>
#endif

#ifdef HAVE_CH_EXTENSION
#  include <Interface/PID_RemoteService.ch>
#else
#  include <Interface/PID_RemoteService.h>
#endif

/*
 * Format of PID name:
 *
 * <PID key>:<Host>:<Basic PID>
 */

class Arjuna_RemotePID : public BOA_(PID_RemoteService)
{
public:
    Arjuna_RemotePID ();
    Arjuna_RemotePID (const char* string);
    Arjuna_RemotePID (const char* hostName, const char* pid);
    virtual ~Arjuna_RemotePID ();

    void  datastore_type (const char* type, CORBA_(Environment& env));
    char* datastore_type (CORBA_(Environment& env));

    char* get_PIDString (CORBA_(Environment& env));
    
    void hostName (const char* hostName, CORBA_(Environment& env));
    char* hostName (CORBA_(Environment& env));
    
    void pidString (const char* string, CORBA_(Environment& env));
    char* pidString (CORBA_(Environment& env));

    static CosPersistencePID::PID_ptr create ();    
    static CosPersistencePID::PID_ptr create (const char* string);
    
protected:
    char* _hostName;
    char* _pidString;

private:
    void setDefault ();
};

class Arjuna_RemotePID_Init
{
public:
    Arjuna_RemotePID_Init ();
    virtual ~Arjuna_RemotePID_Init ();

private:
    static Boolean added;
};

static Arjuna_RemotePID_Init initRemotePID;

#endif
