package com.arjuna.ArjunaOTS;

abstract public class CurrentPOA
    extends org.omg.PortableServer.Servant
    implements org.omg.CORBA.portable.InvokeHandler, CurrentOperations, org.omg.CosTransactions.CurrentOperations
{
    public com.arjuna.ArjunaOTS.Current _this()
    {
        return com.arjuna.ArjunaOTS.CurrentHelper.narrow(_this_object());
    }

    public com.arjuna.ArjunaOTS.Current _this(
        org.omg.CORBA.ORB orb
)
    {
        return com.arjuna.ArjunaOTS.CurrentHelper.narrow(_this_object(orb));
    }

    public org.omg.CORBA.portable.OutputStream _invoke(String opName, 
        org.omg.CORBA.portable.InputStream input, 
        org.omg.CORBA.portable.ResponseHandler handler    )
    {
        java.lang.Object[] _params = {input, handler, this};
        java.lang.reflect.Method _meth = (java.lang.reflect.Method)_Current_hashtable.get(opName);
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

    public static org.omg.CORBA.portable.OutputStream _get_timeout_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        com.arjuna.ArjunaOTS.CurrentOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        int _result = _obj.get_timeout();
        _output = handler.createReply();
        _output.write_long(_result);
        return _output;
    }


    public String[]  _all_interfaces(
        org.omg.PortableServer.POA poa,
        byte[] objectId
    )
    {
        String __ids[] =  {"IDL:arjuna.com/ArjunaOTS/Current:1.0", "IDL:omg.org/CosTransactions/Current:1.0"};
        return __ids;
    }

    public static void _Current_init_hash(
        java.util.HashMap _hash
    )
    {
        java.lang.Class[] params = {org.omg.CORBA.portable.InputStream.class,
            org.omg.CORBA.portable.ResponseHandler.class,
            com.arjuna.ArjunaOTS.CurrentOperations.class};
            try {
                _hash.put("get_timeout", com.arjuna.ArjunaOTS.CurrentPOA.class.getDeclaredMethod("_get_timeout_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
        }

        private static java.util.HashMap _Current_hashtable;

        static
        {
            try { 
                _Current_hashtable = new java.util.HashMap();
                org.omg.CosTransactions.CurrentPOA._Current_init_hash(_Current_hashtable);
                com.arjuna.ArjunaOTS.CurrentPOA._Current_init_hash(_Current_hashtable);
            } catch (java.lang.Exception excep) {
                excep.printStackTrace();
            }
        }

}
