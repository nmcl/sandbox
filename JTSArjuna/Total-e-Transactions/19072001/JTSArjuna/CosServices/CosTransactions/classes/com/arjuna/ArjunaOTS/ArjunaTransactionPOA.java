package com.arjuna.ArjunaOTS;

abstract public class ArjunaTransactionPOA
    extends org.omg.PortableServer.Servant
    implements org.omg.CORBA.portable.InvokeHandler, ArjunaTransactionOperations, com.arjuna.ArjunaOTS.UidCoordinatorOperations, org.omg.CosTransactions.TerminatorOperations
{
    public com.arjuna.ArjunaOTS.ArjunaTransaction _this()
    {
        return com.arjuna.ArjunaOTS.ArjunaTransactionHelper.narrow(_this_object());
    }

    public com.arjuna.ArjunaOTS.ArjunaTransaction _this(
        org.omg.CORBA.ORB orb
)
    {
        return com.arjuna.ArjunaOTS.ArjunaTransactionHelper.narrow(_this_object(orb));
    }

    public org.omg.CORBA.portable.OutputStream _invoke(String opName, 
        org.omg.CORBA.portable.InputStream input, 
        org.omg.CORBA.portable.ResponseHandler handler    )
    {
        java.lang.Object[] _params = {input, handler, this};
        java.lang.reflect.Method _meth = (java.lang.reflect.Method)_ArjunaTransaction_hashtable.get(opName);
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

    public String[]  _all_interfaces(
        org.omg.PortableServer.POA poa,
        byte[] objectId
    )
    {
        String __ids[] =  {"IDL:arjuna.com/ArjunaOTS/ArjunaTransaction:1.0", "IDL:omg.org/CosTransactions/Coordinator:1.0", "IDL:arjuna.com/ArjunaOTS/UidCoordinator:1.0", "IDL:omg.org/CosTransactions/Terminator:1.0"};
        return __ids;
    }

    public static void _ArjunaTransaction_init_hash(
        java.util.HashMap _hash
    )
    {
        java.lang.Class[] params = {org.omg.CORBA.portable.InputStream.class,
            org.omg.CORBA.portable.ResponseHandler.class,
            com.arjuna.ArjunaOTS.ArjunaTransactionOperations.class};
        }

        private static java.util.HashMap _ArjunaTransaction_hashtable;

        static
        {
            try { 
                _ArjunaTransaction_hashtable = new java.util.HashMap();
                org.omg.CosTransactions.CoordinatorPOA._Coordinator_init_hash(_ArjunaTransaction_hashtable);
                com.arjuna.ArjunaOTS.UidCoordinatorPOA._UidCoordinator_init_hash(_ArjunaTransaction_hashtable);
                org.omg.CosTransactions.TerminatorPOA._Terminator_init_hash(_ArjunaTransaction_hashtable);
                com.arjuna.ArjunaOTS.ArjunaTransactionPOA._ArjunaTransaction_init_hash(_ArjunaTransaction_hashtable);
            } catch (java.lang.Exception excep) {
                excep.printStackTrace();
            }
        }

}
