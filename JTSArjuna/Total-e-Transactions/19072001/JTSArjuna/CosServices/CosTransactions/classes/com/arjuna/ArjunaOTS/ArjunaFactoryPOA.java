package com.arjuna.ArjunaOTS;

abstract public class ArjunaFactoryPOA
    extends org.omg.PortableServer.Servant
    implements org.omg.CORBA.portable.InvokeHandler, ArjunaFactoryOperations, org.omg.CosTransactions.TransactionFactoryOperations
{
    public com.arjuna.ArjunaOTS.ArjunaFactory _this()
    {
        return com.arjuna.ArjunaOTS.ArjunaFactoryHelper.narrow(_this_object());
    }

    public com.arjuna.ArjunaOTS.ArjunaFactory _this(
        org.omg.CORBA.ORB orb
)
    {
        return com.arjuna.ArjunaOTS.ArjunaFactoryHelper.narrow(_this_object(orb));
    }

    public org.omg.CORBA.portable.OutputStream _invoke(String opName, 
        org.omg.CORBA.portable.InputStream input, 
        org.omg.CORBA.portable.ResponseHandler handler    )
    {
        java.lang.Object[] _params = {input, handler, this};
        java.lang.reflect.Method _meth = (java.lang.reflect.Method)_ArjunaFactory_hashtable.get(opName);
        if (_meth == null)
            throw new org.omg.CORBA.BAD_OPERATION();
        java.lang.Object return_value = null;
        try {
            return_value = _meth.invoke(this, _params);
        } catch (java.lang.reflect.InvocationTargetException ite) {
            throw (RuntimeException)ite.getTargetException();
        } catch (java.lang.IllegalAccessException iae) {
            throw new org.omg.CORBA.BAD_OPERATION();
        }
        return (org.omg.CORBA.portable.OutputStream)return_value;
    }

    public static org.omg.CORBA.portable.OutputStream _numberOfTransactions_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        com.arjuna.ArjunaOTS.ArjunaFactoryOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            com.arjuna.ArjunaOTS.TransactionType t;
            t = com.arjuna.ArjunaOTS.TransactionTypeHelper.read(_input);
            org.omg.CosTransactions.otid_t[] _result = _obj.numberOfTransactions(t);
            _output = handler.createReply();
            com.arjuna.ArjunaOTS.txIdsHelper.write(_output, _result);
                        }
        catch (org.omg.CosTransactions.Inactive _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosTransactions.InactiveHelper.write(_output, _exception0);
            return _output;
        }
        catch (org.omg.CosTransactions.NoTransaction _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosTransactions.NoTransactionHelper.write(_output, _exception0);
            return _output;
        }
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _getChildTransactions_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        com.arjuna.ArjunaOTS.ArjunaFactoryOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            org.omg.CosTransactions.otid_t parent;
            parent = org.omg.CosTransactions.otid_tHelper.read(_input);
            org.omg.CosTransactions.otid_t[] _result = _obj.getChildTransactions(parent);
            _output = handler.createReply();
            com.arjuna.ArjunaOTS.txIdsHelper.write(_output, _result);
                        }
        catch (org.omg.CosTransactions.Inactive _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosTransactions.InactiveHelper.write(_output, _exception0);
            return _output;
        }
        catch (org.omg.CosTransactions.NoTransaction _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosTransactions.NoTransactionHelper.write(_output, _exception0);
            return _output;
        }
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _getCurrentStatus_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        com.arjuna.ArjunaOTS.ArjunaFactoryOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            org.omg.CosTransactions.otid_t uid;
            uid = org.omg.CosTransactions.otid_tHelper.read(_input);
            org.omg.CosTransactions.Status _result = _obj.getCurrentStatus(uid);
            _output = handler.createReply();
            org.omg.CosTransactions.StatusHelper.write(_output, _result);
                        }
        catch (org.omg.CosTransactions.NoTransaction _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosTransactions.NoTransactionHelper.write(_output, _exception0);
            return _output;
        }
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _getStatus_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        com.arjuna.ArjunaOTS.ArjunaFactoryOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            org.omg.CosTransactions.otid_t uid;
            uid = org.omg.CosTransactions.otid_tHelper.read(_input);
            org.omg.CosTransactions.Status _result = _obj.getStatus(uid);
            _output = handler.createReply();
            org.omg.CosTransactions.StatusHelper.write(_output, _result);
                        }
        catch (org.omg.CosTransactions.NoTransaction _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosTransactions.NoTransactionHelper.write(_output, _exception0);
            return _output;
        }
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _getGlobalInfo_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        com.arjuna.ArjunaOTS.ArjunaFactoryOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        com.arjuna.ArjunaOTS.GlobalTransactionInfo _result = _obj.getGlobalInfo();
        _output = handler.createReply();
        com.arjuna.ArjunaOTS.GlobalTransactionInfoHelper.write(_output, _result);
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _getTransactionInfo_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        com.arjuna.ArjunaOTS.ArjunaFactoryOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            org.omg.CosTransactions.otid_t uid;
            uid = org.omg.CosTransactions.otid_tHelper.read(_input);
            com.arjuna.ArjunaOTS.TransactionInfo _result = _obj.getTransactionInfo(uid);
            _output = handler.createReply();
            com.arjuna.ArjunaOTS.TransactionInfoHelper.write(_output, _result);
                        }
        catch (org.omg.CosTransactions.NoTransaction _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosTransactions.NoTransactionHelper.write(_output, _exception0);
            return _output;
        }
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _getTransaction_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        com.arjuna.ArjunaOTS.ArjunaFactoryOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            org.omg.CosTransactions.otid_t uid;
            uid = org.omg.CosTransactions.otid_tHelper.read(_input);
            org.omg.CosTransactions.Control _result = _obj.getTransaction(uid);
            _output = handler.createReply();
            org.omg.CosTransactions.ControlHelper.write(_output, _result);
                        }
        catch (org.omg.CosTransactions.NoTransaction _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosTransactions.NoTransactionHelper.write(_output, _exception0);
            return _output;
        }
        return _output;
    }


    public String[]  _all_interfaces(
        org.omg.PortableServer.POA poa,
        byte[] objectId
    )
    {
        String __ids[] =  {"IDL:arjuna.com/ArjunaOTS/ArjunaFactory:1.0", "IDL:omg.org/CosTransactions/TransactionFactory:1.0"};
        return __ids;
    }

    public static void _ArjunaFactory_init_hash(
        java.util.HashMap _hash
    )
    {
        java.lang.Class[] params = {org.omg.CORBA.portable.InputStream.class,
            org.omg.CORBA.portable.ResponseHandler.class,
            com.arjuna.ArjunaOTS.ArjunaFactoryOperations.class};
            try {
                _hash.put("numberOfTransactions", com.arjuna.ArjunaOTS.ArjunaFactoryPOA.class.getDeclaredMethod("_numberOfTransactions_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("getChildTransactions", com.arjuna.ArjunaOTS.ArjunaFactoryPOA.class.getDeclaredMethod("_getChildTransactions_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("getCurrentStatus", com.arjuna.ArjunaOTS.ArjunaFactoryPOA.class.getDeclaredMethod("_getCurrentStatus_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("getStatus", com.arjuna.ArjunaOTS.ArjunaFactoryPOA.class.getDeclaredMethod("_getStatus_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("getGlobalInfo", com.arjuna.ArjunaOTS.ArjunaFactoryPOA.class.getDeclaredMethod("_getGlobalInfo_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("getTransactionInfo", com.arjuna.ArjunaOTS.ArjunaFactoryPOA.class.getDeclaredMethod("_getTransactionInfo_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("getTransaction", com.arjuna.ArjunaOTS.ArjunaFactoryPOA.class.getDeclaredMethod("_getTransaction_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
        }

        private static java.util.HashMap _ArjunaFactory_hashtable;

        static
        {
            try { 
                _ArjunaFactory_hashtable = new java.util.HashMap();
                org.omg.CosTransactions.TransactionFactoryPOA._TransactionFactory_init_hash(_ArjunaFactory_hashtable);
                com.arjuna.ArjunaOTS.ArjunaFactoryPOA._ArjunaFactory_init_hash(_ArjunaFactory_hashtable);
            } catch (java.lang.Exception excep) {
                excep.printStackTrace();
            }
        }

}
