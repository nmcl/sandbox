#include <os/iostream.h>

#include <Interface/CosPersistencePID.h>
#include <Interface/PIDFactory.h>

#ifndef IMPLEMENTATION_ARJUNA_PIDFACTORY_H_
#  include <Implementation/Arjuna_PIDFactory.h>
#endif

#include "PObject.h"

PersistentObject::PersistentObject ()
				   : _state(0)
{
    connect(p(env), env);
}

PersistentObject::PersistentObject (CosPersistencePID::PID_ptr pid)
				   : Arjuna_PO(pid),
				     _state(0)
{
    connect(pid, env);
}
    
PersistentObject::~PersistentObject ()
{
    disconnect(CosPersistencePID::PID::_nil(), env);
}

void PersistentObject::set (long val)
{
    _state = val;
    store(CosPersistencePID::PID::_nil(), env);
}

void PersistentObject::get (long& val)
{
    restore(CosPersistencePID::PID::_nil(), env);
    val = _state;
}

void PersistentObject::unpackState (ObjectBuffer_ptr os, CORBA::Environment& e)
{
    os->unpackLong(_state, e);
}

void PersistentObject::packState (ObjectBuffer_ptr os, CORBA::Environment& e)
{
    os->packLong(_state, e);
}

char* PersistentObject::type (CORBA::Environment& e)
{
    return "/Arjuna_PO/PersistentObject";
}
