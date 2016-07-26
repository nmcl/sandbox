#ifndef POBJECT_H_
#define POBJECT_H_

#include <Implementation/Arjuna_PO.h>

/*
 * Add methods for replication, remote state, and which
 * default persistence mechanism to use.
 */

class PersistentObject : public Arjuna_PO
{
public:
    PersistentObject ();
    PersistentObject (CosPersistencePID::PID_ptr pid);    
    virtual ~PersistentObject ();

    void set (long val);
    void get (long& val);

    void unpackState (ObjectBuffer_ptr os, CORBA::Environment& env);
    void packState   (ObjectBuffer_ptr os, CORBA::Environment& env);

    char* type (CORBA::Environment& env);

private:
    long _state;
    CORBA::Environment env;
};

#endif
