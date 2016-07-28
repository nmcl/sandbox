#include <os/iostream.h>

#ifndef OBJECTSTORE_OBJSTORE_H_
#  include <ObjectStore/ObjStore.h>
#endif

#ifndef IMPLEMENTATION_ARJUNA_PO_H_
#  include <Implementation/Arjuna_PO.h>
#endif

#ifndef IMPLEMENTATATION_ARJUNA_BASICPDS_H_
#  include <Implementation/Arjuna_BasicPDS.h>
#endif

#ifndef IMPLEMENTATION_ARJUNA_OBJBUFFER_H_
#  include <Implementation/Arjuna_ObjBuffer.h>
#endif

#ifndef IMPLEMENTATION_ARJUNA_PIDFACTORY_H_
#  include <Implementation/Arjuna_PIDFactory.h>
#endif

#ifndef OBJECTSTORE_LOCALOSTYPES_H_
#  include <ObjectStore/LocalOSTypes.h>
#endif

#ifdef HAVE_CH_EXTENSION
#  include <Interface/PID_Uid.ch>
#else
#  include <Interface/PID_Uid.h>
#endif

#ifndef COMMON_OBJSTATE_H_
#  include <Common/ObjState.h>
#endif

#ifndef COMMON_EXCEPTIONS_H_
#  include <Common/Exceptions.h>
#endif

#ifndef IMPLEMENTATION_PDSINVENTORY_H_
#  include <Implementation/PDSInventory.h>
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

Boolean Arjuna_BasicPDS_Init::added = 0;

Arjuna_BasicPDS_Init::Arjuna_BasicPDS_Init ()
{
    if (!Arjuna_BasicPDS_Init::added)
    {
	PDSInventory::addCreateVoid(Arjuna_BasicPDS::create, Arjuna_BasicPDS::pdsName());
	Arjuna_BasicPDS_Init::added = TRUE;
    }
}

Arjuna_BasicPDS_Init::~Arjuna_BasicPDS_Init ()
{
}


Arjuna_BasicPDS::Arjuna_BasicPDS ()
				 : _currentStatus(DISCONNECTED),
				   _objectStore(0),
				   _objectUid(0),
				   _typeName(0),
				   _theRealObject(PackablePO::_nil())
{
}

Arjuna_BasicPDS::~Arjuna_BasicPDS ()
{
    resetStore();
}

CosPersistencePDS::PDS_ptr Arjuna_BasicPDS::create ()
{
#ifdef PTR_AS_POINTER
    return new Arjuna_BasicPDS();
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

    Arjuna_BasicPDS* p = new Arjuna_BasicPDS();    
    Environment env;
    
    CORBA::Object_ptr ptr = boa->create(CORBA_RD_(CosPersistencePDS::PDS)(p), 0, 0, env);

    return ptr;
#endif
}

const char* Arjuna_BasicPDS::pdsName ()
{
    return "BasicArjunaPDS";
}

/*
 * Break connections with any existing store.
 */

void Arjuna_BasicPDS::resetStore ()
{
    if (_currentStatus != DISCONNECTED)
    {
	_currentStatus = DISCONNECTED;

	if (_objectStore)
	{
	    ObjectStore::destroy(_objectStore);
	    _objectStore = (ObjectStore*) 0;
	}

	if (_objectUid)
	{
	    delete _objectUid;
	    _objectUid = (Uid*) 0;
	}

	if (_typeName)
	{
	    ::delete [] _typeName;
	    _typeName = (char*) 0;
	}

	if (CORBA_(is_nil)(_theRealObject))
	    CORBA::release(_theRealObject);

	_theRealObject = PackablePO::_nil();
    }
}

