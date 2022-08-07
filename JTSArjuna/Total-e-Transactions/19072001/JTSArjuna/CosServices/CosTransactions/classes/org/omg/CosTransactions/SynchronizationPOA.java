package org.omg.CosTransactions;

abstract public class SynchronizationPOA
    extends org.omg.PortableServer.Servant
    implements org.omg.CORBA.portable.InvokeHandler, SynchronizationOperations, org.omg.CosTransactions.TransactionalObjectOperations
{
    public org.omg.CosTransactions.Synchronization _this()
    {
        return org.omg.CosTransactions.SynchronizationHelper.narrow(_this_object());
    }

    public org.omg.CosTransactions.Synchronization _this(
        org.omg.CORBA.ORB orb
)
    {
        return org.omg.CosTransactions.SynchronizationHelper.narrow(_this_object(orb));
    }

    public org.omg.CORBA.portable.OutputStream _invoke(String opName, 
        org.omg.CORBA.portable.InputStream input, 
        org.omg.CORBA.portable.ResponseHandler handler    )
    {
        java.lang.Object[] _params = {input, handler, this};
        java.lang.reflect.Method _meth = (java.lang.reflect.Method)_Synchronization_hashtable.get(opName);
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

    public static org.omg.CORBA.portable.OutputStream _before_completion_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosTransactions.SynchronizationOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        _obj.before_completion();
        _output = handler.createReply();
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _after_completion_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosTransactions.SynchronizationOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        org.omg.CosTransactions.Status s;
        s = org.omg.CosTransactions.StatusHelper.read(_input);
        _obj.after_completion(s);
        _output = handler.createReply();
                return _output;
    }


    public String[]  _all_interfaces(
        org.omg.PortableServer.POA poa,
        byte[] objectId
    )
    {
        String __ids[] =  {"IDL:omg.org/CosTransactions/Synchronization:1.0", "IDL:omg.org/CosTransactions/TransactionalObject:1.0"};
        return __ids;
    }

    public static void _Synchronization_init_hash(
        java.util.HashMap _hash
    )
    {
        java.lang.Class[] params = {org.omg.CORBA.portable.InputStream.class,
            org.omg.CORBA.portable.ResponseHandler.class,
            org.omg.CosTransactions.SynchronizationOperations.class};
            try {
                _hash.put("before_completion", org.omg.CosTransactions.SynchronizationPOA.class.getDeclaredMethod("_before_completion_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("after_completion", org.omg.CosTransactions.SynchronizationPOA.class.getDeclaredMethod("_after_completion_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
        }

        private static java.util.HashMap _Synchronization_hashtable;

        static
        {
            try { 
                _Synchronization_hashtable = new java.util.HashMap();
                org.omg.CosTransactions.TransactionalObjectPOA._TransactionalObject_init_hash(_Synchronization_hashtable);
                org.omg.CosTransactions.SynchronizationPOA._Synchronization_init_hash(_Synchronization_hashtable);
            } catch (java.lang.Exception excep) {
                excep.printStackTrace();
            }
        }

}
