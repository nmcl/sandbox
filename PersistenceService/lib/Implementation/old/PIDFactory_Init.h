#ifndef IMPLEMENTATION_PIDFACTORY_INIT_H_
#define IMPLEMENTATION_PIDFACTORY_INIT_H_

#include <Interface/CosPersistencePID.h>

class PIDFactory_Init
{
public:
    virtual const char* pidName () const = 0;

    virtual CosPersistencePID::PID_ptr create () = 0;
    virtual CosPersistencePID::PID_ptr create (const char* string) = 0;
    
protected:
    PIDFactory_Init ();
    virtual ~PIDFactory_Init ();
};

#endif
