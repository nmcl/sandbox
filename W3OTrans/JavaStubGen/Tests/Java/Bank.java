import W3OTrans.RPC.ClientTCPPort;
import W3OTrans.RPC.PortAddress;
import W3OTrans.Common.InputBuffer;
import W3OTrans.Common.OutputBuffer;
import W3OTrans.StubGen.*;

import java.io.*;

import java.io.IOException;

public class Bank
{
public Bank ( ClientRpcManager _cRpcMan_ ) throws IOException
{
_clientManager = _cRpcMan_;
_myHashVal = new OutputBuffer();
_myHashVal.packInt(-1);

OutputBuffer _callBuffer_ = new OutputBuffer(_myHashVal);
InputBuffer _replyBuffer_ = new InputBuffer();
int _rpcStatus_ = RPC_Status.OPER_UNKNOWN;
intWrapper _serverStatus_ = new intWrapper(ServerErrCode.OPER_INVOKED_OK);


_rpcStatus_ = RPC_Status.OPER_DONE;
try {
_replyBuffer_ = _clientManager.call(65267078, _callBuffer_, _serverStatus_);
} catch (IOException e) {
_rpcStatus_ = RPC_Status.OPER_NOTDONE;
}
if (_rpcStatus_ == RPC_Status.OPER_DONE)
{
switch (_serverStatus_.get())
{
case ServerErrCode.OPER_INVOKED_OK:
break;
default:
_clientManager.rpcAbort(_rpcStatus_, _serverStatus_.get());
}
}
else
_clientManager.rpcAbort(_rpcStatus_, _serverStatus_.get());

}

public Bank ( intWrapper Par_0_, ClientRpcManager _cRpcMan_ ) throws IOException
{
_clientManager = _cRpcMan_;
_myHashVal = new OutputBuffer();
_myHashVal.packInt(-1);

OutputBuffer _callBuffer_ = new OutputBuffer(_myHashVal);
InputBuffer _replyBuffer_ = new InputBuffer();
int _rpcStatus_ = RPC_Status.OPER_UNKNOWN;
intWrapper _serverStatus_ = new intWrapper(ServerErrCode.OPER_INVOKED_OK);

intWrapper wrapperPar_0_ = new intWrapper(Par_0_);
wrapperPar_0_.packInto(_callBuffer_);


_rpcStatus_ = RPC_Status.OPER_DONE;
try {
_replyBuffer_ = _clientManager.call(238968505, _callBuffer_, _serverStatus_);
} catch (IOException e) {
_rpcStatus_ = RPC_Status.OPER_NOTDONE;
}
if (_rpcStatus_ == RPC_Status.OPER_DONE)
{
Par_0_.unpackFrom(_replyBuffer_);

switch (_serverStatus_.get())
{
case ServerErrCode.OPER_INVOKED_OK:
break;
default:
_clientManager.rpcAbort(_rpcStatus_, _serverStatus_.get());
}
}
else
_clientManager.rpcAbort(_rpcStatus_, _serverStatus_.get());

}

public void finalize ( ) throws IOException
{
OutputBuffer _callBuffer_ = new OutputBuffer(_myHashVal);
InputBuffer _replyBuffer_ = new InputBuffer();
int _rpcStatus_ = RPC_Status.OPER_UNKNOWN;
intWrapper _serverStatus_ = new intWrapper(ServerErrCode.OPER_INVOKED_OK);


_rpcStatus_ = RPC_Status.OPER_DONE;
try {
_replyBuffer_ = _clientManager.call(97777030, _callBuffer_, _serverStatus_);
} catch (IOException e) {
_rpcStatus_ = RPC_Status.OPER_NOTDONE;
}
if (_rpcStatus_ == RPC_Status.OPER_DONE)
{
switch (_serverStatus_.get())
{
case ServerErrCode.OPER_INVOKED_OK:
break;
default:
_clientManager.rpcAbort(_rpcStatus_, _serverStatus_.get());
}
}
else
_clientManager.rpcAbort(_rpcStatus_, _serverStatus_.get());

}

public int insert ( int pin, int amount) throws IOException
{
InputBuffer _rvBuffer_ = new InputBuffer();
OutputBuffer _callBuffer_ = new OutputBuffer(_myHashVal);
InputBuffer _replyBuffer_ = new InputBuffer();
int _rpcStatus_ = RPC_Status.OPER_UNKNOWN;
intWrapper _serverStatus_ = new intWrapper(ServerErrCode.OPER_INVOKED_OK);

int _returnedValue_ = -1;
intWrapper wrapperpin = new intWrapper(pin);
wrapperpin.packInto(_callBuffer_);

intWrapper wrapperamount = new intWrapper(amount);
wrapperamount.packInto(_callBuffer_);


_rpcStatus_ = RPC_Status.OPER_DONE;
try {
_replyBuffer_ = _clientManager.call(30214553, _callBuffer_, _serverStatus_);
} catch (IOException e) {
_rpcStatus_ = RPC_Status.OPER_NOTDONE;
}
if (_rpcStatus_ == RPC_Status.OPER_DONE)
{
switch (_serverStatus_.get())
{
case ServerErrCode.OPER_INVOKED_OK:
_replyBuffer_.unpackFrom(_rvBuffer_);

intWrapper _returnedValue_0_ = new intWrapper();
_returnedValue_0_.unpackFrom(_rvBuffer_);
_returnedValue_ = _returnedValue_0_.get();

break;
default:
_clientManager.rpcAbort(_rpcStatus_, _serverStatus_.get());
}
}
else
_clientManager.rpcAbort(_rpcStatus_, _serverStatus_.get());

return _returnedValue_;
}

public int withdraw ( int pin, doubleWrapper amount) throws IOException
{
InputBuffer _rvBuffer_ = new InputBuffer();
OutputBuffer _callBuffer_ = new OutputBuffer(_myHashVal);
InputBuffer _replyBuffer_ = new InputBuffer();
int _rpcStatus_ = RPC_Status.OPER_UNKNOWN;
intWrapper _serverStatus_ = new intWrapper(ServerErrCode.OPER_INVOKED_OK);

int _returnedValue_ = -1;
intWrapper wrapperpin = new intWrapper(pin);
wrapperpin.packInto(_callBuffer_);

doubleWrapper wrapperamount = new doubleWrapper(amount);
wrapperamount.packInto(_callBuffer_);


_rpcStatus_ = RPC_Status.OPER_DONE;
try {
_replyBuffer_ = _clientManager.call(153236404, _callBuffer_, _serverStatus_);
} catch (IOException e) {
_rpcStatus_ = RPC_Status.OPER_NOTDONE;
}
if (_rpcStatus_ == RPC_Status.OPER_DONE)
{
amount.unpackFrom(_replyBuffer_);

switch (_serverStatus_.get())
{
case ServerErrCode.OPER_INVOKED_OK:
_replyBuffer_.unpackFrom(_rvBuffer_);

intWrapper _returnedValue_0_ = new intWrapper();
_returnedValue_0_.unpackFrom(_rvBuffer_);
_returnedValue_ = _returnedValue_0_.get();

break;
default:
_clientManager.rpcAbort(_rpcStatus_, _serverStatus_.get());
}
}
else
_clientManager.rpcAbort(_rpcStatus_, _serverStatus_.get());

return _returnedValue_;
}

public int inspect ( int pin, intWrapper amount) throws IOException
{
InputBuffer _rvBuffer_ = new InputBuffer();
OutputBuffer _callBuffer_ = new OutputBuffer(_myHashVal);
InputBuffer _replyBuffer_ = new InputBuffer();
int _rpcStatus_ = RPC_Status.OPER_UNKNOWN;
intWrapper _serverStatus_ = new intWrapper(ServerErrCode.OPER_INVOKED_OK);

int _returnedValue_ = -1;
intWrapper wrapperpin = new intWrapper(pin);
wrapperpin.packInto(_callBuffer_);

intWrapper wrapperamount = new intWrapper(amount);
wrapperamount.packInto(_callBuffer_);


_rpcStatus_ = RPC_Status.OPER_DONE;
try {
_replyBuffer_ = _clientManager.call(227927945, _callBuffer_, _serverStatus_);
} catch (IOException e) {
_rpcStatus_ = RPC_Status.OPER_NOTDONE;
}
if (_rpcStatus_ == RPC_Status.OPER_DONE)
{
amount.unpackFrom(_replyBuffer_);

switch (_serverStatus_.get())
{
case ServerErrCode.OPER_INVOKED_OK:
_replyBuffer_.unpackFrom(_rvBuffer_);

intWrapper _returnedValue_0_ = new intWrapper();
_returnedValue_0_.unpackFrom(_rvBuffer_);
_returnedValue_ = _returnedValue_0_.get();

break;
default:
_clientManager.rpcAbort(_rpcStatus_, _serverStatus_.get());
}
}
else
_clientManager.rpcAbort(_rpcStatus_, _serverStatus_.get());

return _returnedValue_;
}

public int transfer ( int pinFrom, int pinTo, int amount) throws IOException
{
InputBuffer _rvBuffer_ = new InputBuffer();
OutputBuffer _callBuffer_ = new OutputBuffer(_myHashVal);
InputBuffer _replyBuffer_ = new InputBuffer();
int _rpcStatus_ = RPC_Status.OPER_UNKNOWN;
intWrapper _serverStatus_ = new intWrapper(ServerErrCode.OPER_INVOKED_OK);

int _returnedValue_ = -1;
intWrapper wrapperpinFrom = new intWrapper(pinFrom);
wrapperpinFrom.packInto(_callBuffer_);

intWrapper wrapperpinTo = new intWrapper(pinTo);
wrapperpinTo.packInto(_callBuffer_);

intWrapper wrapperamount = new intWrapper(amount);
wrapperamount.packInto(_callBuffer_);


_rpcStatus_ = RPC_Status.OPER_DONE;
try {
_replyBuffer_ = _clientManager.call(231676393, _callBuffer_, _serverStatus_);
} catch (IOException e) {
_rpcStatus_ = RPC_Status.OPER_NOTDONE;
}
if (_rpcStatus_ == RPC_Status.OPER_DONE)
{
switch (_serverStatus_.get())
{
case ServerErrCode.OPER_INVOKED_OK:
_replyBuffer_.unpackFrom(_rvBuffer_);

intWrapper _returnedValue_0_ = new intWrapper();
_returnedValue_0_.unpackFrom(_rvBuffer_);
_returnedValue_ = _returnedValue_0_.get();

break;
default:
_clientManager.rpcAbort(_rpcStatus_, _serverStatus_.get());
}
}
else
_clientManager.rpcAbort(_rpcStatus_, _serverStatus_.get());

return _returnedValue_;
}


private ClientRpcManager _clientManager;
private OutputBuffer _myHashVal;

};
