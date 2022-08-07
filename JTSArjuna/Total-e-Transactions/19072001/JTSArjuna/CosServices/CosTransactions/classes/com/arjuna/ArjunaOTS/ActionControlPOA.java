package com.arjuna.ArjunaOTS;

abstract public class ActionControlPOA
    extends org.omg.PortableServer.Servant
    implements org.omg.CORBA.portable.InvokeHandler, ActionControlOperations, org.omg.CosTransactions.ControlOperations
{
    public com.arjuna.ArjunaOTS.ActionControl _this()
    {
        return com.arjuna.ArjunaOTS.ActionControlHelper.narrow(_this_object());
    }

    public com.arjuna.ArjunaOTS.ActionControl _this(
        org.omg.CORBA.ORB orb
)
    {
        return com.arjuna.ArjunaOTS.ActionControlHelper.narrow(_this_object(orb));
    }

    public org.omg.CORBA.portable.OutputStream _invoke(String opName, 
        org.omg.CORBA.portable.InputStream input, 
        org.omg.CORBA.portable.ResponseHandler handler    )
    {
        java.lang.Object[] _params = {input, handler, this};
        java.lang.reflect.Method _meth = (java.lang.reflect.Method)_ActionControl_hashtable.get(opName);
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

    public static org.omg.CORBA.portable.OutputStream _getParentControl_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        com.arjuna.ArjunaOTS.ActionControlOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            org.omg.CosTransactions.Control _result = _obj.getParentControl();
            _output = handler.createReply();
            org.omg.CosTransactions.ControlHelper.write(_output, _result);
            }
        catch (org.omg.CosTransactions.Unavailable _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosTransactions.UnavailableHelper.write(_output, _exception0);
            return _output;
        }
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _destroy_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        com.arjuna.ArjunaOTS.ActionControlOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            _obj.destroy();
            _output = handler.createReply();
            }
        catch (com.arjuna.ArjunaOTS.ActiveTransaction _exception0)
        {
            _output = handler.createExceptionReply();
            com.arjuna.ArjunaOTS.ActiveTransactionHelper.write(_output, _exception0);
            return _output;
        }
        catch (com.arjuna.ArjunaOTS.ActiveThreads _exception0)
        {
            _output = handler.createExceptionReply();
            com.arjuna.ArjunaOTS.ActiveThreadsHelper.write(_output, _exception0);
            return _output;
        }
        catch (com.arjuna.ArjunaOTS.BadControl _exception0)
        {
            _output = handler.createExceptionReply();
            com.arjuna.ArjunaOTS.BadControlHelper.write(_output, _exception0);
            return _output;
        }
        catch (com.arjuna.ArjunaOTS.Destroyed _exception0)
        {
            _output = handler.createExceptionReply();
            com.arjuna.ArjunaOTS.DestroyedHelper.write(_output, _exception0);
            return _output;
        }
        return _output;
    }


    public String[]  _all_interfaces(
        org.omg.PortableServer.POA poa,
        byte[] objectId
    )
    {
        String __ids[] =  {"IDL:arjuna.com/ArjunaOTS/ActionControl:1.0", "IDL:omg.org/CosTransactions/Control:1.0"};
        return __ids;
    }

    public static void _ActionControl_init_hash(
        java.util.HashMap _hash
    )
    {
        java.lang.Class[] params = {org.omg.CORBA.portable.InputStream.class,
            org.omg.CORBA.portable.ResponseHandler.class,
            com.arjuna.ArjunaOTS.ActionControlOperations.class};
            try {
                _hash.put("getParentControl", com.arjuna.ArjunaOTS.ActionControlPOA.class.getDeclaredMethod("_getParentControl_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("destroy", com.arjuna.ArjunaOTS.ActionControlPOA.class.getDeclaredMethod("_destroy_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
        }

        private static java.util.HashMap _ActionControl_hashtable;

        static
        {
            try { 
                _ActionControl_hashtable = new java.util.HashMap();
                com.arjuna.ArjunaOTS.ActionControlPOA._ActionControl_init_hash(_ActionControl_hashtable);
                org.omg.CosTransactions.ControlPOA._Control_init_hash(_ActionControl_hashtable);
            } catch (java.lang.Exception excep) {
                excep.printStackTrace();
            }
        }

}
