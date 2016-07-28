#include <os/iostream.h>
#include <os/stdlib.h>

#ifndef COMMON_EXCEPTIONS_H_
#  include <Common/Exceptions.h>
#endif

#ifdef HAVE_CH_EXTENSION
#  include <Interface/PID_RemoteService.ch>
#  include <Interface/CosPersistencePOM.ch>
#else
#  include <Interface/PID_RemoteService.h>
#  include <Interface/CosPersistencePOM.h>
#endif

#ifndef IMPLEMENTATION_PDSINVENTORY_H_
#  include <Implementation/PDSInventory.h>
#endif

#ifndef IMPLEMENTATION_ARJUNA_REMOTEPDS_H_
#  include <Implementation/Arjuna_RemotePDS.h>
#endif

#ifndef IMPLEMENTATION_ARJUNA_PIDFACTORY_H_
#  include <Implementation/Arjuna_PIDFactory.h>
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

Boolean Arjuna_RemotePDS_Init::added = 0;

Arjuna_RemotePDS_Init::Arjuna_RemotePDS_Init ()
{
    if (!Arjuna_RemotePDS_Init::added)
    {
	PDSInventory::addCreateVoid(Arjuna_RemotePDS::create, Arjuna_RemotePDS::pdsName());
	Arjuna_RemotePDS_Init::added = TRUE;
    }
}

Arjuna_RemotePDS_Init::~Arjuna_RemotePDS_Init ()
{
}

Arjuna_RemotePDS::Arjuna_RemotePDS ()
				   : _currentStatus(DISCONNECTED),
				     _theRealObjectStore(CosPersistencePDS::PDS::_nil()),
				     _thePDSFactory(PDSFactory::_nil()),
				     _theRemotePID(CosPersistencePID::PID::_nil())
{
}

Arjuna_RemotePDS::~Arjuna_RemotePDS ()
{
    if (CORBA_(is_nil)(_theRealObjectStore))
	CORBA::release(_theRealObjectStore);

    if (CORBA_(is_nil)(_thePDSFactory))
	CORBA::release(_thePDSFactory);

    if (CORBA_(is_nil)(_theRemotePID))
	CORBA::release(_theRemotePID);
}

CosPersistencePDS::PDS_ptr Arjuna_RemotePDS::create ()
{
#ifdef PTR_AS_POINTER    
    return new Arjuna_RemotePDS();
#else
    /*
     * We must have done an ORB_init by this point!
     */

    ORB_ptr orb = ORBInterface::orb();
    BOA_ptr boa = ORBInterface::boa();

    if (CORBA_(is_nil)(orb) || CORBA_(is_nil)(boa))
    {
	cerr << "Error - orb/boa not initialised!" << endl;
	return PDS::_nil();
    }

    Arjuna_RemotePDS* p = new Arjuna_RemotePDS();
    Environment env;
    
    CORBA::Object_ptr ptr = boa->create(CORBA_RD_(CosPersistencePDS::PDS)(p), 0, 0, env);

    return ptr;
#endif
}

const char* Arjuna_RemotePDS::pdsName ()
{
    return "RemoteArjunaPDS";
}

void Arjuna_RemotePDS::resetStore ()
{
    if (_currentStatus != DISCONNECTED)
    {
	_currentStatus = DISCONNECTED;

	CORBA::release(_theRealObjectStore);
	_theRealObjectStore = CosPersistencePDS::PDS::_nil();

	CORBA::release(_thePDSFactory);
	_thePDSFactory = PDSFactory::_nil();

	CORBA::release(_theRemotePID);
	_theRemotePID = CosPersistencePID::PID::_nil();
    }
}

CosPersistencePDS::PDS_ptr Arjuna_RemotePDS::connect (CORBA::Object_ptr obj, CosPersistencePID::PID_ptr pid,
						      CORBA_(Environment& env))
{
    if (_currentStatus != DISCONNECTED)
	EX_THROW(CORBA::BAD_OPERATION(ALREADY_CONNECTED, CORBA::COMPLETED_NO, "ALREADY_CONNECTED"));
    
    resetStore();

    EX_TRY
    {
	setupStore(pid);

	_theRealObjectStore->connect(obj, _theRemotePID, env);

	_currentStatus = CONNECTED;
    }
    EX_CATCH (const BAD_PARAM&,e)
    {
	EX_THROW(e);
    }
    EX_CATCH (INV_OBJREF,e)
    {
	cerr << &exEnv << endl;
	EX_THROW(exEnv);
    }

#ifdef PTR_AS_POINTER
    return this;
#else
    /*
     * We must have done an ORB_init by this point!
     */

    ORB_ptr orb = ORBInterface::orb();
    BOA_ptr boa = ORBInterface::boa();

    if (CORBA_(is_nil)(orb) || CORBA_(is_nil)(boa))
    {
	cerr << "Error - orb/boa not initialised!" << endl;
	return PDS::_nil();
    }

    Environment dummyEnv;
    
    CORBA::Object_ptr ptr = boa->create(CORBA_RD_(CosPersistencePDS::PDS)(this), 0, 0, dummyEnv);

    return ptr;
#endif        
}

