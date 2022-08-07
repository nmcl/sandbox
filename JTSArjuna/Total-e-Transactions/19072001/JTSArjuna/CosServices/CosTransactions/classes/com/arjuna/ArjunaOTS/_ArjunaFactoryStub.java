//
// Java code automatically generated by the Orbix 2000 IDL Compiler
// Do not edit this code!
//

package com.arjuna.ArjunaOTS;

public class _ArjunaFactoryStub
    extends org.omg.CORBA.portable.ObjectImpl
    implements ArjunaFactory, org.omg.CosTransactions.TransactionFactory
{
    final public static java.lang.Class _opsClass = ArjunaFactoryOperations.class;


    public _ArjunaFactoryStub () {}

    public org.omg.CosTransactions.Control create(
        int time_out
    )
    {
        while (true) {
            if(!_is_local()) {
                org.omg.CORBA.portable.OutputStream _output = null;
                org.omg.CORBA.portable.InputStream _input = null;
                org.omg.CosTransactions.Control _result;
                try {
                    _output = _request("create", true);
                    _output.write_ulong(time_out);
                    _input = _invoke(_output);
                    _result = org.omg.CosTransactions.ControlHelper.read(_input);
                    return _result;
                }
                catch (org.omg.CORBA.portable.RemarshalException _exception) {
                    continue;
                }
                catch (org.omg.CORBA.portable.ApplicationException _exception)
                {
                    final java.lang.String _exception_id = _exception.getId();
                    throw new org.omg.CORBA.UNKNOWN("Unexpected User Exception;" + _exception_id);
                }
                finally {
                    _releaseReply(_input);
                }
            }
            else {
                org.omg.CORBA.portable.ServantObject _so = _servant_preinvoke("create", _opsClass);
                if (_so == null) {
                    continue;
                }
                ArjunaFactoryOperations _self = (ArjunaFactoryOperations)_so.servant;
                try {
                    return _self.create(time_out);
                }
                finally {
                    _servant_postinvoke(_so);
                }
            }
        }
    }


    public org.omg.CosTransactions.Control recreate(
        org.omg.CosTransactions.PropagationContext ctx
    )
    {
        while (true) {
            if(!_is_local()) {
                org.omg.CORBA.portable.OutputStream _output = null;
                org.omg.CORBA.portable.InputStream _input = null;
                org.omg.CosTransactions.Control _result;
                try {
                    _output = _request("recreate", true);
                    org.omg.CosTransactions.PropagationContextHelper.write(_output, ctx);
                    _input = _invoke(_output);
                    _result = org.omg.CosTransactions.ControlHelper.read(_input);
                    return _result;
                }
                catch (org.omg.CORBA.portable.RemarshalException _exception) {
                    continue;
                }
                catch (org.omg.CORBA.portable.ApplicationException _exception)
                {
                    final java.lang.String _exception_id = _exception.getId();
                    throw new org.omg.CORBA.UNKNOWN("Unexpected User Exception;" + _exception_id);
                }
                finally {
                    _releaseReply(_input);
                }
            }
            else {
                org.omg.CORBA.portable.ServantObject _so = _servant_preinvoke("recreate", _opsClass);
                if (_so == null) {
                    continue;
                }
                ArjunaFactoryOperations _self = (ArjunaFactoryOperations)_so.servant;
                try {
                    return _self.recreate(ctx);
                }
                finally {
                    _servant_postinvoke(_so);
                }
            }
        }
    }


    public org.omg.CosTransactions.otid_t[] numberOfTransactions(
        com.arjuna.ArjunaOTS.TransactionType t
    ) throws org.omg.CosTransactions.Inactive, org.omg.CosTransactions.NoTransaction
    {
        while (true) {
            if(!_is_local()) {
                org.omg.CORBA.portable.OutputStream _output = null;
                org.omg.CORBA.portable.InputStream _input = null;
                org.omg.CosTransactions.otid_t[] _result;
                try {
                    _output = _request("numberOfTransactions", true);
                    com.arjuna.ArjunaOTS.TransactionTypeHelper.write(_output, t);
                    _input = _invoke(_output);
                    _result = com.arjuna.ArjunaOTS.txIdsHelper.read(_input);
                    return _result;
                }
                catch (org.omg.CORBA.portable.RemarshalException _exception) {
                    continue;
                }
                catch (org.omg.CORBA.portable.ApplicationException _exception)
                {
                    final java.lang.String _exception_id = _exception.getId();
                        if (_exception_id.equals(org.omg.CosTransactions.InactiveHelper.id())) {
                            throw org.omg.CosTransactions.InactiveHelper.read(_exception.getInputStream());
                        }
                        if (_exception_id.equals(org.omg.CosTransactions.NoTransactionHelper.id())) {
                            throw org.omg.CosTransactions.NoTransactionHelper.read(_exception.getInputStream());
                        }
                    throw new org.omg.CORBA.UNKNOWN("Unexpected User Exception;" + _exception_id);
                }
                finally {
                    _releaseReply(_input);
                }
            }
            else {
                org.omg.CORBA.portable.ServantObject _so = _servant_preinvoke("numberOfTransactions", _opsClass);
                if (_so == null) {
                    continue;
                }
                ArjunaFactoryOperations _self = (ArjunaFactoryOperations)_so.servant;
                try {
                    return _self.numberOfTransactions(t);
                }
                finally {
                    _servant_postinvoke(_so);
                }
            }
        }
    }


    public org.omg.CosTransactions.otid_t[] getChildTransactions(
        org.omg.CosTransactions.otid_t parent
    ) throws org.omg.CosTransactions.Inactive, org.omg.CosTransactions.NoTransaction
    {
        while (true) {
            if(!_is_local()) {
                org.omg.CORBA.portable.OutputStream _output = null;
                org.omg.CORBA.portable.InputStream _input = null;
                org.omg.CosTransactions.otid_t[] _result;
                try {
                    _output = _request("getChildTransactions", true);
                    org.omg.CosTransactions.otid_tHelper.write(_output, parent);
                    _input = _invoke(_output);
                    _result = com.arjuna.ArjunaOTS.txIdsHelper.read(_input);
                    return _result;
                }
                catch (org.omg.CORBA.portable.RemarshalException _exception) {
                    continue;
                }
                catch (org.omg.CORBA.portable.ApplicationException _exception)
                {
                    final java.lang.String _exception_id = _exception.getId();
                        if (_exception_id.equals(org.omg.CosTransactions.InactiveHelper.id())) {
                            throw org.omg.CosTransactions.InactiveHelper.read(_exception.getInputStream());
                        }
                        if (_exception_id.equals(org.omg.CosTransactions.NoTransactionHelper.id())) {
                            throw org.omg.CosTransactions.NoTransactionHelper.read(_exception.getInputStream());
                        }
                    throw new org.omg.CORBA.UNKNOWN("Unexpected User Exception;" + _exception_id);
                }
                finally {
                    _releaseReply(_input);
                }
            }
            else {
                org.omg.CORBA.portable.ServantObject _so = _servant_preinvoke("getChildTransactions", _opsClass);
                if (_so == null) {
                    continue;
                }
                ArjunaFactoryOperations _self = (ArjunaFactoryOperations)_so.servant;
                try {
                    return _self.getChildTransactions(parent);
                }
                finally {
                    _servant_postinvoke(_so);
                }
            }
        }
    }


    public org.omg.CosTransactions.Status getCurrentStatus(
        org.omg.CosTransactions.otid_t uid
    ) throws org.omg.CosTransactions.NoTransaction
    {
        while (true) {
            if(!_is_local()) {
                org.omg.CORBA.portable.OutputStream _output = null;
                org.omg.CORBA.portable.InputStream _input = null;
                org.omg.CosTransactions.Status _result;
                try {
                    _output = _request("getCurrentStatus", true);
                    org.omg.CosTransactions.otid_tHelper.write(_output, uid);
                    _input = _invoke(_output);
                    _result = org.omg.CosTransactions.StatusHelper.read(_input);
                    return _result;
                }
                catch (org.omg.CORBA.portable.RemarshalException _exception) {
                    continue;
                }
                catch (org.omg.CORBA.portable.ApplicationException _exception)
                {
                    final java.lang.String _exception_id = _exception.getId();
                        if (_exception_id.equals(org.omg.CosTransactions.NoTransactionHelper.id())) {
                            throw org.omg.CosTransactions.NoTransactionHelper.read(_exception.getInputStream());
                        }
                    throw new org.omg.CORBA.UNKNOWN("Unexpected User Exception;" + _exception_id);
                }
                finally {
                    _releaseReply(_input);
                }
            }
            else {
                org.omg.CORBA.portable.ServantObject _so = _servant_preinvoke("getCurrentStatus", _opsClass);
                if (_so == null) {
                    continue;
                }
                ArjunaFactoryOperations _self = (ArjunaFactoryOperations)_so.servant;
                try {
                    return _self.getCurrentStatus(uid);
                }
                finally {
                    _servant_postinvoke(_so);
                }
            }
        }
    }


    public org.omg.CosTransactions.Status getStatus(
        org.omg.CosTransactions.otid_t uid
    ) throws org.omg.CosTransactions.NoTransaction
    {
        while (true) {
            if(!_is_local()) {
                org.omg.CORBA.portable.OutputStream _output = null;
                org.omg.CORBA.portable.InputStream _input = null;
                org.omg.CosTransactions.Status _result;
                try {
                    _output = _request("getStatus", true);
                    org.omg.CosTransactions.otid_tHelper.write(_output, uid);
                    _input = _invoke(_output);
                    _result = org.omg.CosTransactions.StatusHelper.read(_input);
                    return _result;
                }
                catch (org.omg.CORBA.portable.RemarshalException _exception) {
                    continue;
                }
                catch (org.omg.CORBA.portable.ApplicationException _exception)
                {
                    final java.lang.String _exception_id = _exception.getId();
                        if (_exception_id.equals(org.omg.CosTransactions.NoTransactionHelper.id())) {
                            throw org.omg.CosTransactions.NoTransactionHelper.read(_exception.getInputStream());
                        }
                    throw new org.omg.CORBA.UNKNOWN("Unexpected User Exception;" + _exception_id);
                }
                finally {
                    _releaseReply(_input);
                }
            }
            else {
                org.omg.CORBA.portable.ServantObject _so = _servant_preinvoke("getStatus", _opsClass);
                if (_so == null) {
                    continue;
                }
                ArjunaFactoryOperations _self = (ArjunaFactoryOperations)_so.servant;
                try {
                    return _self.getStatus(uid);
                }
                finally {
                    _servant_postinvoke(_so);
                }
            }
        }
    }


    public com.arjuna.ArjunaOTS.GlobalTransactionInfo getGlobalInfo()
    {
        while (true) {
            if(!_is_local()) {
                org.omg.CORBA.portable.OutputStream _output = null;
                org.omg.CORBA.portable.InputStream _input = null;
                com.arjuna.ArjunaOTS.GlobalTransactionInfo _result;
                try {
                    _output = _request("getGlobalInfo", true);
                    _input = _invoke(_output);
                    _result = com.arjuna.ArjunaOTS.GlobalTransactionInfoHelper.read(_input);
                    return _result;
                }
                catch (org.omg.CORBA.portable.RemarshalException _exception) {
                    continue;
                }
                catch (org.omg.CORBA.portable.ApplicationException _exception)
                {
                    final java.lang.String _exception_id = _exception.getId();
                    throw new org.omg.CORBA.UNKNOWN("Unexpected User Exception;" + _exception_id);
                }
                finally {
                    _releaseReply(_input);
                }
            }
            else {
                org.omg.CORBA.portable.ServantObject _so = _servant_preinvoke("getGlobalInfo", _opsClass);
                if (_so == null) {
                    continue;
                }
                ArjunaFactoryOperations _self = (ArjunaFactoryOperations)_so.servant;
                try {
                    return _self.getGlobalInfo();
                }
                finally {
                    _servant_postinvoke(_so);
                }
            }
        }
    }


    public com.arjuna.ArjunaOTS.TransactionInfo getTransactionInfo(
        org.omg.CosTransactions.otid_t uid
    ) throws org.omg.CosTransactions.NoTransaction
    {
        while (true) {
            if(!_is_local()) {
                org.omg.CORBA.portable.OutputStream _output = null;
                org.omg.CORBA.portable.InputStream _input = null;
                com.arjuna.ArjunaOTS.TransactionInfo _result;
                try {
                    _output = _request("getTransactionInfo", true);
                    org.omg.CosTransactions.otid_tHelper.write(_output, uid);
                    _input = _invoke(_output);
                    _result = com.arjuna.ArjunaOTS.TransactionInfoHelper.read(_input);
                    return _result;
                }
                catch (org.omg.CORBA.portable.RemarshalException _exception) {
                    continue;
                }
                catch (org.omg.CORBA.portable.ApplicationException _exception)
                {
                    final java.lang.String _exception_id = _exception.getId();
                        if (_exception_id.equals(org.omg.CosTransactions.NoTransactionHelper.id())) {
                            throw org.omg.CosTransactions.NoTransactionHelper.read(_exception.getInputStream());
                        }
                    throw new org.omg.CORBA.UNKNOWN("Unexpected User Exception;" + _exception_id);
                }
                finally {
                    _releaseReply(_input);
                }
            }
            else {
                org.omg.CORBA.portable.ServantObject _so = _servant_preinvoke("getTransactionInfo", _opsClass);
                if (_so == null) {
                    continue;
                }
                ArjunaFactoryOperations _self = (ArjunaFactoryOperations)_so.servant;
                try {
                    return _self.getTransactionInfo(uid);
                }
                finally {
                    _servant_postinvoke(_so);
                }
            }
        }
    }


    public org.omg.CosTransactions.Control getTransaction(
        org.omg.CosTransactions.otid_t uid
    ) throws org.omg.CosTransactions.NoTransaction
    {
        while (true) {
            if(!_is_local()) {
                org.omg.CORBA.portable.OutputStream _output = null;
                org.omg.CORBA.portable.InputStream _input = null;
                org.omg.CosTransactions.Control _result;
                try {
                    _output = _request("getTransaction", true);
                    org.omg.CosTransactions.otid_tHelper.write(_output, uid);
                    _input = _invoke(_output);
                    _result = org.omg.CosTransactions.ControlHelper.read(_input);
                    return _result;
                }
                catch (org.omg.CORBA.portable.RemarshalException _exception) {
                    continue;
                }
                catch (org.omg.CORBA.portable.ApplicationException _exception)
                {
                    final java.lang.String _exception_id = _exception.getId();
                        if (_exception_id.equals(org.omg.CosTransactions.NoTransactionHelper.id())) {
                            throw org.omg.CosTransactions.NoTransactionHelper.read(_exception.getInputStream());
                        }
                    throw new org.omg.CORBA.UNKNOWN("Unexpected User Exception;" + _exception_id);
                }
                finally {
                    _releaseReply(_input);
                }
            }
            else {
                org.omg.CORBA.portable.ServantObject _so = _servant_preinvoke("getTransaction", _opsClass);
                if (_so == null) {
                    continue;
                }
                ArjunaFactoryOperations _self = (ArjunaFactoryOperations)_so.servant;
                try {
                    return _self.getTransaction(uid);
                }
                finally {
                    _servant_postinvoke(_so);
                }
            }
        }
    }


    public static final String _interfaces[] =
    {"IDL:omg.org/CosTransactions/TransactionFactory:1.0", "IDL:arjuna.com/ArjunaOTS/ArjunaFactory:1.0"};

    public String[] _ids()
    {
        return _interfaces;
    }

}
