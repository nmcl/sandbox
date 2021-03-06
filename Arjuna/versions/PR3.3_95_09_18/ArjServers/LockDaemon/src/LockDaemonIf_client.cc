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

#include <StubGen/ClientRpc.h>

#include "LockDaemonIf_stub.h"

RemoteLockDaemonIf::RemoteLockDaemonIf ( const ClientRpcManager& manager, const RpcBuffer& phv ): _clientManager(manager), _myHashVal(phv)
{
_myHashVal << 43507030 << -1;
}

RemoteLockDaemonIf::RemoteLockDaemonIf ( Boolean& Par_0_, ClientRpcManager *_cRpcMan_ ): _clientManager (*_cRpcMan_), _myHashVal(-1)
{
RpcBuffer _callBuffer_(_myHashVal);
RpcBuffer _replyBuffer_;
RPC_Status _rpcStatus_ = OPER_UNKNOWN;
Int32 _serverStatus_ = OPER_INVOKED_OK;

_cRpcMan_->destroy();

_callBuffer_ << &Par_0_;

_rpcStatus_ = _clientManager.call(3587918, _callBuffer_, _serverStatus_, _replyBuffer_);
if (_rpcStatus_ == OPER_DONE)
{
Boolean* Par_0_Ptr = &Par_0_;
_replyBuffer_ >> Par_0_Ptr;
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

RemoteLockDaemonIf::~RemoteLockDaemonIf ( )
{
RpcBuffer _callBuffer_(_myHashVal);
RpcBuffer _replyBuffer_;
RPC_Status _rpcStatus_ = OPER_UNKNOWN;
Int32 _serverStatus_ = OPER_INVOKED_OK;


_rpcStatus_ = _clientManager.call(75959190, _callBuffer_, _serverStatus_, _replyBuffer_);
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

void RemoteLockDaemonIf::propagate ( const Uid& Par_0_, const Uid& Par_1_, const TypeName Par_2_, Boolean& Par_3_)
{
RpcBuffer _callBuffer_(_myHashVal);
RpcBuffer _replyBuffer_;
RPC_Status _rpcStatus_ = OPER_UNKNOWN;
Int32 _serverStatus_ = OPER_INVOKED_OK;

_callBuffer_ << &Par_0_;
_callBuffer_ << &Par_1_;
_callBuffer_ << Par_2_;
_callBuffer_ << &Par_3_;

_rpcStatus_ = _clientManager.call(119239758, _callBuffer_, _serverStatus_, _replyBuffer_);
if (_rpcStatus_ == OPER_DONE)
{
Boolean* Par_3_Ptr = &Par_3_;
_replyBuffer_ >> Par_3_Ptr;
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

void RemoteLockDaemonIf::releaseAll ( const Uid& Par_0_, const Uid& Par_1_, const TypeName Par_2_, Boolean& Par_3_)
{
RpcBuffer _callBuffer_(_myHashVal);
RpcBuffer _replyBuffer_;
RPC_Status _rpcStatus_ = OPER_UNKNOWN;
Int32 _serverStatus_ = OPER_INVOKED_OK;

_callBuffer_ << &Par_0_;
_callBuffer_ << &Par_1_;
_callBuffer_ << Par_2_;
_callBuffer_ << &Par_3_;

_rpcStatus_ = _clientManager.call(99837102, _callBuffer_, _serverStatus_, _replyBuffer_);
if (_rpcStatus_ == OPER_DONE)
{
Boolean* Par_3_Ptr = &Par_3_;
_replyBuffer_ >> Par_3_Ptr;
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

void RemoteLockDaemonIf::releaselock ( const Uid& Par_0_, const Uid& Par_1_, const TypeName Par_2_, Boolean& Par_3_)
{
RpcBuffer _callBuffer_(_myHashVal);
RpcBuffer _replyBuffer_;
RPC_Status _rpcStatus_ = OPER_UNKNOWN;
Int32 _serverStatus_ = OPER_INVOKED_OK;

_callBuffer_ << &Par_0_;
_callBuffer_ << &Par_1_;
_callBuffer_ << Par_2_;
_callBuffer_ << &Par_3_;

_rpcStatus_ = _clientManager.call(59903054, _callBuffer_, _serverStatus_, _replyBuffer_);
if (_rpcStatus_ == OPER_DONE)
{
Boolean* Par_3_Ptr = &Par_3_;
_replyBuffer_ >> Par_3_Ptr;
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

void RemoteLockDaemonIf::setlock ( Lock* Par_0_, const Uid& Par_1_, const TypeName Par_2_, LockResult& Par_3_)
{
RpcBuffer _callBuffer_(_myHashVal);
RpcBuffer _replyBuffer_;
RPC_Status _rpcStatus_ = OPER_UNKNOWN;
Int32 _serverStatus_ = OPER_INVOKED_OK;

_callBuffer_ << Par_0_;
_callBuffer_ << &Par_1_;
_callBuffer_ << Par_2_;
_callBuffer_ << &Par_3_;

_rpcStatus_ = _clientManager.call(246490452, _callBuffer_, _serverStatus_, _replyBuffer_);
if (_rpcStatus_ == OPER_DONE)
{
LockResult* Par_3_Ptr = &Par_3_;
_replyBuffer_ >> Par_3_Ptr;
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
