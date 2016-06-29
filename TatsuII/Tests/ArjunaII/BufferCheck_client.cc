/*
 * Automatically Generated by:
 * Arjuna Stub Generator Version 6.0.0 - $Date: 1997/06/11 20:25:11 $
 *
 * Do Not Edit By Hand
 */

#ifdef STUB
#  pragma @NoRename
#endif

#include <StubGen/ClientRpc.h>

#include "BufferCheck_stub.h"

RemoteBufferCheck::RemoteBufferCheck ( const ClientRpcManager& manager, const RpcBuffer& phv ): RemoteA(manager, RpcBuffer(phv,203446107)), _clientManager(manager), _myHashVal(phv)
{
_myHashVal << 203446107 << -1;
}

RemoteBufferCheck::RemoteBufferCheck ( int& Par_0_, int& Par_1_, ClientRpcManager *_cRpcMan_ ): RemoteA(*_cRpcMan_, RpcBuffer()), _clientManager (*_cRpcMan_), _myHashVal(-1)
{
RpcBuffer _callBuffer_(_myHashVal);
RpcBuffer _replyBuffer_;
RPC_Status _rpcStatus_ = OPER_UNKNOWN;
Int32 _serverStatus_ = OPER_INVOKED_OK;

_cRpcMan_->destroy();

_callBuffer_ << &Par_0_;
_callBuffer_ << &Par_1_;

_rpcStatus_ = _clientManager.call(258928217, _callBuffer_, _serverStatus_, _replyBuffer_);

if (_rpcStatus_ == OPER_DONE)
{
int* Par_0_Ptr = &Par_0_;
_replyBuffer_ >> Par_0_Ptr;
int* Par_1_Ptr = &Par_1_;
_replyBuffer_ >> Par_1_Ptr;
switch(_serverStatus_)
{
case OPER_INVOKED_OK:
break;
default:
_clientManager.rpcAbort(_rpcStatus_, _serverStatus_);
}
}
else
_clientManager.rpcAbort(_rpcStatus_, _serverStatus_);

}

RemoteBufferCheck::~RemoteBufferCheck ( )
{
RpcBuffer _callBuffer_(_myHashVal);
RpcBuffer _replyBuffer_;
RPC_Status _rpcStatus_ = OPER_UNKNOWN;
Int32 _serverStatus_ = OPER_INVOKED_OK;


_rpcStatus_ = _clientManager.call(84735718, _callBuffer_, _serverStatus_, _replyBuffer_);

if (_rpcStatus_ == OPER_DONE)
{
switch(_serverStatus_)
{
case OPER_INVOKED_OK:
break;
default:
_clientManager.rpcAbort(_rpcStatus_, _serverStatus_);
}
}
else
_clientManager.rpcAbort(_rpcStatus_, _serverStatus_);

}

int RemoteBufferCheck::Test ( Buffer Par_0_, Buffer& Par_1_)
{
RpcBuffer _rvBuffer_;
RpcBuffer _callBuffer_(_myHashVal);
RpcBuffer _replyBuffer_;
RPC_Status _rpcStatus_ = OPER_UNKNOWN;
Int32 _serverStatus_ = OPER_INVOKED_OK;

int _returnedValue_ = -1;
_callBuffer_ << Par_0_;
_callBuffer_ << &Par_1_;

_rpcStatus_ = _clientManager.call(96138770, _callBuffer_, _serverStatus_, _replyBuffer_);

if (_rpcStatus_ == OPER_DONE)
{
Buffer* Par_1_Ptr = &Par_1_;
_replyBuffer_ >> Par_1_Ptr;
switch(_serverStatus_)
{
case OPER_INVOKED_OK:
_replyBuffer_ >> _rvBuffer_;
_rvBuffer_ >> _returnedValue_;
break;
default:
_clientManager.rpcAbort(_rpcStatus_, _serverStatus_);
}
}
else
_clientManager.rpcAbort(_rpcStatus_, _serverStatus_);

return (_returnedValue_);
}
