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

#define STUDENTRECORD_SERVER

#include "StudentRecord_stub.h"

ServerStudentRecord::ServerStudentRecord () : theRealObject(0),_constructArgs_(),_constructOpcode_(0),_constructRefCount_(0),_constructResult_()
{}

ServerStudentRecord::~ServerStudentRecord ()
{
}

void ServerStudentRecord::serve ( int argc, char **argv )
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

Int32 ServerStudentRecord::dispatchToClass ( LocalStudentRecord * theObject, Int32 funcCode, RpcBuffer& work, RpcBuffer& result)
{
Int32 classCode;

work >> classCode;

switch (classCode)
{
case -1:
return dispatchToOper(theObject, funcCode, work, result);
case 53946306:
return ServerLockManager::dispatchToClass(theObject, funcCode, work, result);
default:
return DISPATCH_ERROR;
}
}

Int32 ServerStudentRecord::dispatchToOper ( LocalStudentRecord * theObject, Int32 funcCode, RpcBuffer& work, RpcBuffer& result)
{
switch (funcCode)
{
case 257251797:
return StudentRecord257251797(theObject, work, result);
case 223600228:
return StudentRecord223600228(theObject, work, result);
case 195690166:
return StudentRecord195690166(theObject, work, result);
case 31187698:
return storeRecord31187698(theObject, work, result);
case 80055922:
return fetchRecord80055922(theObject, work, result);
case 212180354:
return fetchExclusiveRecord212180354(theObject, work, result);
case 33872498:
return replaceRecord33872498(theObject, work, result);
case 140900197:
return resetRecord140900197(theObject, work, result);
case 45858054:
return hasRemoteState45858054(theObject, work, result);
case 234554646:
return useStoreLocation234554646(theObject, work, result);
case 120944197:
return save_state120944197(theObject, work, result);
case 169823749:
return restore_state169823749(theObject, work, result);
case 110749670:
return type110749670(theObject, work, result);
default:
return DISPATCH_ERROR;
}
}

Int32 ServerStudentRecord::StudentRecord257251797 (LocalStudentRecord * _theObject_, RpcBuffer& _work_, RpcBuffer& _result_)
{
Int32 _errCode_ = OPER_INVOKED_OK;
int _constructedHere_ = 0;
RpcBuffer _savedArgs_ = _work_;
SOutCome* res = 0;
_work_ >> res;
if (_work_.readState() == BUFFER_GOOD)
{
if ((_constructRefCount_ > 0) && ((_constructOpcode_ != 257251797) || (_constructArgs_ != _savedArgs_)))
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
theRealObject = new LocalStudentRecord(*res);
}
}
if (_constructedHere_)
{
_result_ << res;
}
if (_errCode_ == OPER_INVOKED_OK)
{
_constructRefCount_++;
if (_constructRefCount_ == 1)
{
_constructResult_ = _result_;
_constructArgs_ = _savedArgs_;
_constructOpcode_ = 257251797;
}
}
}
else
_errCode_ = PARAMETER_ERROR;

_work_.rewrite();

if(_work_.shouldPack(res))
delete res;
return _errCode_;
}


Int32 ServerStudentRecord::StudentRecord223600228 (LocalStudentRecord * _theObject_, RpcBuffer& _work_, RpcBuffer& _result_)
{
Int32 _errCode_ = OPER_INVOKED_OK;
int _constructedHere_ = 0;
RpcBuffer _savedArgs_ = _work_;
SOutCome* res = 0;
_work_ >> res;
Uid* myUid = 0;
_work_ >> myUid;
if (_work_.readState() == BUFFER_GOOD)
{
if ((_constructRefCount_ > 0) && ((_constructOpcode_ != 223600228) || (_constructArgs_ != _savedArgs_)))
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
theRealObject = new LocalStudentRecord(*res, *myUid);
}
}
if (_constructedHere_)
{
_result_ << res;
}
if (_errCode_ == OPER_INVOKED_OK)
{
_constructRefCount_++;
if (_constructRefCount_ == 1)
{
_constructResult_ = _result_;
_constructArgs_ = _savedArgs_;
_constructOpcode_ = 223600228;
}
}
}
else
_errCode_ = PARAMETER_ERROR;

_work_.rewrite();

if(_work_.shouldPack(res))
delete res;
if(_work_.shouldPack(myUid))
delete myUid;
return _errCode_;
}


Int32 ServerStudentRecord::StudentRecord195690166 (LocalStudentRecord * _theObject_, RpcBuffer& _work_, RpcBuffer& _result_)
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


Int32 ServerStudentRecord::storeRecord31187698 (LocalStudentRecord * _theObject_, RpcBuffer& _work_, RpcBuffer& _result_)
{
Int32 _errCode_ = OPER_INVOKED_OK;
SOutCome* res = 0;
_work_ >> res;
Buffer toStore;
_work_ >> toStore;
if (_work_.readState() == BUFFER_GOOD)
{
_theObject_->storeRecord(*res, toStore);
_result_ << res;
}
else
_errCode_ = PARAMETER_ERROR;

_work_.rewrite();

if(_work_.shouldPack(res))
delete res;
return _errCode_;
}


Int32 ServerStudentRecord::fetchRecord80055922 (LocalStudentRecord * _theObject_, RpcBuffer& _work_, RpcBuffer& _result_)
{
Int32 _errCode_ = OPER_INVOKED_OK;
SOutCome* res = 0;
_work_ >> res;
Buffer* toRetrieve = 0;
_work_ >> toRetrieve;
if (_work_.readState() == BUFFER_GOOD)
{
_theObject_->fetchRecord(*res, *toRetrieve);
_result_ << res;
_result_ << toRetrieve;
}
else
_errCode_ = PARAMETER_ERROR;

_work_.rewrite();

if(_work_.shouldPack(res))
delete res;
if(_work_.shouldPack(toRetrieve))
delete toRetrieve;
return _errCode_;
}


