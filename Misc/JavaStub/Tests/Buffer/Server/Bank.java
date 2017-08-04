import RPC.ClientTCPPort;
import RPC.PortAddress;
import RPC.InputBuffer;
import RPC.OutputBuffer;
import Wrappers.*;

import java.io.*;

import java.io.IOException;

public class Bank
{
public Bank ( ClientRpcManager _cRpcMan_ )
{
_clientManager = _cRpcMan_;
_myHashVal = new OutputBuffer();
_myHashVal.packInt(-1);

OutputBuffer _callBuffer_ = new OutputBuffer();
InputBuffer _replyBuffer_ = new InputBuffer();
int _rpcStatus_ = OPER_UNKNOWN;
int _serverStatus_ = OPER_INVOKED_OK;

_callBuffer_.packBuffer(_myHashVal);

_rpcStatus_ = _clientManager.call(65267078, _callBuffer_, _serverStatus_, _replyBuffer_);
if (_rpcStatus_ == OPER_DONE)
{
switch (_serverStatus_)
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

public Bank ( intWrapper Par_0_, ClientRpcManager _cRpcMan_ )
{
_clientManager = _cRpcMan_;
_myHashVal = new OutputBuffer();
_myHashVal.packInt(-1);

OutputBuffer _callBuffer_ = new OutputBuffer();
InputBuffer _replyBuffer_ = new InputBuffer();
int _rpcStatus_ = OPER_UNKNOWN;
int _serverStatus_ = OPER_INVOKED_OK;

_callBuffer_.packBuffer(_myHashVal);
intWrapper wrapperPar_0_ = new intWrapper(Par_0_);
wrapperPar_0_.packInto(_callBuffer_);


_rpcStatus_ = _clientManager.call(238968505, _callBuffer_, _serverStatus_, _replyBuffer_);
if (_rpcStatus_ == OPER_DONE)
{
Par_0_.unpackFrom(_replyBuffer_);

switch (_serverStatus_)
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

public void final ( )
{
OutputBuffer _callBuffer_ = new OutputBuffer();
InputBuffer _replyBuffer_ = new InputBuffer();
int _rpcStatus_ = OPER_UNKNOWN;
int _serverStatus_ = OPER_INVOKED_OK;

_callBuffer_.packBuffer(_myHashVal);

_rpcStatus_ = _clientManager.call(97777030, _callBuffer_, _serverStatus_, _replyBuffer_);
if (_rpcStatus_ == OPER_DONE)
{
switch (_serverStatus_)
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

public int insert ( int pin, int amount)
{
InputBuffer _rvBuffer_ = new InputBuffer();
OutputBuffer _callBuffer_ = new OutputBuffer();
InputBuffer _replyBuffer_ = new InputBuffer();
int _rpcStatus_ = OPER_UNKNOWN;
int _serverStatus_ = OPER_INVOKED_OK;

_callBuffer_.packBuffer(_myHashVal);
int _returnedValue_ = -1;
int wrapperpin = new int(pin);
wrapperpin.packInto(_callBuffer_);

int wrapperamount = new int(amount);
wrapperamount.packInto(_callBuffer_);


_rpcStatus_ = _clientManager.call(30214553, _callBuffer_, _serverStatus_, _replyBuffer_);
if (_rpcStatus_ == OPER_DONE)
{
switch (_serverStatus_)
{
case OPER_INVOKED_OK:
_replyBuffer_.unpackFrom(_rvBuffer_);

intWrapper _returnedValue_0_ = new intWrapper();
_returnedValue_0_.unpackFrom(_rvBuffer_);
_returnedValue_ = _returnedValue_0_0.value();

break;
default:
_clientManager.rpcAbort(_rpcStatus_, _serverStatus_);
}
}
else
_clientManager.rpcAbort(_rpcStatus_, _serverStatus_);

return _returnedValue_;
}

public int withdraw ( int pin, doubleWrapper amount)
{
InputBuffer _rvBuffer_ = new InputBuffer();
OutputBuffer _callBuffer_ = new OutputBuffer();
InputBuffer _replyBuffer_ = new InputBuffer();
int _rpcStatus_ = OPER_UNKNOWN;
int _serverStatus_ = OPER_INVOKED_OK;

_callBuffer_.packBuffer(_myHashVal);
int _returnedValue_ = -1;
int wrapperpin = new int(pin);
wrapperpin.packInto(_callBuffer_);

doubleWrapper wrapperamount = new doubleWrapper(amount);
wrapperamount.packInto(_callBuffer_);


_rpcStatus_ = _clientManager.call(153236404, _callBuffer_, _serverStatus_, _replyBuffer_);
if (_rpcStatus_ == OPER_DONE)
{
amount.unpackFrom(_replyBuffer_);

switch (_serverStatus_)
{
case OPER_INVOKED_OK:
_replyBuffer_.unpackFrom(_rvBuffer_);

intWrapper _returnedValue_0_ = new intWrapper();
_returnedValue_0_.unpackFrom(_rvBuffer_);
_returnedValue_ = _returnedValue_0_0.value();

break;
default:
_clientManager.rpcAbort(_rpcStatus_, _serverStatus_);
}
}
else
_clientManager.rpcAbort(_rpcStatus_, _serverStatus_);

return _returnedValue_;
}

public int inspect ( int pin, intWrapper amount)
{
InputBuffer _rvBuffer_ = new InputBuffer();
OutputBuffer _callBuffer_ = new OutputBuffer();
InputBuffer _replyBuffer_ = new InputBuffer();
int _rpcStatus_ = OPER_UNKNOWN;
int _serverStatus_ = OPER_INVOKED_OK;

_callBuffer_.packBuffer(_myHashVal);
int _returnedValue_ = -1;
int wrapperpin = new int(pin);
wrapperpin.packInto(_callBuffer_);

intWrapper wrapperamount = new intWrapper(amount);
wrapperamount.packInto(_callBuffer_);


_rpcStatus_ = _clientManager.call(227927945, _callBuffer_, _serverStatus_, _replyBuffer_);
if (_rpcStatus_ == OPER_DONE)
{
amount.unpackFrom(_replyBuffer_);

switch (_serverStatus_)
{
case OPER_INVOKED_OK:
_replyBuffer_.unpackFrom(_rvBuffer_);

intWrapper _returnedValue_0_ = new intWrapper();
_returnedValue_0_.unpackFrom(_rvBuffer_);
_returnedValue_ = _returnedValue_0_0.value();

break;
default:
_clientManager.rpcAbort(_rpcStatus_, _serverStatus_);
}
}
else
_clientManager.rpcAbort(_rpcStatus_, _serverStatus_);

return _returnedValue_;
}

public int transfer ( int pinFrom, int pinTo, int amount)
{
InputBuffer _rvBuffer_ = new InputBuffer();
OutputBuffer _callBuffer_ = new OutputBuffer();
InputBuffer _replyBuffer_ = new InputBuffer();
int _rpcStatus_ = OPER_UNKNOWN;
int _serverStatus_ = OPER_INVOKED_OK;

_callBuffer_.packBuffer(_myHashVal);
int _returnedValue_ = -1;
int wrapperpinFrom = new int(pinFrom);
wrapperpinFrom.packInto(_callBuffer_);

int wrapperpinTo = new int(pinTo);
wrapperpinTo.packInto(_callBuffer_);

int wrapperamount = new int(amount);
wrapperamount.packInto(_callBuffer_);


_rpcStatus_ = _clientManager.call(231676393, _callBuffer_, _serverStatus_, _replyBuffer_);
if (_rpcStatus_ == OPER_DONE)
{
switch (_serverStatus_)
{
case OPER_INVOKED_OK:
_replyBuffer_.unpackFrom(_rvBuffer_);

intWrapper _returnedValue_0_ = new intWrapper();
_returnedValue_0_.unpackFrom(_rvBuffer_);
_returnedValue_ = _returnedValue_0_0.value();

break;
default:
_clientManager.rpcAbort(_rpcStatus_, _serverStatus_);
}
}
else
_clientManager.rpcAbort(_rpcStatus_, _serverStatus_);

return _returnedValue_;
}


private ClientRpcManager _clientManager_;
private OutputBuffer _myHashVal;

};
