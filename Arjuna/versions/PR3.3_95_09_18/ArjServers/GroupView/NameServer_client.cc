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

#include "NameServ_stub.h"

RemoteNameServer::RemoteNameServer ( const ClientRpcManager& manager, const RpcBuffer& phv ): RemoteLockManager(manager, RpcBuffer(phv,177853682)), _clientManager(manager), _myHashVal(phv)
{
_myHashVal << 177853682 << -1;
}

RemoteNameServer::RemoteNameServer ( int& Par_0_, ClientRpcManager *_cRpcMan_ ): RemoteLockManager(*_cRpcMan_, RpcBuffer()), _clientManager (*_cRpcMan_), _myHashVal(-1)
{
RpcBuffer _callBuffer_(_myHashVal);
RpcBuffer _replyBuffer_;
RPC_Status _rpcStatus_ = OPER_UNKNOWN;
Int32 _serverStatus_ = OPER_INVOKED_OK;

_cRpcMan_->destroy();

_callBuffer_ << &Par_0_;

_rpcStatus_ = _clientManager.call(200632073, _callBuffer_, _serverStatus_, _replyBuffer_);
if (_rpcStatus_ == OPER_DONE)
{
int* Par_0_Ptr = &Par_0_;
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

RemoteNameServer::RemoteNameServer ( const Uid& Par_0_, int& Par_1_, ClientRpcManager *_cRpcMan_ ): RemoteLockManager(*_cRpcMan_, RpcBuffer()), _clientManager (*_cRpcMan_), _myHashVal(-1)
{
RpcBuffer _callBuffer_(_myHashVal);
RpcBuffer _replyBuffer_;
RPC_Status _rpcStatus_ = OPER_UNKNOWN;
Int32 _serverStatus_ = OPER_INVOKED_OK;

_cRpcMan_->destroy();

_callBuffer_ << &Par_0_;
_callBuffer_ << &Par_1_;

_rpcStatus_ = _clientManager.call(238333945, _callBuffer_, _serverStatus_, _replyBuffer_);
if (_rpcStatus_ == OPER_DONE)
{
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

RemoteNameServer::~RemoteNameServer ( )
{
RpcBuffer _callBuffer_(_myHashVal);
RpcBuffer _replyBuffer_;
RPC_Status _rpcStatus_ = OPER_UNKNOWN;
Int32 _serverStatus_ = OPER_INVOKED_OK;


_rpcStatus_ = _clientManager.call(177161750, _callBuffer_, _serverStatus_, _replyBuffer_);
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

Boolean RemoteNameServer::getView ( ReplicaDescriptor& Par_0_, const Uid& Par_1_, const char* Par_2_, pid_t Par_3_, int& Par_4_, unsigned long& Par_5_)
{
RpcBuffer _rvBuffer_;
RpcBuffer _callBuffer_(_myHashVal);
RpcBuffer _replyBuffer_;
RPC_Status _rpcStatus_ = OPER_UNKNOWN;
Int32 _serverStatus_ = OPER_INVOKED_OK;

Boolean _returnedValue_;
_callBuffer_ << &Par_0_;
_callBuffer_ << &Par_1_;
_callBuffer_ << Par_2_;
_callBuffer_ << Par_3_;
_callBuffer_ << &Par_4_;
_callBuffer_ << &Par_5_;

_rpcStatus_ = _clientManager.call(189166844, _callBuffer_, _serverStatus_, _replyBuffer_);
if (_rpcStatus_ == OPER_DONE)
{
ReplicaDescriptor* Par_0_Ptr = &Par_0_;
_replyBuffer_ >> Par_0_Ptr;
int* Par_4_Ptr = &Par_4_;
_replyBuffer_ >> Par_4_Ptr;
unsigned long* Par_5_Ptr = &Par_5_;
_replyBuffer_ >> Par_5_Ptr;
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

Boolean RemoteNameServer::lockGroup ( const Uid& Par_0_, const char* Par_1_, pid_t Par_2_, LockMode Par_3_, int& Par_4_, unsigned long& Par_5_)
{
RpcBuffer _rvBuffer_;
RpcBuffer _callBuffer_(_myHashVal);
RpcBuffer _replyBuffer_;
RPC_Status _rpcStatus_ = OPER_UNKNOWN;
Int32 _serverStatus_ = OPER_INVOKED_OK;

Boolean _returnedValue_;
_callBuffer_ << &Par_0_;
_callBuffer_ << Par_1_;
_callBuffer_ << Par_2_;
_callBuffer_ << Par_3_;
_callBuffer_ << &Par_4_;
_callBuffer_ << &Par_5_;

_rpcStatus_ = _clientManager.call(81205148, _callBuffer_, _serverStatus_, _replyBuffer_);
if (_rpcStatus_ == OPER_DONE)
{
int* Par_4_Ptr = &Par_4_;
_replyBuffer_ >> Par_4_Ptr;
unsigned long* Par_5_Ptr = &Par_5_;
_replyBuffer_ >> Par_5_Ptr;
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

Boolean RemoteNameServer::getUseCount ( const Uid& Par_0_, long& Par_1_, int& Par_2_, unsigned long& Par_3_)
{
RpcBuffer _rvBuffer_;
RpcBuffer _callBuffer_(_myHashVal);
RpcBuffer _replyBuffer_;
RPC_Status _rpcStatus_ = OPER_UNKNOWN;
Int32 _serverStatus_ = OPER_INVOKED_OK;

Boolean _returnedValue_;
_callBuffer_ << &Par_0_;
_callBuffer_ << &Par_1_;
_callBuffer_ << &Par_2_;
_callBuffer_ << &Par_3_;

_rpcStatus_ = _clientManager.call(130535164, _callBuffer_, _serverStatus_, _replyBuffer_);
if (_rpcStatus_ == OPER_DONE)
{
long* Par_1_Ptr = &Par_1_;
_replyBuffer_ >> Par_1_Ptr;
int* Par_2_Ptr = &Par_2_;
_replyBuffer_ >> Par_2_Ptr;
unsigned long* Par_3_Ptr = &Par_3_;
_replyBuffer_ >> Par_3_Ptr;
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

Boolean RemoteNameServer::traverseDatabase ( ReplicaDescriptor& Par_0_, Uid& Par_1_, int& Par_2_, const char* Par_3_, pid_t Par_4_, int& Par_5_, unsigned long& Par_6_)
{
RpcBuffer _rvBuffer_;
RpcBuffer _callBuffer_(_myHashVal);
RpcBuffer _replyBuffer_;
RPC_Status _rpcStatus_ = OPER_UNKNOWN;
Int32 _serverStatus_ = OPER_INVOKED_OK;

Boolean _returnedValue_;
_callBuffer_ << &Par_0_;
_callBuffer_ << &Par_1_;
_callBuffer_ << &Par_2_;
_callBuffer_ << Par_3_;
_callBuffer_ << Par_4_;
_callBuffer_ << &Par_5_;
_callBuffer_ << &Par_6_;

_rpcStatus_ = _clientManager.call(119504620, _callBuffer_, _serverStatus_, _replyBuffer_);
if (_rpcStatus_ == OPER_DONE)
{
ReplicaDescriptor* Par_0_Ptr = &Par_0_;
_replyBuffer_ >> Par_0_Ptr;
Uid* Par_1_Ptr = &Par_1_;
_replyBuffer_ >> Par_1_Ptr;
int* Par_2_Ptr = &Par_2_;
_replyBuffer_ >> Par_2_Ptr;
int* Par_5_Ptr = &Par_5_;
_replyBuffer_ >> Par_5_Ptr;
unsigned long* Par_6_Ptr = &Par_6_;
_replyBuffer_ >> Par_6_Ptr;
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

OutCome RemoteNameServer::addReplicas ( const ReplicaDescriptor& Par_0_, const Uid& Par_1_, const char* Par_2_, pid_t Par_3_, int& Par_4_, unsigned long& Par_5_)
{
RpcBuffer _rvBuffer_;
RpcBuffer _callBuffer_(_myHashVal);
RpcBuffer _replyBuffer_;
RPC_Status _rpcStatus_ = OPER_UNKNOWN;
Int32 _serverStatus_ = OPER_INVOKED_OK;

OutCome _returnedValue_;
_callBuffer_ << &Par_0_;
_callBuffer_ << &Par_1_;
_callBuffer_ << Par_2_;
_callBuffer_ << Par_3_;
_callBuffer_ << &Par_4_;
_callBuffer_ << &Par_5_;

_rpcStatus_ = _clientManager.call(222535708, _callBuffer_, _serverStatus_, _replyBuffer_);
if (_rpcStatus_ == OPER_DONE)
{
int* Par_4_Ptr = &Par_4_;
_replyBuffer_ >> Par_4_Ptr;
unsigned long* Par_5_Ptr = &Par_5_;
_replyBuffer_ >> Par_5_Ptr;
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

OutCome RemoteNameServer::deleteReplicas ( const Uid& Par_0_, const ReplicaDescriptor& Par_1_, const char* Par_2_, pid_t Par_3_, int& Par_4_, unsigned long& Par_5_, const Boolean& Par_6_)
{
RpcBuffer _rvBuffer_;
RpcBuffer _callBuffer_(_myHashVal);
RpcBuffer _replyBuffer_;
RPC_Status _rpcStatus_ = OPER_UNKNOWN;
Int32 _serverStatus_ = OPER_INVOKED_OK;

OutCome _returnedValue_;
_callBuffer_ << &Par_0_;
_callBuffer_ << &Par_1_;
_callBuffer_ << Par_2_;
_callBuffer_ << Par_3_;
_callBuffer_ << &Par_4_;
_callBuffer_ << &Par_5_;
_callBuffer_ << &Par_6_;

_rpcStatus_ = _clientManager.call(232059006, _callBuffer_, _serverStatus_, _replyBuffer_);
if (_rpcStatus_ == OPER_DONE)
{
int* Par_4_Ptr = &Par_4_;
_replyBuffer_ >> Par_4_Ptr;
unsigned long* Par_5_Ptr = &Par_5_;
_replyBuffer_ >> Par_5_Ptr;
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

OutCome RemoteNameServer::alterLocation ( const Uid& Par_0_, const Uid& Par_1_, const char* Par_2_, const char* Par_3_, pid_t Par_4_, int& Par_5_, unsigned long& Par_6_)
{
RpcBuffer _rvBuffer_;
RpcBuffer _callBuffer_(_myHashVal);
RpcBuffer _replyBuffer_;
RPC_Status _rpcStatus_ = OPER_UNKNOWN;
Int32 _serverStatus_ = OPER_INVOKED_OK;

OutCome _returnedValue_;
_callBuffer_ << &Par_0_;
_callBuffer_ << &Par_1_;
_callBuffer_ << Par_2_;
_callBuffer_ << Par_3_;
_callBuffer_ << Par_4_;
_callBuffer_ << &Par_5_;
_callBuffer_ << &Par_6_;

_rpcStatus_ = _clientManager.call(175794844, _callBuffer_, _serverStatus_, _replyBuffer_);
if (_rpcStatus_ == OPER_DONE)
{
int* Par_5_Ptr = &Par_5_;
_replyBuffer_ >> Par_5_Ptr;
unsigned long* Par_6_Ptr = &Par_6_;
_replyBuffer_ >> Par_6_Ptr;
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

OutCome RemoteNameServer::exclude ( const ReplicaDescriptor& Par_0_, const Uid& Par_1_, const char* Par_2_, pid_t Par_3_, int& Par_4_, unsigned long& Par_5_)
{
RpcBuffer _rvBuffer_;
RpcBuffer _callBuffer_(_myHashVal);
RpcBuffer _replyBuffer_;
RPC_Status _rpcStatus_ = OPER_UNKNOWN;
Int32 _serverStatus_ = OPER_INVOKED_OK;

OutCome _returnedValue_;
_callBuffer_ << &Par_0_;
_callBuffer_ << &Par_1_;
_callBuffer_ << Par_2_;
_callBuffer_ << Par_3_;
_callBuffer_ << &Par_4_;
_callBuffer_ << &Par_5_;

_rpcStatus_ = _clientManager.call(15081196, _callBuffer_, _serverStatus_, _replyBuffer_);
if (_rpcStatus_ == OPER_DONE)
{
int* Par_4_Ptr = &Par_4_;
_replyBuffer_ >> Par_4_Ptr;
unsigned long* Par_5_Ptr = &Par_5_;
_replyBuffer_ >> Par_5_Ptr;
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

OutCome RemoteNameServer::include ( const ReplicaDescriptor& Par_0_, const Uid& Par_1_, const char* Par_2_, pid_t Par_3_, int& Par_4_, unsigned long& Par_5_)
{
RpcBuffer _rvBuffer_;
RpcBuffer _callBuffer_(_myHashVal);
RpcBuffer _replyBuffer_;
RPC_Status _rpcStatus_ = OPER_UNKNOWN;
Int32 _serverStatus_ = OPER_INVOKED_OK;

OutCome _returnedValue_;
_callBuffer_ << &Par_0_;
_callBuffer_ << &Par_1_;
_callBuffer_ << Par_2_;
_callBuffer_ << Par_3_;
_callBuffer_ << &Par_4_;
_callBuffer_ << &Par_5_;

_rpcStatus_ = _clientManager.call(15073868, _callBuffer_, _serverStatus_, _replyBuffer_);
if (_rpcStatus_ == OPER_DONE)
{
int* Par_4_Ptr = &Par_4_;
_replyBuffer_ >> Par_4_Ptr;
unsigned long* Par_5_Ptr = &Par_5_;
_replyBuffer_ >> Par_5_Ptr;
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

OutCome RemoteNameServer::releaseGroup ( const Uid& Par_0_, const char* Par_1_, pid_t Par_2_, int& Par_3_, unsigned long& Par_4_)
{
RpcBuffer _rvBuffer_;
RpcBuffer _callBuffer_(_myHashVal);
RpcBuffer _replyBuffer_;
RPC_Status _rpcStatus_ = OPER_UNKNOWN;
Int32 _serverStatus_ = OPER_INVOKED_OK;

OutCome _returnedValue_;
_callBuffer_ << &Par_0_;
_callBuffer_ << Par_1_;
_callBuffer_ << Par_2_;
_callBuffer_ << &Par_3_;
_callBuffer_ << &Par_4_;

_rpcStatus_ = _clientManager.call(109857964, _callBuffer_, _serverStatus_, _replyBuffer_);
if (_rpcStatus_ == OPER_DONE)
{
int* Par_3_Ptr = &Par_3_;
_replyBuffer_ >> Par_3_Ptr;
unsigned long* Par_4_Ptr = &Par_4_;
_replyBuffer_ >> Par_4_Ptr;
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

Boolean RemoteNameServer::groupAssociation ( const Uid& Par_0_, Uid& Par_1_, int& Par_2_, unsigned long& Par_3_)
{
RpcBuffer _rvBuffer_;
RpcBuffer _callBuffer_(_myHashVal);
RpcBuffer _replyBuffer_;
RPC_Status _rpcStatus_ = OPER_UNKNOWN;
Int32 _serverStatus_ = OPER_INVOKED_OK;

Boolean _returnedValue_;
_callBuffer_ << &Par_0_;
_callBuffer_ << &Par_1_;
_callBuffer_ << &Par_2_;
_callBuffer_ << &Par_3_;

_rpcStatus_ = _clientManager.call(75444940, _callBuffer_, _serverStatus_, _replyBuffer_);
if (_rpcStatus_ == OPER_DONE)
{
Uid* Par_1_Ptr = &Par_1_;
_replyBuffer_ >> Par_1_Ptr;
int* Par_2_Ptr = &Par_2_;
_replyBuffer_ >> Par_2_Ptr;
unsigned long* Par_3_Ptr = &Par_3_;
_replyBuffer_ >> Par_3_Ptr;
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

OutCome RemoteNameServer::recover ( const char* Par_0_, int& Par_1_, unsigned long& Par_2_)
{
RpcBuffer _rvBuffer_;
RpcBuffer _callBuffer_(_myHashVal);
RpcBuffer _replyBuffer_;
RPC_Status _rpcStatus_ = OPER_UNKNOWN;
Int32 _serverStatus_ = OPER_INVOKED_OK;

OutCome _returnedValue_;
_callBuffer_ << Par_0_;
_callBuffer_ << &Par_1_;
_callBuffer_ << &Par_2_;

_rpcStatus_ = _clientManager.call(42677660, _callBuffer_, _serverStatus_, _replyBuffer_);
if (_rpcStatus_ == OPER_DONE)
{
int* Par_1_Ptr = &Par_1_;
_replyBuffer_ >> Par_1_Ptr;
unsigned long* Par_2_Ptr = &Par_2_;
_replyBuffer_ >> Par_2_Ptr;
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

Boolean RemoteNameServer::testAndSet ( ReplicaDescriptor& Par_0_, const Uid& Par_1_, const char* Par_2_, pid_t Par_3_, int& Par_4_, unsigned long& Par_5_)
{
RpcBuffer _rvBuffer_;
RpcBuffer _callBuffer_(_myHashVal);
RpcBuffer _replyBuffer_;
RPC_Status _rpcStatus_ = OPER_UNKNOWN;
Int32 _serverStatus_ = OPER_INVOKED_OK;

Boolean _returnedValue_;
_callBuffer_ << &Par_0_;
_callBuffer_ << &Par_1_;
_callBuffer_ << Par_2_;
_callBuffer_ << Par_3_;
_callBuffer_ << &Par_4_;
_callBuffer_ << &Par_5_;

_rpcStatus_ = _clientManager.call(262775036, _callBuffer_, _serverStatus_, _replyBuffer_);
if (_rpcStatus_ == OPER_DONE)
{
ReplicaDescriptor* Par_0_Ptr = &Par_0_;
_replyBuffer_ >> Par_0_Ptr;
int* Par_4_Ptr = &Par_4_;
_replyBuffer_ >> Par_4_Ptr;
unsigned long* Par_5_Ptr = &Par_5_;
_replyBuffer_ >> Par_5_Ptr;
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

OutCome RemoteNameServer::needsUpdating ( const Uid& Par_0_, Boolean& Par_1_, ReplicaDescriptor& Par_2_, const char* Par_3_, pid_t Par_4_, int& Par_5_, unsigned long& Par_6_)
{
RpcBuffer _rvBuffer_;
RpcBuffer _callBuffer_(_myHashVal);
RpcBuffer _replyBuffer_;
RPC_Status _rpcStatus_ = OPER_UNKNOWN;
Int32 _serverStatus_ = OPER_INVOKED_OK;

OutCome _returnedValue_;
_callBuffer_ << &Par_0_;
_callBuffer_ << &Par_1_;
_callBuffer_ << &Par_2_;
_callBuffer_ << Par_3_;
_callBuffer_ << Par_4_;
_callBuffer_ << &Par_5_;
_callBuffer_ << &Par_6_;

_rpcStatus_ = _clientManager.call(82735980, _callBuffer_, _serverStatus_, _replyBuffer_);
if (_rpcStatus_ == OPER_DONE)
{
Boolean* Par_1_Ptr = &Par_1_;
_replyBuffer_ >> Par_1_Ptr;
ReplicaDescriptor* Par_2_Ptr = &Par_2_;
_replyBuffer_ >> Par_2_Ptr;
int* Par_5_Ptr = &Par_5_;
_replyBuffer_ >> Par_5_Ptr;
unsigned long* Par_6_Ptr = &Par_6_;
_replyBuffer_ >> Par_6_Ptr;
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

OutCome RemoteNameServer::getNodeUpdateList ( ReplicaDescriptor& Par_0_, ReplicaDescriptor& Par_1_, const char* Par_2_, int& Par_3_, unsigned long& Par_4_)
{
RpcBuffer _rvBuffer_;
RpcBuffer _callBuffer_(_myHashVal);
RpcBuffer _replyBuffer_;
RPC_Status _rpcStatus_ = OPER_UNKNOWN;
Int32 _serverStatus_ = OPER_INVOKED_OK;

OutCome _returnedValue_;
_callBuffer_ << &Par_0_;
_callBuffer_ << &Par_1_;
_callBuffer_ << Par_2_;
_callBuffer_ << &Par_3_;
_callBuffer_ << &Par_4_;

_rpcStatus_ = _clientManager.call(131495436, _callBuffer_, _serverStatus_, _replyBuffer_);
if (_rpcStatus_ == OPER_DONE)
{
ReplicaDescriptor* Par_0_Ptr = &Par_0_;
_replyBuffer_ >> Par_0_Ptr;
ReplicaDescriptor* Par_1_Ptr = &Par_1_;
_replyBuffer_ >> Par_1_Ptr;
int* Par_3_Ptr = &Par_3_;
_replyBuffer_ >> Par_3_Ptr;
unsigned long* Par_4_Ptr = &Par_4_;
_replyBuffer_ >> Par_4_Ptr;
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

Boolean RemoteNameServer::getGroupViewState ( ObjectState& Par_0_, int& Par_1_, unsigned long& Par_2_)
{
RpcBuffer _rvBuffer_;
RpcBuffer _callBuffer_(_myHashVal);
RpcBuffer _replyBuffer_;
RPC_Status _rpcStatus_ = OPER_UNKNOWN;
Int32 _serverStatus_ = OPER_INVOKED_OK;

Boolean _returnedValue_;
_callBuffer_ << &Par_0_;
_callBuffer_ << &Par_1_;
_callBuffer_ << &Par_2_;

_rpcStatus_ = _clientManager.call(230028300, _callBuffer_, _serverStatus_, _replyBuffer_);
if (_rpcStatus_ == OPER_DONE)
{
ObjectState* Par_0_Ptr = &Par_0_;
_replyBuffer_ >> Par_0_Ptr;
int* Par_1_Ptr = &Par_1_;
_replyBuffer_ >> Par_1_Ptr;
unsigned long* Par_2_Ptr = &Par_2_;
_replyBuffer_ >> Par_2_Ptr;
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

Boolean RemoteNameServer::imposeNewGroupViewState ( ObjectState& Par_0_, int& Par_1_)
{
RpcBuffer _rvBuffer_;
RpcBuffer _callBuffer_(_myHashVal);
RpcBuffer _replyBuffer_;
RPC_Status _rpcStatus_ = OPER_UNKNOWN;
Int32 _serverStatus_ = OPER_INVOKED_OK;

Boolean _returnedValue_;
_callBuffer_ << &Par_0_;
_callBuffer_ << &Par_1_;

_rpcStatus_ = _clientManager.call(49971337, _callBuffer_, _serverStatus_, _replyBuffer_);
if (_rpcStatus_ == OPER_DONE)
{
ObjectState* Par_0_Ptr = &Par_0_;
_replyBuffer_ >> Par_0_Ptr;
int* Par_1_Ptr = &Par_1_;
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

Boolean RemoteNameServer::getReplicaState ( const Uid& Par_0_, ObjectState& Par_1_, int& Par_2_, unsigned long& Par_3_)
{
RpcBuffer _rvBuffer_;
RpcBuffer _callBuffer_(_myHashVal);
RpcBuffer _replyBuffer_;
RPC_Status _rpcStatus_ = OPER_UNKNOWN;
Int32 _serverStatus_ = OPER_INVOKED_OK;

Boolean _returnedValue_;
_callBuffer_ << &Par_0_;
_callBuffer_ << &Par_1_;
_callBuffer_ << &Par_2_;
_callBuffer_ << &Par_3_;

_rpcStatus_ = _clientManager.call(150067804, _callBuffer_, _serverStatus_, _replyBuffer_);
if (_rpcStatus_ == OPER_DONE)
{
ObjectState* Par_1_Ptr = &Par_1_;
_replyBuffer_ >> Par_1_Ptr;
int* Par_2_Ptr = &Par_2_;
_replyBuffer_ >> Par_2_Ptr;
unsigned long* Par_3_Ptr = &Par_3_;
_replyBuffer_ >> Par_3_Ptr;
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

Boolean RemoteNameServer::imposeReplicaState ( const Uid& Par_0_, ObjectState& Par_1_, int& Par_2_, unsigned long& Par_3_)
{
RpcBuffer _rvBuffer_;
RpcBuffer _callBuffer_(_myHashVal);
RpcBuffer _replyBuffer_;
RPC_Status _rpcStatus_ = OPER_UNKNOWN;
Int32 _serverStatus_ = OPER_INVOKED_OK;

Boolean _returnedValue_;
_callBuffer_ << &Par_0_;
_callBuffer_ << &Par_1_;
_callBuffer_ << &Par_2_;
_callBuffer_ << &Par_3_;

_rpcStatus_ = _clientManager.call(229743500, _callBuffer_, _serverStatus_, _replyBuffer_);
if (_rpcStatus_ == OPER_DONE)
{
ObjectState* Par_1_Ptr = &Par_1_;
_replyBuffer_ >> Par_1_Ptr;
int* Par_2_Ptr = &Par_2_;
_replyBuffer_ >> Par_2_Ptr;
unsigned long* Par_3_Ptr = &Par_3_;
_replyBuffer_ >> Par_3_Ptr;
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

OutCome RemoteNameServer::addTemplate ( const Uid& Par_0_, Buffer& Par_1_, const char* Par_2_, pid_t Par_3_, int& Par_4_, unsigned long& Par_5_)
{
RpcBuffer _rvBuffer_;
RpcBuffer _callBuffer_(_myHashVal);
RpcBuffer _replyBuffer_;
RPC_Status _rpcStatus_ = OPER_UNKNOWN;
Int32 _serverStatus_ = OPER_INVOKED_OK;

OutCome _returnedValue_;
_callBuffer_ << &Par_0_;
_callBuffer_ << &Par_1_;
_callBuffer_ << Par_2_;
_callBuffer_ << Par_3_;
_callBuffer_ << &Par_4_;
_callBuffer_ << &Par_5_;

_rpcStatus_ = _clientManager.call(57558620, _callBuffer_, _serverStatus_, _replyBuffer_);
if (_rpcStatus_ == OPER_DONE)
{
Buffer* Par_1_Ptr = &Par_1_;
_replyBuffer_ >> Par_1_Ptr;
int* Par_4_Ptr = &Par_4_;
_replyBuffer_ >> Par_4_Ptr;
unsigned long* Par_5_Ptr = &Par_5_;
_replyBuffer_ >> Par_5_Ptr;
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

Boolean RemoteNameServer::getTemplate ( const Uid& Par_0_, Buffer& Par_1_, LockMode Par_2_, int& Par_3_, unsigned long& Par_4_)
{
RpcBuffer _rvBuffer_;
RpcBuffer _callBuffer_(_myHashVal);
RpcBuffer _replyBuffer_;
RPC_Status _rpcStatus_ = OPER_UNKNOWN;
Int32 _serverStatus_ = OPER_INVOKED_OK;

Boolean _returnedValue_;
_callBuffer_ << &Par_0_;
_callBuffer_ << &Par_1_;
_callBuffer_ << Par_2_;
_callBuffer_ << &Par_3_;
_callBuffer_ << &Par_4_;

_rpcStatus_ = _clientManager.call(150841692, _callBuffer_, _serverStatus_, _replyBuffer_);
if (_rpcStatus_ == OPER_DONE)
{
Buffer* Par_1_Ptr = &Par_1_;
_replyBuffer_ >> Par_1_Ptr;
int* Par_3_Ptr = &Par_3_;
_replyBuffer_ >> Par_3_Ptr;
unsigned long* Par_4_Ptr = &Par_4_;
_replyBuffer_ >> Par_4_Ptr;
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

OutCome RemoteNameServer::deleteTemplate ( const Uid& Par_0_, const char* Par_1_, pid_t Par_2_, int& Par_3_, unsigned long& Par_4_)
{
RpcBuffer _rvBuffer_;
RpcBuffer _callBuffer_(_myHashVal);
RpcBuffer _replyBuffer_;
RPC_Status _rpcStatus_ = OPER_UNKNOWN;
Int32 _serverStatus_ = OPER_INVOKED_OK;

OutCome _returnedValue_;
_callBuffer_ << &Par_0_;
_callBuffer_ << Par_1_;
_callBuffer_ << Par_2_;
_callBuffer_ << &Par_3_;
_callBuffer_ << &Par_4_;

_rpcStatus_ = _clientManager.call(54505916, _callBuffer_, _serverStatus_, _replyBuffer_);
if (_rpcStatus_ == OPER_DONE)
{
int* Par_3_Ptr = &Par_3_;
_replyBuffer_ >> Par_3_Ptr;
unsigned long* Par_4_Ptr = &Par_4_;
_replyBuffer_ >> Par_4_Ptr;
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

OutCome RemoteNameServer::replaceTemplate ( const Uid& Par_0_, Buffer& Par_1_, const char* Par_2_, pid_t Par_3_, int& Par_4_, unsigned long& Par_5_)
{
RpcBuffer _rvBuffer_;
RpcBuffer _callBuffer_(_myHashVal);
RpcBuffer _replyBuffer_;
RPC_Status _rpcStatus_ = OPER_UNKNOWN;
Int32 _serverStatus_ = OPER_INVOKED_OK;

OutCome _returnedValue_;
_callBuffer_ << &Par_0_;
_callBuffer_ << &Par_1_;
_callBuffer_ << Par_2_;
_callBuffer_ << Par_3_;
_callBuffer_ << &Par_4_;
_callBuffer_ << &Par_5_;

_rpcStatus_ = _clientManager.call(52922652, _callBuffer_, _serverStatus_, _replyBuffer_);
if (_rpcStatus_ == OPER_DONE)
{
Buffer* Par_1_Ptr = &Par_1_;
_replyBuffer_ >> Par_1_Ptr;
int* Par_4_Ptr = &Par_4_;
_replyBuffer_ >> Par_4_Ptr;
unsigned long* Par_5_Ptr = &Par_5_;
_replyBuffer_ >> Par_5_Ptr;
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

OutCome RemoteNameServer::assignCompositeObject ( const Uid& Par_0_, const Uid& Par_1_, const char* Par_2_, pid_t Par_3_, int& Par_4_, unsigned long& Par_5_)
{
RpcBuffer _rvBuffer_;
RpcBuffer _callBuffer_(_myHashVal);
RpcBuffer _replyBuffer_;
RPC_Status _rpcStatus_ = OPER_UNKNOWN;
Int32 _serverStatus_ = OPER_INVOKED_OK;

OutCome _returnedValue_;
_callBuffer_ << &Par_0_;
_callBuffer_ << &Par_1_;
_callBuffer_ << Par_2_;
_callBuffer_ << Par_3_;
_callBuffer_ << &Par_4_;
_callBuffer_ << &Par_5_;

_rpcStatus_ = _clientManager.call(176394412, _callBuffer_, _serverStatus_, _replyBuffer_);
if (_rpcStatus_ == OPER_DONE)
{
int* Par_4_Ptr = &Par_4_;
_replyBuffer_ >> Par_4_Ptr;
unsigned long* Par_5_Ptr = &Par_5_;
_replyBuffer_ >> Par_5_Ptr;
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

OutCome RemoteNameServer::assignCompositeObject ( const Uid& Par_0_, const ReplicaDescriptor& Par_1_, const char* Par_2_, pid_t Par_3_, int& Par_4_, unsigned long& Par_5_)
{
RpcBuffer _rvBuffer_;
RpcBuffer _callBuffer_(_myHashVal);
RpcBuffer _replyBuffer_;
RPC_Status _rpcStatus_ = OPER_UNKNOWN;
Int32 _serverStatus_ = OPER_INVOKED_OK;

OutCome _returnedValue_;
_callBuffer_ << &Par_0_;
_callBuffer_ << &Par_1_;
_callBuffer_ << Par_2_;
_callBuffer_ << Par_3_;
_callBuffer_ << &Par_4_;
_callBuffer_ << &Par_5_;

_rpcStatus_ = _clientManager.call(57978364, _callBuffer_, _serverStatus_, _replyBuffer_);
if (_rpcStatus_ == OPER_DONE)
{
int* Par_4_Ptr = &Par_4_;
_replyBuffer_ >> Par_4_Ptr;
unsigned long* Par_5_Ptr = &Par_5_;
_replyBuffer_ >> Par_5_Ptr;
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

OutCome RemoteNameServer::removeCompositeObject ( const Uid& Par_0_, const Uid& Par_1_, const char* Par_2_, pid_t Par_3_, int& Par_4_, unsigned long& Par_5_)
{
RpcBuffer _rvBuffer_;
RpcBuffer _callBuffer_(_myHashVal);
RpcBuffer _replyBuffer_;
RPC_Status _rpcStatus_ = OPER_UNKNOWN;
Int32 _serverStatus_ = OPER_INVOKED_OK;

OutCome _returnedValue_;
_callBuffer_ << &Par_0_;
_callBuffer_ << &Par_1_;
_callBuffer_ << Par_2_;
_callBuffer_ << Par_3_;
_callBuffer_ << &Par_4_;
_callBuffer_ << &Par_5_;

_rpcStatus_ = _clientManager.call(55701260, _callBuffer_, _serverStatus_, _replyBuffer_);
if (_rpcStatus_ == OPER_DONE)
{
int* Par_4_Ptr = &Par_4_;
_replyBuffer_ >> Par_4_Ptr;
unsigned long* Par_5_Ptr = &Par_5_;
_replyBuffer_ >> Par_5_Ptr;
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

OutCome RemoteNameServer::removeCompositeObject ( const Uid& Par_0_, const ReplicaDescriptor& Par_1_, const char* Par_2_, pid_t Par_3_, int& Par_4_, unsigned long& Par_5_)
{
RpcBuffer _rvBuffer_;
RpcBuffer _callBuffer_(_myHashVal);
RpcBuffer _replyBuffer_;
RPC_Status _rpcStatus_ = OPER_UNKNOWN;
Int32 _serverStatus_ = OPER_INVOKED_OK;

OutCome _returnedValue_;
_callBuffer_ << &Par_0_;
_callBuffer_ << &Par_1_;
_callBuffer_ << Par_2_;
_callBuffer_ << Par_3_;
_callBuffer_ << &Par_4_;
_callBuffer_ << &Par_5_;

_rpcStatus_ = _clientManager.call(17515596, _callBuffer_, _serverStatus_, _replyBuffer_);
if (_rpcStatus_ == OPER_DONE)
{
int* Par_4_Ptr = &Par_4_;
_replyBuffer_ >> Par_4_Ptr;
unsigned long* Par_5_Ptr = &Par_5_;
_replyBuffer_ >> Par_5_Ptr;
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

void RemoteNameServer::storeName ( Boolean& Par_0_, Buffer Par_1_, Buffer Par_2_, int& Par_3_, unsigned long& Par_4_)
{
RpcBuffer _callBuffer_(_myHashVal);
RpcBuffer _replyBuffer_;
RPC_Status _rpcStatus_ = OPER_UNKNOWN;
Int32 _serverStatus_ = OPER_INVOKED_OK;

_callBuffer_ << &Par_0_;
_callBuffer_ << Par_1_;
_callBuffer_ << Par_2_;
_callBuffer_ << &Par_3_;
_callBuffer_ << &Par_4_;

_rpcStatus_ = _clientManager.call(139252988, _callBuffer_, _serverStatus_, _replyBuffer_);
if (_rpcStatus_ == OPER_DONE)
{
Boolean* Par_0_Ptr = &Par_0_;
_replyBuffer_ >> Par_0_Ptr;
int* Par_3_Ptr = &Par_3_;
_replyBuffer_ >> Par_3_Ptr;
unsigned long* Par_4_Ptr = &Par_4_;
_replyBuffer_ >> Par_4_Ptr;
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

void RemoteNameServer::fetchName ( Boolean& Par_0_, Buffer Par_1_, Buffer& Par_2_, int& Par_3_, unsigned long& Par_4_)
{
RpcBuffer _callBuffer_(_myHashVal);
RpcBuffer _replyBuffer_;
RPC_Status _rpcStatus_ = OPER_UNKNOWN;
Int32 _serverStatus_ = OPER_INVOKED_OK;

_callBuffer_ << &Par_0_;
_callBuffer_ << Par_1_;
_callBuffer_ << &Par_2_;
_callBuffer_ << &Par_3_;
_callBuffer_ << &Par_4_;

_rpcStatus_ = _clientManager.call(32268428, _callBuffer_, _serverStatus_, _replyBuffer_);
if (_rpcStatus_ == OPER_DONE)
{
Boolean* Par_0_Ptr = &Par_0_;
_replyBuffer_ >> Par_0_Ptr;
Buffer* Par_2_Ptr = &Par_2_;
_replyBuffer_ >> Par_2_Ptr;
int* Par_3_Ptr = &Par_3_;
_replyBuffer_ >> Par_3_Ptr;
unsigned long* Par_4_Ptr = &Par_4_;
_replyBuffer_ >> Par_4_Ptr;
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

void RemoteNameServer::removeName ( Boolean& Par_0_, Buffer Par_1_, int& Par_2_, unsigned long& Par_3_)
{
RpcBuffer _callBuffer_(_myHashVal);
RpcBuffer _replyBuffer_;
RPC_Status _rpcStatus_ = OPER_UNKNOWN;
Int32 _serverStatus_ = OPER_INVOKED_OK;

_callBuffer_ << &Par_0_;
_callBuffer_ << Par_1_;
_callBuffer_ << &Par_2_;
_callBuffer_ << &Par_3_;

_rpcStatus_ = _clientManager.call(234338428, _callBuffer_, _serverStatus_, _replyBuffer_);
if (_rpcStatus_ == OPER_DONE)
{
Boolean* Par_0_Ptr = &Par_0_;
_replyBuffer_ >> Par_0_Ptr;
int* Par_2_Ptr = &Par_2_;
_replyBuffer_ >> Par_2_Ptr;
unsigned long* Par_3_Ptr = &Par_3_;
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

void RemoteNameServer::replaceName ( Boolean& Par_0_, Buffer Par_1_, Buffer Par_2_, int& Par_3_, unsigned long& Par_4_)
{
RpcBuffer _callBuffer_(_myHashVal);
RpcBuffer _replyBuffer_;
RPC_Status _rpcStatus_ = OPER_UNKNOWN;
Int32 _serverStatus_ = OPER_INVOKED_OK;

_callBuffer_ << &Par_0_;
_callBuffer_ << Par_1_;
_callBuffer_ << Par_2_;
_callBuffer_ << &Par_3_;
_callBuffer_ << &Par_4_;

_rpcStatus_ = _clientManager.call(14100364, _callBuffer_, _serverStatus_, _replyBuffer_);
if (_rpcStatus_ == OPER_DONE)
{
Boolean* Par_0_Ptr = &Par_0_;
_replyBuffer_ >> Par_0_Ptr;
int* Par_3_Ptr = &Par_3_;
_replyBuffer_ >> Par_3_Ptr;
unsigned long* Par_4_Ptr = &Par_4_;
_replyBuffer_ >> Par_4_Ptr;
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

void RemoteNameServer::getNextName ( Boolean& Par_0_, Buffer& Par_1_, int& Par_2_, int& Par_3_, unsigned long& Par_4_)
{
RpcBuffer _callBuffer_(_myHashVal);
RpcBuffer _replyBuffer_;
RPC_Status _rpcStatus_ = OPER_UNKNOWN;
Int32 _serverStatus_ = OPER_INVOKED_OK;

_callBuffer_ << &Par_0_;
_callBuffer_ << &Par_1_;
_callBuffer_ << &Par_2_;
_callBuffer_ << &Par_3_;
_callBuffer_ << &Par_4_;

_rpcStatus_ = _clientManager.call(264496828, _callBuffer_, _serverStatus_, _replyBuffer_);
if (_rpcStatus_ == OPER_DONE)
{
Boolean* Par_0_Ptr = &Par_0_;
_replyBuffer_ >> Par_0_Ptr;
Buffer* Par_1_Ptr = &Par_1_;
_replyBuffer_ >> Par_1_Ptr;
int* Par_2_Ptr = &Par_2_;
_replyBuffer_ >> Par_2_Ptr;
int* Par_3_Ptr = &Par_3_;
_replyBuffer_ >> Par_3_Ptr;
unsigned long* Par_4_Ptr = &Par_4_;
_replyBuffer_ >> Par_4_Ptr;
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

void RemoteNameServer::reset ( int& Par_0_)
{
RpcBuffer _callBuffer_(_myHashVal);
RpcBuffer _replyBuffer_;
RPC_Status _rpcStatus_ = OPER_UNKNOWN;
Int32 _serverStatus_ = OPER_INVOKED_OK;

_callBuffer_ << &Par_0_;

_rpcStatus_ = _clientManager.call(20145801, _callBuffer_, _serverStatus_, _replyBuffer_);
if (_rpcStatus_ == OPER_DONE)
{
int* Par_0_Ptr = &Par_0_;
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

Boolean RemoteNameServer::save_state ( ObjectState& Par_0_, ObjectType Par_1_)
{
RpcBuffer _rvBuffer_;
RpcBuffer _callBuffer_(_myHashVal);
RpcBuffer _replyBuffer_;
RPC_Status _rpcStatus_ = OPER_UNKNOWN;
Int32 _serverStatus_ = OPER_INVOKED_OK;

Boolean _returnedValue_;
_callBuffer_ << &Par_0_;
_callBuffer_ << Par_1_;

_rpcStatus_ = _clientManager.call(173681173, _callBuffer_, _serverStatus_, _replyBuffer_);
if (_rpcStatus_ == OPER_DONE)
{
ObjectState* Par_0_Ptr = &Par_0_;
_replyBuffer_ >> Par_0_Ptr;
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

Boolean RemoteNameServer::restore_state ( ObjectState& Par_0_, ObjectType Par_1_)
{
RpcBuffer _rvBuffer_;
RpcBuffer _callBuffer_(_myHashVal);
RpcBuffer _replyBuffer_;
RPC_Status _rpcStatus_ = OPER_UNKNOWN;
Int32 _serverStatus_ = OPER_INVOKED_OK;

Boolean _returnedValue_;
_callBuffer_ << &Par_0_;
_callBuffer_ << Par_1_;

_rpcStatus_ = _clientManager.call(165059637, _callBuffer_, _serverStatus_, _replyBuffer_);
if (_rpcStatus_ == OPER_DONE)
{
ObjectState* Par_0_Ptr = &Par_0_;
_replyBuffer_ >> Par_0_Ptr;
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

const TypeName RemoteNameServer::type ( ) const 
{
RpcBuffer _rvBuffer_;
RpcBuffer _callBuffer_(_myHashVal);
RpcBuffer _replyBuffer_;
RPC_Status _rpcStatus_ = OPER_UNKNOWN;
Int32 _serverStatus_ = OPER_INVOKED_OK;

TypeName _returnedValue_ = 0;

_rpcStatus_ = _clientManager.call(152344342, _callBuffer_, _serverStatus_, _replyBuffer_);
if (_rpcStatus_ == OPER_DONE)
{
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