Int32 ServerStudentRecord::fetchExclusiveRecord212180354 (LocalStudentRecord * _theObject_, RpcBuffer& _work_, RpcBuffer& _result_)
{
Int32 _errCode_ = OPER_INVOKED_OK;
SOutCome* res = 0;
_work_ >> res;
Buffer* toRetrieve = 0;
_work_ >> toRetrieve;
if (_work_.readState() == BUFFER_GOOD)
{
_theObject_->fetchExclusiveRecord(*res, *toRetrieve);
_result_ << res;
_result_ << toRetrieve;
}
else
_errCode_ = PARAMETER_ERROR;

_work_.rewrite();

if(_work_.shouldPack(res))
delete res;
if(_work_.shouldPack(toRetrieve))
delete toRetrieve;
return _errCode_;
}


Int32 ServerStudentRecord::replaceRecord33872498 (LocalStudentRecord * _theObject_, RpcBuffer& _work_, RpcBuffer& _result_)
{
Int32 _errCode_ = OPER_INVOKED_OK;
SOutCome* res = 0;
_work_ >> res;
Buffer replaceWith;
_work_ >> replaceWith;
if (_work_.readState() == BUFFER_GOOD)
{
_theObject_->replaceRecord(*res, replaceWith);
_result_ << res;
}
else
_errCode_ = PARAMETER_ERROR;

_work_.rewrite();

if(_work_.shouldPack(res))
delete res;
return _errCode_;
}


Int32 ServerStudentRecord::resetRecord140900197 (LocalStudentRecord * _theObject_, RpcBuffer& _work_, RpcBuffer& _result_)
{
Int32 _errCode_ = OPER_INVOKED_OK;
SOutCome* res = 0;
_work_ >> res;
if (_work_.readState() == BUFFER_GOOD)
{
_theObject_->resetRecord(*res);
_result_ << res;
}
else
_errCode_ = PARAMETER_ERROR;

_work_.rewrite();

if(_work_.shouldPack(res))
delete res;
return _errCode_;
}


Int32 ServerStudentRecord::hasRemoteState45858054 (LocalStudentRecord * _theObject_, RpcBuffer& _work_, RpcBuffer& _result_)
{
RpcBuffer _rvBuffer_;
Int32 _errCode_ = OPER_INVOKED_OK;

Boolean _returnedValue_ = _theObject_->hasRemoteState();
_rvBuffer_ << _returnedValue_;
if (_rvBuffer_.length() > 0)
_result_ << _rvBuffer_;
return _errCode_;
}


Int32 ServerStudentRecord::useStoreLocation234554646 (LocalStudentRecord * _theObject_, RpcBuffer& _work_, RpcBuffer& _result_)
{
RpcBuffer _rvBuffer_;
Int32 _errCode_ = OPER_INVOKED_OK;

Boolean _returnedValue_ = _theObject_->useStoreLocation();
_rvBuffer_ << _returnedValue_;
if (_rvBuffer_.length() > 0)
_result_ << _rvBuffer_;
return _errCode_;
}


Int32 ServerStudentRecord::save_state120944197 (LocalStudentRecord * _theObject_, RpcBuffer& _work_, RpcBuffer& _result_)
{
RpcBuffer _rvBuffer_;
Int32 _errCode_ = OPER_INVOKED_OK;
ObjectState* Par_0_ = 0;
_work_ >> Par_0_;
ObjectType Par_1_;
_work_ >> Par_1_;
if (_work_.readState() == BUFFER_GOOD)
{
Boolean _returnedValue_ = _theObject_->save_state(*Par_0_, Par_1_);
_rvBuffer_ << _returnedValue_;
_result_ << Par_0_;
if (_rvBuffer_.length() > 0)
_result_ << _rvBuffer_;
}
else
_errCode_ = PARAMETER_ERROR;

_work_.rewrite();

if(_work_.shouldPack(Par_0_))
delete Par_0_;
return _errCode_;
}


Int32 ServerStudentRecord::restore_state169823749 (LocalStudentRecord * _theObject_, RpcBuffer& _work_, RpcBuffer& _result_)
{
RpcBuffer _rvBuffer_;
Int32 _errCode_ = OPER_INVOKED_OK;
ObjectState* Par_0_ = 0;
_work_ >> Par_0_;
ObjectType Par_1_;
_work_ >> Par_1_;
if (_work_.readState() == BUFFER_GOOD)
{
Boolean _returnedValue_ = _theObject_->restore_state(*Par_0_, Par_1_);
_rvBuffer_ << _returnedValue_;
_result_ << Par_0_;
if (_rvBuffer_.length() > 0)
_result_ << _rvBuffer_;
}
else
_errCode_ = PARAMETER_ERROR;

_work_.rewrite();

if(_work_.shouldPack(Par_0_))
delete Par_0_;
return _errCode_;
}


Int32 ServerStudentRecord::type110749670 (LocalStudentRecord * _theObject_, RpcBuffer& _work_, RpcBuffer& _result_)
{
RpcBuffer _rvBuffer_;
Int32 _errCode_ = OPER_INVOKED_OK;

const TypeName _returnedValue_ = _theObject_->type();
_rvBuffer_ << _returnedValue_;
if (_rvBuffer_.length() > 0)
_result_ << _rvBuffer_;
return _errCode_;
}


