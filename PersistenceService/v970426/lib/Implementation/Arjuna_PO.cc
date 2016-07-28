#include <os/iostream.h>

#ifndef COMMON_BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef IMPLEMENTATION_ARJUNA_PO_H_
#  include <Implementation/Arjuna_PO.h>
#endif

#ifndef IMPLEMENTATION_ARJUNA_POM_H_
#  include <Implementation/Arjuna_POM.h>
#endif

#ifndef IMPLEMENTATION_ARJUNA_BASICPID_H_
#  include <Implementation/Arjuna_BasicPID.h>
#endif

#ifndef IMPLEMENTATION_ARJUNA_PIDFACTORY_H_
#  include <Implementation/Arjuna_PIDFactory.h>
#endif

#ifdef HAVE_CH_EXTENSION
#  include <Interface/CosPersistencePID.ch>
#else
#  include <Interface/CosPersistencePID.h>
#endif

#ifndef COMMON_EXCEPTIONS_H_
#  include <Common/Exceptions.h>
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

Arjuna_PO::Arjuna_PO ()
		     : _protocolManager(0),
		       _myPID(CosPersistencePID::PID::_nil()),
		       _theProtocol(CosPersistencePDS::PDS::_nil()),
		       _isNew(TRUE)
{
    init();
}

Arjuna_PO::Arjuna_PO (CosPersistencePID::PID_ptr pid)
		     : _protocolManager(0),
		       _myPID(CosPersistencePID::PID::_duplicate(pid)),
		       _theProtocol(CosPersistencePDS::PDS::_nil()),
		       _isNew(FALSE)
{
    init();
}

Arjuna_PO::~Arjuna_PO ()
{
    if (!CORBA_(is_nil)(_protocolManager))
#ifdef PTR_AS_POINTER    
	delete _protocolManager;
#else
        CORBA::release(_protocolManager);
#endif    

    if (CORBA_(is_nil)(_myPID))
	CORBA::release(_myPID);

    if (CORBA_(is_nil)(_myPID))
	CORBA::release(_theProtocol);
}

void Arjuna_PO::init ()
{
#ifdef PTR_AS_POINTER    
    _protocolManager = new Arjuna_POM;
#else
    /*
     * We must have done an ORB_init by this point!
     */

    ORB_ptr orb = ORBInterface::orb();
    BOA_ptr boa = ORBInterface::boa();

    if (CORBA_(is_nil)(orb) || CORBA_(is_nil)(boa))
	cerr << "Error - orb/boa not initialised!" << endl;
    else
    {
	Environment env;

	_protocolManager = boa->create(CORBA_RD_(CosPersistencePOM::POM)(new Arjuna_POM), 0, 0, env);
    }
#endif    
}

void Arjuna_PO::createNewPid ()
{
    /*
     * If we need to create a new pid, create a basic one.
     * Later we can give ways for users to specify the default
     * implementation they want, e.g., replicated.
     */

    if (CORBA_(is_nil)(_myPID))
    {
	CORBA_(Environment env);

	const char* typeName = type(env);
	Uid u;

#ifdef PTR_AS_POINTER	
	_myPID = new Arjuna_BasicPID(NULL, typeName, u);
#else
	/*
	 * We must have done an ORB_init by this point!
	 */

	ORB_ptr orb = ORBInterface::orb();
	BOA_ptr boa = ORBInterface::boa();

	if (CORBA_(is_nil)(orb) || CORBA_(is_nil)(boa))
	    cerr << "Error - orb/boa not initialised!" << endl;
	else
	{
	    Environment env;

	    _myPID = boa->create(CORBA_RD_(CosPersistencePID::PID)(new Arjuna_BasicPID(NULL, typeName, u)), 0, 0, env);
	}
#endif	
    }
}

CosPersistencePID::PID_ptr Arjuna_PO::p (CORBA_(Environment& env))
{
    return _myPID;
}

/*
 * Ignore after connect.
 */

void Arjuna_PO::p (CosPersistencePID::PID_ptr pid, CORBA_(Environment& env))
{
    if (CORBA_(is_nil)(_theProtocol))
	EX_THROW(CORBA::BAD_OPERATION(IS_CONNECTED, CORBA::COMPLETED_NO, "IS_CONNECTED"));
    
    CORBA::release(_myPID);
    _myPID = CosPersistencePID::PID::_duplicate(pid);
    _isNew = FALSE;
}

/*
 * Check we are connected?
 */