void Arjuna_RemotePDS::disconnect (CORBA::Object_ptr obj, CosPersistencePID::PID_ptr pid, CORBA_(Environment& env))
{
    if (_currentStatus == DISCONNECTED)
	EX_THROW(CORBA::BAD_OPERATION(NOT_CONNECTED, CORBA::COMPLETED_NO, "NOT_CONNECTED"));

    _theRealObjectStore->disconnect(obj, _theRemotePID, env);
    _currentStatus = DISCONNECTED;
}

/*
 * Need some way to prevent restore for new (blank) objects.
 */

void Arjuna_RemotePDS::restore (CORBA::Object_ptr obj, CosPersistencePID::PID_ptr pid, CORBA_(Environment& env))
{
    if (_currentStatus == DISCONNECTED)
	EX_THROW(CORBA::BAD_OPERATION(NOT_CONNECTED, CORBA::COMPLETED_NO, "NOT_CONNECTED"));

    _theRealObjectStore->restore(obj, _theRemotePID, env);
}

void Arjuna_RemotePDS::store (CORBA::Object_ptr obj, CosPersistencePID::PID_ptr pid, CORBA_(Environment& env))
{
    if (_currentStatus == DISCONNECTED)
	EX_THROW(CORBA::BAD_OPERATION(NOT_CONNECTED, CORBA::COMPLETED_NO, "NOT_CONNECTED"));

    _theRealObjectStore->store(obj, _theRemotePID, env);
}

void Arjuna_RemotePDS::deleteState (CORBA::Object_ptr obj, CosPersistencePID::PID_ptr pid, CORBA_(Environment& env))
{
    if (_currentStatus == DISCONNECTED)
	EX_THROW(CORBA::BAD_OPERATION(NOT_CONNECTED, CORBA::COMPLETED_NO, "NOT_CONNECTED"));

    _theRealObjectStore->deleteState(obj, _theRemotePID, env);
}

/*
 * At present we create a direct link to some other non-remote PDS in a
 * server. Therefore we keep the local pid here and just ship that
 * back to the server instead of the (remote) pid we're given at each
 * operation. If the remote PDS was more intelligent we could always
 * ship the actual pid, and let the PDS figure it out.
 */

void Arjuna_RemotePDS::setupStore (CosPersistencePID::PID_ptr pid)
{
    CORBA_(Environment env);
    PID_RemoteService_ptr PIDptr;
    Boolean result = FALSE;

    /*
     * This should not happen if we are called by Arjuna_POM, since
     * the PDSFactory should have already seen the pid.
     */
    
    EX_TRY
    {
	PIDptr = PID_RemoteService::_narrow(pid);
    }
    EX_CATCH (CORBA::BAD_PARAM,e)
    {
	cerr << "Not a PID_RemoteService." << endl;
	EX_THROW(e);
    }

    char* hostName = PIDptr->hostName(env);
    char* pidString = PIDptr->pidString(env);

    if (hostName && pidString)
    {
	_theRemotePID = Arjuna_PIDFactory::createFromString(pidString);
	
	EX_TRY
	{
#ifdef ORBIX	    
	    _thePDSFactory = PDSFactory::_bind("foo:PersistenceServer", hostName);

	    _theRealObjectStore = _thePDSFactory->create_from_pidstring(pidString, env);
	    result = TRUE;
#else
	    result = FALSE;
#endif	    
	}
	EX_CATCH (CORBA::SystemException, e)
	{
	    cerr << "Bind to remote factory failed." << endl;
	    cerr << &exEnv << endl;

	    CORBA::release(_thePDSFactory);	    
	    exit(1);
	}
    }

    if (hostName)
	::delete [] hostName;

    if (pidString)
	::delete [] pidString;

    if (!result)
	EX_THROW(CORBA::PERSIST_STORE(NO_OBJECTSTORE, CORBA::COMPLETED_NO, "NO_OBJECTSTORE"));
}
