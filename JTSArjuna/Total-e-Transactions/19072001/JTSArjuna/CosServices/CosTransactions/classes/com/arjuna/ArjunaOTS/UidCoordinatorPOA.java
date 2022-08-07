package com.arjuna.ArjunaOTS;

abstract public class UidCoordinatorPOA
    extends org.omg.PortableServer.Servant
    implements org.omg.CORBA.portable.InvokeHandler, UidCoordinatorOperations, org.omg.CosTransactions.CoordinatorOperations
{
    public com.arjuna.ArjunaOTS.UidCoordinator _this()
    {
        return com.arjuna.ArjunaOTS.UidCoordinatorHelper.narrow(_this_object());
    }

    public com.arjuna.ArjunaOTS.UidCoordinator _this(
        org.omg.CORBA.ORB orb
)
    {
        return com.arjuna.ArjunaOTS.UidCoordinatorHelper.narrow(_this_object(orb));
    }

    public org.omg.CORBA.portable.OutputStream _invoke(String opName, 
        org.omg.CORBA.portable.InputStream input, 
        org.omg.CORBA.portable.ResponseHandler handler    )
    {
        java.lang.Object[] _params = {input, handler, this};
        java.lang.reflect.Method _meth = (java.lang.reflect.Method)_UidCoordinator_hashtable.get(opName);
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

    public static org.omg.CORBA.portable.OutputStream __get_uid_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        com.arjuna.ArjunaOTS.UidCoordinatorOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        java.lang.String _result = _obj.uid();
        _output = handler.createReply();
        _output.write_string(_result);
        return _output;
    }
    public static org.omg.CORBA.portable.OutputStream __get_topLevelUid_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        com.arjuna.ArjunaOTS.UidCoordinatorOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        java.lang.String _result = _obj.topLevelUid();
        _output = handler.createReply();
        _output.write_string(_result);
        return _output;
    }
    public String[]  _all_interfaces(
        org.omg.PortableServer.POA poa,
        byte[] objectId
    )
    {
        String __ids[] =  {"IDL:arjuna.com/ArjunaOTS/UidCoordinator:1.0", "IDL:omg.org/CosTransactions/Coordinator:1.0"};
        return __ids;
    }

    public static void _UidCoordinator_init_hash(
        java.util.HashMap _hash
    )
    {
        java.lang.Class[] params = {org.omg.CORBA.portable.InputStream.class,
            org.omg.CORBA.portable.ResponseHandler.class,
            com.arjuna.ArjunaOTS.UidCoordinatorOperations.class};
            try {
                _hash.put("_get_uid", com.arjuna.ArjunaOTS.UidCoordinatorPOA.class.getDeclaredMethod("__get_uid_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("_get_topLevelUid", com.arjuna.ArjunaOTS.UidCoordinatorPOA.class.getDeclaredMethod("__get_topLevelUid_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
        }

        private static java.util.HashMap _UidCoordinator_hashtable;

        static
        {
            try { 
                _UidCoordinator_hashtable = new java.util.HashMap();
                org.omg.CosTransactions.CoordinatorPOA._Coordinator_init_hash(_UidCoordinator_hashtable);
                com.arjuna.ArjunaOTS.UidCoordinatorPOA._UidCoordinator_init_hash(_UidCoordinator_hashtable);
            } catch (java.lang.Exception excep) {
                excep.printStackTrace();
            }
        }

}
