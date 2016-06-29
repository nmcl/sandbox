/*
 * Automatically Generated by:
 * Arjuna Stub Generator Version 6.0.0 - $Date: 1997/06/11 20:25:11 $
 *
 * Do Not Edit By Hand
 */

#ifdef STUB
#  pragma @NoRename
#endif

#include <StubGen/ServerRpc.h>

#define BUFFERCHECK_SERVER

#include "BufferCheck_stub.h"

ServerBufferCheck::ServerBufferCheck () : theRealObject(0),_constructArgs_(),_constructOpcode_(0),_constructRefCount_(0),_constructResult_()
{}

ServerBufferCheck::~ServerBufferCheck ()
{
}

void ServerBufferCheck::serve ( int argc, char **argv )
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

Int32 ServerBufferCheck::dispatchToClass ( LocalBufferCheck * theObject, Int32 funcCode, RpcBuffer& work, RpcBuffer& result)
{
Int32 classCode;

work >> classCode;

switch (classCode)
{
case -1:
return dispatchToOper(theObject, funcCode, work, result);
case 65:
return ServerA::dispatchToClass(theObject, funcCode, work, result);
default:
return DISPATCH_ERROR;
}
}

Int32 ServerBufferCheck::dispatchToOper ( LocalBufferCheck * theObject, Int32 funcCode, RpcBuffer& work, RpcBuffer& result)
{
switch (funcCode)
{
case 258928217:
return BufferCheck258928217(theObject, work, result);
case 84735718:
return BufferCheck84735718(theObject, work, result);
case 96138770:
return Test96138770(theObject, work, result);
default:
return DISPATCH_ERROR;
}
}

Int32 ServerBufferCheck::BufferCheck258928217 (LocalBufferCheck * _theObject_, RpcBuffer& _work_, RpcBuffer& _result_)
{
Int32 _errCode_ = OPER_INVOKED_OK;
int _constructedHere_ = 0;
RpcBuffer _savedArgs_ = _work_;
int* Par_0_ = 0;
_work_ >> Par_0_;
int* Par_1_ = 0;
_work_ >> Par_1_;
if (_work_.readState() == BUFFER_GOOD)
{
if ((_constructRefCount_ > 0) && ((_constructOpcode_ != 258928217) || (_constructArgs_ != _savedArgs_)))
_errCode_ = ALREADY_CONSTRUCTED;
else
{
if (_constructRefCount_ > 0)
{
_result_ = _constructResult_;
}
else
{
_constructedHere_ = 1;
theRealObject = new LocalBufferCheck(*Par_0_, *Par_1_);
}
}
if (_constructedHere_)
{
_result_ << Par_0_;
_result_ << Par_1_;
}
if (_errCode_ == OPER_INVOKED_OK)
{
_constructRefCount_++;
if (_constructRefCount_ == 1)
{
_constructResult_ = _result_;
_constructArgs_ = _savedArgs_;
_constructOpcode_ = 258928217;
}
}
}
else
_errCode_ = PARAMETER_ERROR;

_work_.rewrite();

if (_work_.shouldPack(Par_0_))
delete Par_0_;
if (_work_.shouldPack(Par_1_))
delete Par_1_;
return _errCode_;
}


Int32 ServerBufferCheck::BufferCheck84735718 (LocalBufferCheck * _theObject_, RpcBuffer& _work_, RpcBuffer& _result_)
{
Int32 _errCode_ = OPER_INVOKED_OK;

if (--_constructRefCount_ == 0)
{
delete _theObject_;
_theObject_ = 0;
theRealObject = 0;
_constructOpcode_ = 0;
}

return _errCode_;
}


Int32 ServerBufferCheck::Test96138770 (LocalBufferCheck * _theObject_, RpcBuffer& _work_, RpcBuffer& _result_)
{
RpcBuffer _rvBuffer_;
Int32 _errCode_ = OPER_INVOKED_OK;
Buffer Par_0_;
_work_ >> Par_0_;
Buffer* Par_1_ = 0;
_work_ >> Par_1_;
if (_work_.readState() == BUFFER_GOOD)
{
int _returnedValue_ = _theObject_->Test(Par_0_, *Par_1_);
_rvBuffer_ << _returnedValue_;
_result_ << Par_1_;
if (_rvBuffer_.length() > 0)
_result_ << _rvBuffer_;
}
else
_errCode_ = PARAMETER_ERROR;

_work_.rewrite();

if (_work_.shouldPack(Par_1_))
delete Par_1_;
return _errCode_;
}