CosPersistencePDS::PDS_ptr Arjuna_PO::connect (CosPersistencePID::PID_ptr pid, CORBA_(Environment& env))
{
    if (CORBA_(is_nil)(_theProtocol))
	EX_THROW(CORBA::BAD_OPERATION(IS_CONNECTED, CORBA::COMPLETED_NO, "IS_CONNECTED"));

    if (CORBA_(is_nil)(_myPID))
    {
	if (CORBA_(is_nil)(pid))
	{
	    CORBA::release(_myPID);
	    _myPID = CosPersistencePID::PID::_duplicate(pid);
	}
	else
	    createNewPid();
    }

    
#ifdef PTR_AS_POINTER
    _theProtocol = CosPersistencePDS::PDS::_duplicate(_protocolManager->connect(this, _myPID, env));
#else
    /*
     * We must have done an ORB_init by this point!
     */

    ORB_ptr orb = ORBInterface::orb();
    BOA_ptr boa = ORBInterface::boa();

    if (CORBA_(is_nil)(orb) || CORBA_(is_nil)(boa))
	cerr << "Error - orb/boa not initialised!" << endl;
    else
    {
	Environment env;

	CORBA::Object_ptr ptr = boa->create(CORBA_RD_(CosPersistencePO::PO)(this), 0, 0, env);

	_theProtocol = CosPersistencePDS::PDS::_duplicate(_protocolManager->connect(ptr, _myPID, env));
    }
#endif

    return _theProtocol;
}

void Arjuna_PO::disconnect (CosPersistencePID::PID_ptr pid, CORBA_(Environment& env))
{
    if (CORBA_(is_nil)(_theProtocol))
	EX_THROW(CORBA::BAD_OPERATION(IS_CONNECTED, CORBA::COMPLETED_NO, "IS_CONNECTED"));

#ifdef PTR_AS_POINTER    
    _protocolManager->disconnect(this, _myPID, env);
#else
    /*
     * We must have done an ORB_init by this point!
     */

    ORB_ptr orb = ORBInterface::orb();
    BOA_ptr boa = ORBInterface::boa();

    if (CORBA_(is_nil)(orb) || CORBA_(is_nil)(boa))
	cerr << "Error - orb/boa not initialised!" << endl;
    else
    {
	Environment env;

	CORBA::Object_ptr ptr = boa->create(CORBA_RD_(CosPersistencePO::PO)(this), 0, 0, env);

	_protocolManager->disconnect(ptr, _myPID, env);
    }
#endif    
    
    CORBA::release(_myPID);
    _myPID = CosPersistencePID::PID::_nil();
    
    CORBA::release(_theProtocol);
    _theProtocol = CosPersistencePDS::PDS::_nil();
}

void Arjuna_PO::restore (CosPersistencePID::PID_ptr pid, CORBA_(Environment& env))
{
#ifdef PTR_AS_POINTER
    if (!_isNew)
	_protocolManager->restore(this, _myPID, env);
#else
    if (!_isNew)
    {
	/*
	 * We must have done an ORB_init by this point!
	 */

	ORB_ptr orb = ORBInterface::orb();
	BOA_ptr boa = ORBInterface::boa();

	if (CORBA_(is_nil)(orb) || CORBA_(is_nil)(boa))
	    cerr << "Error - orb/boa not initialised!" << endl;
	else
	{
	    Environment env;

	    CORBA::Object_ptr ptr = boa->create(CORBA_RD_(CosPersistencePO::PO)(this), 0, 0, env);

	    _protocolManager->restore(ptr, _myPID, env);
	}
    }
#endif    
}

void Arjuna_PO::store (CosPersistencePID::PID_ptr pid, CORBA_(Environment& env))
{
#ifdef PTR_AS_POINTER    
    _protocolManager->store(this, _myPID, env);
#else
    /*
     * We must have done an ORB_init by this point!
     */

    ORB_ptr orb = ORBInterface::orb();
    BOA_ptr boa = ORBInterface::boa();

    if (CORBA_(is_nil)(orb) || CORBA_(is_nil)(boa))
	cerr << "Error - orb/boa not initialised!" << endl;
    else
    {
	Environment env;

	CORBA::Object_ptr ptr = boa->create(CORBA_RD_(CosPersistencePO::PO)(this), 0, 0, env);

	_protocolManager->store(ptr, _myPID, env);
    }
#endif
	
    _isNew = FALSE;
}

void Arjuna_PO::deleteState (CosPersistencePID::PID_ptr pid, CORBA_(Environment& env))
{
#ifdef PTR_AS_POINTER    
    _protocolManager->deleteState(this, _myPID, env);
#else
    /*
     * We must have done an ORB_init by this point!
     */

    ORB_ptr orb = ORBInterface::orb();
    BOA_ptr boa = ORBInterface::boa();

    if (CORBA_(is_nil)(orb) || CORBA_(is_nil)(boa))
	cerr << "Error - orb/boa not initialised!" << endl;
    else
    {
	Environment env;

	CORBA::Object_ptr ptr = boa->create(CORBA_RD_(CosPersistencePO::PO)(this), 0, 0, env);

	_protocolManager->deleteState(ptr, _myPID, env);
    }
#endif    
}

void Arjuna_PO::unpackState (ObjectBuffer_ptr os, CORBA_(Environment& env))
{
    EX_THROW(ObjectBuffer::UnpackError());
}

void Arjuna_PO::packState (ObjectBuffer_ptr os, CORBA_(Environment& env))
{
    EX_THROW(ObjectBuffer::PackError());
}

char* Arjuna_PO::type (CORBA_(Environment& env))
{
    return "/Arjuna_PO/NoTypeName";
}
