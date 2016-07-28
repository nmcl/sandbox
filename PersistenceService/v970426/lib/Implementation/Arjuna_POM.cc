#ifndef COMMON_EXCEPTIONS_H_
#  include <Common/Exceptions.h>
#endif

#ifndef IMPLEMENTATION_ARJUNA_POM_H_
#  include <Implementation/Arjuna_POM.h>
#endif

#ifndef IMPLEMENTATION_ARJUNA_PDSFACTORY_H_
#  include <Implementation/Arjuna_PDSFactory.h>
#endif

#ifndef COMMON_ORBINTERFACE_H_
#  include <Common/ORBInterface.h>
#endif

#ifndef COMMON_COSPERSISNAMES_H_
#  include <Common/CosPersisNames.h>
#endif

#ifndef COMMON_EXCEPT_H_
#  include <Common/except.h>
#endif

Arjuna_POM::Arjuna_POM ()
		       : _protocolService(CosPersistencePDS::PDS::_nil())
{
}

Arjuna_POM::~Arjuna_POM ()
{
    if (CORBA_(is_nil)(_protocolService))
	CORBA::release(_protocolService);
}

CosPersistencePDS::PDS_ptr Arjuna_POM::connect (CORBA::Object_ptr obj, CosPersistencePID::PID_ptr pid, CORBA_(Environment& env))
{
    if (CORBA_(is_nil)(_protocolService))
	EX_THROW(CORBA::BAD_OPERATION(ALREADY_CONNECTED, CORBA::COMPLETED_NO, "ALREADY_CONNECTED"));
    else
    {
	_protocolService = Arjuna_PDSFactory::createFromPID(pid);
	return _protocolService->connect(obj, pid, env);
    }
}

void Arjuna_POM::disconnect (CORBA::Object_ptr obj, CosPersistencePID::PID_ptr pid, CORBA_(Environment& env))
{
    if (CORBA_(is_nil)(_protocolService))
	EX_THROW(CORBA::BAD_OPERATION(NOT_CONNECTED, CORBA::COMPLETED_NO, "NOT_CONNECTED"));
    else
    {
	_protocolService->disconnect(obj, pid, env);
	CORBA::release(_protocolService);
	_protocolService = CosPersistencePDS::PDS::_nil();
    }
}

void Arjuna_POM::restore (CORBA::Object_ptr obj, CosPersistencePID::PID_ptr pid, CORBA_(Environment& env))
{
    if (CORBA_(is_nil)(_protocolService))
	EX_THROW(CORBA::BAD_OPERATION(NOT_CONNECTED, CORBA::COMPLETED_NO, "NOT_CONNECTED"));
    else
	_protocolService->restore(obj, pid, env);
}

void Arjuna_POM::store (CORBA::Object_ptr obj, CosPersistencePID::PID_ptr pid, CORBA_(Environment& env))
{
    if (CORBA_(is_nil)(_protocolService))
	EX_THROW(CORBA::BAD_OPERATION(NOT_CONNECTED, CORBA::COMPLETED_NO, "NOT_CONNECTED"));
    else
	_protocolService->store(obj, pid, env);
}

void Arjuna_POM::deleteState (CORBA::Object_ptr obj, CosPersistencePID::PID_ptr pid, CORBA_(Environment& env))
{
    if (CORBA_(is_nil)(_protocolService))
	EX_THROW(CORBA::BAD_OPERATION(NOT_CONNECTED, CORBA::COMPLETED_NO, "NOT_CONNECTED"));
    else    
	_protocolService->deleteState(obj, pid, env);
}
