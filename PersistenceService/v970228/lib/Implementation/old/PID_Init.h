#ifndef IMPLEMENTATION_PID_INIT_H_
#define IMPLEMENTATION_PID_INIT_H_

class PID_Init
{
public:
    virtual const char* pidName () const = 0;

    virtual CosPersistencePID::PID_ptr create () = 0;
    virtual CosPersistencePID::PID_ptr create (const char* string) = 0;
    
protected:
    PID_Init ();
    virtual ~PID_Init ();
};

#endif
