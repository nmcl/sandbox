package org.omg.CosTransactions;

abstract public class TransactionalObjectPOA
    extends org.omg.PortableServer.Servant
    implements org.omg.CORBA.portable.InvokeHandler, TransactionalObjectOperations
{
    public org.omg.CosTransactions.TransactionalObject _this()
    {
        return org.omg.CosTransactions.TransactionalObjectHelper.narrow(_this_object());
    }

    public org.omg.CosTransactions.TransactionalObject _this(
        org.omg.CORBA.ORB orb
)
    {
        return org.omg.CosTransactions.TransactionalObjectHelper.narrow(_this_object(orb));
    }

    public org.omg.CORBA.portable.OutputStream _invoke(String opName, 
        org.omg.CORBA.portable.InputStream input, 
        org.omg.CORBA.portable.ResponseHandler handler    )
    {
        java.lang.Object[] _params = {input, handler, this};
        java.lang.reflect.Method _meth = (java.lang.reflect.Method)_TransactionalObject_hashtable.get(opName);
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
        String __ids[] =  {"IDL:omg.org/CosTransactions/TransactionalObject:1.0"};
        return __ids;
    }

    public static void _TransactionalObject_init_hash(
        java.util.HashMap _hash
    )
    {
        java.lang.Class[] params = {org.omg.CORBA.portable.InputStream.class,
            org.omg.CORBA.portable.ResponseHandler.class,
            org.omg.CosTransactions.TransactionalObjectOperations.class};
        }

        private static java.util.HashMap _TransactionalObject_hashtable;

        static
        {
            try { 
                _TransactionalObject_hashtable = new java.util.HashMap();
                org.omg.CosTransactions.TransactionalObjectPOA._TransactionalObject_init_hash(_TransactionalObject_hashtable);
            } catch (java.lang.Exception excep) {
                excep.printStackTrace();
            }
        }

}
