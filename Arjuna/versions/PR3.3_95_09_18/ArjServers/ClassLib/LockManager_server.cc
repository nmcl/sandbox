/*
 * Automatically Generated by:
 * Arjuna Stub Generator Version 5.5.3 - $Date: 1996/09/10 15:38:44 $
 *
 * Do Not Edit By Hand
 *
 */

#ifdef STUB
#  pragma @NoRename
#endif

#include <StubGen/ServerRpc.h>

#define LOCKMANAGER_SERVER

#include "LockMan_stub.h"

ServerLockManager::ServerLockManager () : theRealObject(0),_constructArgs_(),_constructOpcode_(0),_constructRefCount_(0),_constructResult_()
{}

ServerLockManager::~ServerLockManager ()
{
}

void ServerLockManager::serve ( int argc, char **argv )
{
ServerRpc ServerRPC;
Int32 opcode, errcode;

ServerRPC.initialise(argc, argv);

for (;;)
{
RpcBuffer work;
RpcBuffer result;

ServerRPC.getWork(opcode, work);
errcode = dispatchToClass(theRealObject, opcode, work, result);
ServerRPC.sendResult(errcode, result);
}
}

Int32 ServerLockManager::dispatchToClass ( LocalLockManager * theObject, Int32 funcCode, RpcBuffer& work, RpcBuffer& result)
{
Int32 classCode;

work >> classCode;

switch (classCode)
{
case -1:
return dispatchToOper(theObject, funcCode, work, result);
case 116324194:
return ServerStateManager::dispatchToClass(theObject, funcCode, work, result);
default:
return DISPATCH_ERROR;
}
}

Int32 ServerLockManager::dispatchToOper ( LocalLockManager * theObject, Int32 funcCode, RpcBuffer& work, RpcBuffer& result)
{
switch (funcCode)
{
case 186814694:
return type186814694(theObject, work, result);
default:
return DISPATCH_ERROR;
}
}



Int32 ServerLockManager::type186814694 (LocalLockManager * _theObject_, RpcBuffer& _work_, RpcBuffer& _result_)
{
RpcBuffer _rvBuffer_;
Int32 _errCode_ = OPER_INVOKED_OK;

const TypeName _returnedValue_ = _theObject_->type();
_rvBuffer_ << _returnedValue_;
if (_rvBuffer_.length() > 0)
_result_ << _rvBuffer_;
return _errCode_;
}


