#ifndef IMPLEMENTATION_PIDINVENTORY_H_
#define IMPLEMENTATION_PIDINVENTORY_H_

#ifndef COMMON_NAMESPACE_H_
#  include <Common/nameSpace.h>
#endif

#ifdef HAVE_CH_EXTENSION
#  include <Interface/CosPersistencePID.ch>
#else
#  include <Interface/CosPersistencePID.h>
#endif

#ifndef IMPLEMENTATION_ARJUNA_BASICPID_H_
#  include <Implementation/Arjuna_BasicPID.h>
#endif

#ifndef IMPLEMENTATION_ARJUNA_REMOTEPID_H_
#  include <Implementation/Arjuna_RemotePID.h>
#endif

class PIDListElement;

class PIDInventory
{
public:
    static void addCreateVoid (CosPersistencePID::PID_ptr (*create)(), const char* name);
    static void addCreateString (CosPersistencePID::PID_ptr (*create)(const char*), const char* name);

    static CosPersistencePID::PID_ptr createVoid (const char* name);
    static CosPersistencePID::PID_ptr createString (const char* name, const char* string);

protected:
    PIDInventory ();
    ~PIDInventory ();
    
private:
    static PIDListElement* _headOfList;
};

#endif
