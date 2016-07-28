#ifndef IMPLEMENTATION_PIDFACTORY_INIT_H_
#define IMPLEMENTATION_PIDFACTORY_INIT_H_

class PIDFactory_ptr;

class PIDFactory_Init
{
public:
    virtual const char* factoryName () const = 0;

    virtual PIDFactory_ptr create () = 0;
    virtual PIDFactory_ptr create (const char* string) = 0;
    
protected:
    PIDFactory_Init ();
    virtual ~PIDFactory_Init ();
};

#endif
