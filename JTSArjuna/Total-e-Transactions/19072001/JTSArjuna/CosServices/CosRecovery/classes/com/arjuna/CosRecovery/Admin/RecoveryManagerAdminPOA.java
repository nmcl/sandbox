package com.arjuna.CosRecovery.Admin;

abstract public class RecoveryManagerAdminPOA
    extends org.omg.PortableServer.Servant
    implements org.omg.CORBA.portable.InvokeHandler, RecoveryManagerAdminOperations
{
    public com.arjuna.CosRecovery.Admin.RecoveryManagerAdmin _this()
    {
        return com.arjuna.CosRecovery.Admin.RecoveryManagerAdminHelper.narrow(_this_object());
    }

    public com.arjuna.CosRecovery.Admin.RecoveryManagerAdmin _this(
        org.omg.CORBA.ORB orb
)
    {
        return com.arjuna.CosRecovery.Admin.RecoveryManagerAdminHelper.narrow(_this_object(orb));
    }

    public org.omg.CORBA.portable.OutputStream _invoke(String opName, 
        org.omg.CORBA.portable.InputStream input, 
        org.omg.CORBA.portable.ResponseHandler handler    )
    {
        java.lang.Object[] _params = {input, handler, this};
        java.lang.reflect.Method _meth = (java.lang.reflect.Method)_RecoveryManagerAdmin_hashtable.get(opName);
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

    public static org.omg.CORBA.portable.OutputStream _ping_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        com.arjuna.CosRecovery.Admin.RecoveryManagerAdminOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        int _result = _obj.ping();
        _output = handler.createReply();
        _output.write_long(_result);
        return _output;
    }


    public String[]  _all_interfaces(
        org.omg.PortableServer.POA poa,
        byte[] objectId
    )
    {
        String __ids[] =  {"IDL:CosRecovery.arjuna.com/Admin/RecoveryManagerAdmin:1.0"};
        return __ids;
    }

    public static void _RecoveryManagerAdmin_init_hash(
        java.util.HashMap _hash
    )
    {
        java.lang.Class[] params = {org.omg.CORBA.portable.InputStream.class,
            org.omg.CORBA.portable.ResponseHandler.class,
            com.arjuna.CosRecovery.Admin.RecoveryManagerAdminOperations.class};
            try {
                _hash.put("ping", com.arjuna.CosRecovery.Admin.RecoveryManagerAdminPOA.class.getDeclaredMethod("_ping_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
        }

        private static java.util.HashMap _RecoveryManagerAdmin_hashtable;

        static
        {
            try { 
                _RecoveryManagerAdmin_hashtable = new java.util.HashMap();
                com.arjuna.CosRecovery.Admin.RecoveryManagerAdminPOA._RecoveryManagerAdmin_init_hash(_RecoveryManagerAdmin_hashtable);
            } catch (java.lang.Exception excep) {
                excep.printStackTrace();
            }
        }

}
