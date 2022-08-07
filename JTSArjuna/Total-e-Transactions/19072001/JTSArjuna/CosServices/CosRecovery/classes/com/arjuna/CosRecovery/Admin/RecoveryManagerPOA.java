package com.arjuna.CosRecovery.Admin;

abstract public class RecoveryManagerPOA
    extends org.omg.PortableServer.Servant
    implements org.omg.CORBA.portable.InvokeHandler, RecoveryManagerOperations
{
    public com.arjuna.CosRecovery.Admin.RecoveryManager _this()
    {
        return com.arjuna.CosRecovery.Admin.RecoveryManagerHelper.narrow(_this_object());
    }

    public com.arjuna.CosRecovery.Admin.RecoveryManager _this(
        org.omg.CORBA.ORB orb
)
    {
        return com.arjuna.CosRecovery.Admin.RecoveryManagerHelper.narrow(_this_object(orb));
    }

    public org.omg.CORBA.portable.OutputStream _invoke(String opName, 
        org.omg.CORBA.portable.InputStream input, 
        org.omg.CORBA.portable.ResponseHandler handler    )
    {
        java.lang.Object[] _params = {input, handler, this};
        java.lang.reflect.Method _meth = (java.lang.reflect.Method)_RecoveryManager_hashtable.get(opName);
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

    public static org.omg.CORBA.portable.OutputStream _createIOR_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        com.arjuna.CosRecovery.Admin.RecoveryManagerOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        java.lang.String id;
        id = _input.read_string();
        java.lang.String _result = _obj.createIOR(id);
        _output = handler.createReply();
        _output.write_string(_result);
                return _output;
    }


    public String[]  _all_interfaces(
        org.omg.PortableServer.POA poa,
        byte[] objectId
    )
    {
        String __ids[] =  {"IDL:CosRecovery.arjuna.com/Admin/RecoveryManager:1.0"};
        return __ids;
    }

    public static void _RecoveryManager_init_hash(
        java.util.HashMap _hash
    )
    {
        java.lang.Class[] params = {org.omg.CORBA.portable.InputStream.class,
            org.omg.CORBA.portable.ResponseHandler.class,
            com.arjuna.CosRecovery.Admin.RecoveryManagerOperations.class};
            try {
                _hash.put("createIOR", com.arjuna.CosRecovery.Admin.RecoveryManagerPOA.class.getDeclaredMethod("_createIOR_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
        }

        private static java.util.HashMap _RecoveryManager_hashtable;

        static
        {
            try { 
                _RecoveryManager_hashtable = new java.util.HashMap();
                com.arjuna.CosRecovery.Admin.RecoveryManagerPOA._RecoveryManager_init_hash(_RecoveryManager_hashtable);
            } catch (java.lang.Exception excep) {
                excep.printStackTrace();
            }
        }

}