CosPersistencePDS::PDS_ptr Arjuna_BasicPDS::connect (CORBA::Object_ptr obj, CosPersistencePID::PID_ptr pid,
						     CORBA_(Environment& env))
{
    if (_currentStatus != DISCONNECTED)
	EX_THROW(CORBA::BAD_OPERATION(ALREADY_CONNECTED, CORBA::COMPLETED_NO, "ALREADY_CONNECTED"));
    
    resetStore();

    EX_TRY
    {
	/*
	 * Implicit call to duplicate.
	 */
	
	_theRealObject = PackablePO::_narrow(obj);

	setupStore(pid);
    
	_currentStatus = CONNECTED;
	//	restore(obj, pid, env);  // restore initial state.
    }
    EX_CATCH (const BAD_PARAM&, e)
    {
	EX_THROW(e);
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

void Arjuna_BasicPDS::disconnect (CORBA::Object_ptr obj, CosPersistencePID::PID_ptr pid,
				  CORBA_(Environment& env))
{
    if (_currentStatus == DISCONNECTED)
	EX_THROW(CORBA::BAD_OPERATION(NOT_CONNECTED, CORBA::COMPLETED_NO, "NOT_CONNECTED"));
    else
    {
	if (_currentStatus != DELETED)
	{
	    store(obj, pid, env);
	    resetStore();
	}
    }
}

/*
 * Need some way to prevent restore for new (blank) objects.
 */

void Arjuna_BasicPDS::restore (CORBA::Object_ptr obj, CosPersistencePID::PID_ptr pid,
			       CORBA_(Environment& env))
{
    if (_currentStatus == DISCONNECTED)
	EX_THROW(CORBA::BAD_OPERATION(NOT_CONNECTED, CORBA::COMPLETED_NO, "NOT_CONNECTED"));

    if (!_objectStore)
	EX_THROW(CORBA::PERSIST_STORE(NO_OBJECTSTORE, CORBA::COMPLETED_NO, "NO_OBJECTSTORE"));
    
    ObjectState* oldState = _objectStore->read_committed(*_objectUid, _typeName);

    if (oldState)
    {
	EX_TRY
	{
#ifdef PTR_AS_POINTER	    
	    Arjuna_ObjectState state(oldState);  // deletes oldState in destructor
	    _theRealObject->unpackState(&state);
#else
	    /*
	     * We must have done an ORB_init by this point!
	     */

	    ORB_ptr orb = ORBInterface::orb();
	    BOA_ptr boa = ORBInterface::boa();

	    if (CORBA_(is_nil)(orb) || CORBA_(is_nil)(boa))
	    {
		cerr << "Error - orb/boa not initialised!" << endl;
		EX_THROW(CORBA::PERSIST_STORE(READ_STATE, CORBA::COMPLETED_NO, "READ_STATE"));		
	    }
	    
	    Arjuna_ObjectState state(oldState);  // deletes oldState in destructor
	    Environment env;
    
	    CORBA::Object_ptr ptr = boa->create(CORBA_RD_(ObjectBuffer)(&state), 0, 0, env);

	    _theRealObject->unpackState(ptr, env);

	    CORBA::release(ptr);
#endif
	}
	EX_CATCH (const ObjectBuffer::UnpackError&,e)
	{
	    EX_THROW(CORBA::BAD_OPERATION(OBJECT_UNPACK, CORBA::COMPLETED_MAYBE, "OBJECT_UNPACK"));
	}
    }
    else
	EX_THROW(CORBA::PERSIST_STORE(READ_STATE, CORBA::COMPLETED_NO, "READ_STATE"));
}

void Arjuna_BasicPDS::store (CORBA::Object_ptr obj, CosPersistencePID::PID_ptr pid,
			     CORBA_(Environment& env))
{
    if (_currentStatus == DISCONNECTED)
	EX_THROW(CORBA::BAD_OPERATION(NOT_CONNECTED, CORBA::COMPLETED_NO, "NOT_CONNECTED"));

    if (!_objectStore)
	EX_THROW(CORBA::PERSIST_STORE(NO_OBJECTSTORE, CORBA::COMPLETED_NO, "NO_OBJECTSTORE"));
	
    ObjectState* newState = new ObjectState(*_objectUid, _typeName);

    EX_TRY
    {
#ifdef PTR_AS_POINTER
	Arjuna_ObjectState state(newState);  // deletes newState in destructor
	_theRealObject->packState(&state);
#else
	/*
	 * We must have done an ORB_init by this point!
	 */

	ORB_ptr orb = ORBInterface::orb();
	BOA_ptr boa = ORBInterface::boa();

	if (CORBA_(is_nil)(orb) || CORBA_(is_nil)(boa))
	{
	    cerr << "Error - orb/boa not initialised!" << endl;
	    EX_THROW(CORBA::PERSIST_STORE(READ_STATE, CORBA::COMPLETED_NO, "WRITE_STATE"));
	}
	    
	Environment env;
	Arjuna_ObjectState state(newState);  // deletes newState in destructor

	CORBA::Object_ptr ptr = boa->create(CORBA_RD_(ObjectBuffer)(&state), 0, 0, env);

	_theRealObject->packState(ptr, env);
	
	CORBA::release(ptr);
#endif	
	if (!_objectStore->write_committed(*_objectUid, _typeName, *newState))
	    EX_THROW(CORBA::PERSIST_STORE(WRITE_STATE, CORBA::COMPLETED_NO, "WRITE_STATE"));
    }
    EX_CATCH (const ObjectBuffer::PackError&,e)
    {
	EX_THROW(CORBA::BAD_OPERATION(OBJECT_PACK, CORBA::COMPLETED_NO, "OBJECT_PACK"));
    }
}

void Arjuna_BasicPDS::deleteState (CORBA::Object_ptr obj, CosPersistencePID::PID_ptr pid,
				   CORBA_(Environment& env))
{
    if (_currentStatus == DISCONNECTED)
	EX_THROW(CORBA::BAD_OPERATION(NOT_CONNECTED, CORBA::COMPLETED_NO, "NOT_CONNECTED"));

    if (!_objectStore)
    {
	EX_THROW(CORBA::PERSIST_STORE(NO_OBJECTSTORE, CORBA::COMPLETED_NO, "NO_OBJECTSTORE"));
    }

    (void) _objectStore->remove_committed(*_objectUid, _typeName);

    _currentStatus = DELETED;
    
    disconnect(obj, pid, env);
}

void Arjuna_BasicPDS::setupStore (CosPersistencePID::PID_ptr pid)
{
    Environment env;
    PID_Uid_ptr PIDptr;
    Boolean result = FALSE;

    EX_TRY
    {
	PIDptr = PID_Uid::_narrow(pid);
    }
    EX_CATCH (CORBA::BAD_PARAM, e)
    {
	cerr << "Not an Arjuna PID." << endl;
	EX_THROW(e);
    }

    if (_objectStore)
    {
	ObjectStore::destroy(_objectStore);
	_objectStore = (ObjectStore*) 0;
    }
    
    char* objUid = PIDptr->uid(env);
    char* objStore = PIDptr->objectStore(env);
    _typeName = PIDptr->typeName(env);

    if (objUid && _typeName && objStore)
    {
	_objectUid = new Uid(objUid);
	_objectStore = ObjectStore::create(objStore, "");

	if (_objectStore)
	    result = TRUE;
    }

    if (objUid)
	::delete [] objUid;

    if (objStore)
	::delete [] objStore;

    if (!result)
	EX_THROW(CORBA::PERSIST_STORE(NO_OBJECTSTORE, CORBA::COMPLETED_NO, "NO_OBJECTSTORE"));
}
