package org.omg.CosTransactions;

abstract public class ControlPOA
    extends org.omg.PortableServer.Servant
    implements org.omg.CORBA.portable.InvokeHandler, ControlOperations
{
    public org.omg.CosTransactions.Control _this()
    {
        return org.omg.CosTransactions.ControlHelper.narrow(_this_object());
    }

    public org.omg.CosTransactions.Control _this(
        org.omg.CORBA.ORB orb
)
    {
        return org.omg.CosTransactions.ControlHelper.narrow(_this_object(orb));
    }

    public org.omg.CORBA.portable.OutputStream _invoke(String opName, 
        org.omg.CORBA.portable.InputStream input, 
        org.omg.CORBA.portable.ResponseHandler handler    )
    {
        java.lang.Object[] _params = {input, handler, this};
        java.lang.reflect.Method _meth = (java.lang.reflect.Method)_Control_hashtable.get(opName);
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

    public static org.omg.CORBA.portable.OutputStream _get_terminator_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosTransactions.ControlOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            org.omg.CosTransactions.Terminator _result = _obj.get_terminator();
            _output = handler.createReply();
            org.omg.CosTransactions.TerminatorHelper.write(_output, _result);
            }
        catch (org.omg.CosTransactions.Unavailable _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosTransactions.UnavailableHelper.write(_output, _exception0);
            return _output;
        }
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _get_coordinator_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosTransactions.ControlOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            org.omg.CosTransactions.Coordinator _result = _obj.get_coordinator();
            _output = handler.createReply();
            org.omg.CosTransactions.CoordinatorHelper.write(_output, _result);
            }
        catch (org.omg.CosTransactions.Unavailable _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosTransactions.UnavailableHelper.write(_output, _exception0);
            return _output;
        }
        return _output;
    }


    public String[]  _all_interfaces(
        org.omg.PortableServer.POA poa,
        byte[] objectId
    )
    {
        String __ids[] =  {"IDL:omg.org/CosTransactions/Control:1.0"};
        return __ids;
    }

    public static void _Control_init_hash(
        java.util.HashMap _hash
    )
    {
        java.lang.Class[] params = {org.omg.CORBA.portable.InputStream.class,
            org.omg.CORBA.portable.ResponseHandler.class,
            org.omg.CosTransactions.ControlOperations.class};
            try {
                _hash.put("get_terminator", org.omg.CosTransactions.ControlPOA.class.getDeclaredMethod("_get_terminator_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("get_coordinator", org.omg.CosTransactions.ControlPOA.class.getDeclaredMethod("_get_coordinator_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
        }

        private static java.util.HashMap _Control_hashtable;

        static
        {
            try { 
                _Control_hashtable = new java.util.HashMap();
                org.omg.CosTransactions.ControlPOA._Control_init_hash(_Control_hashtable);
            } catch (java.lang.Exception excep) {
                excep.printStackTrace();
            }
        }

}
