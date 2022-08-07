package org.omg.CosTransactions;

abstract public class SubtransactionAwareResourcePOA
    extends org.omg.PortableServer.Servant
    implements org.omg.CORBA.portable.InvokeHandler, SubtransactionAwareResourceOperations, org.omg.CosTransactions.ResourceOperations
{
    public org.omg.CosTransactions.SubtransactionAwareResource _this()
    {
        return org.omg.CosTransactions.SubtransactionAwareResourceHelper.narrow(_this_object());
    }

    public org.omg.CosTransactions.SubtransactionAwareResource _this(
        org.omg.CORBA.ORB orb
)
    {
        return org.omg.CosTransactions.SubtransactionAwareResourceHelper.narrow(_this_object(orb));
    }

    public org.omg.CORBA.portable.OutputStream _invoke(String opName, 
        org.omg.CORBA.portable.InputStream input, 
        org.omg.CORBA.portable.ResponseHandler handler    )
    {
        java.lang.Object[] _params = {input, handler, this};
        java.lang.reflect.Method _meth = (java.lang.reflect.Method)_SubtransactionAwareResource_hashtable.get(opName);
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

    public static org.omg.CORBA.portable.OutputStream _commit_subtransaction_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosTransactions.SubtransactionAwareResourceOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        org.omg.CosTransactions.Coordinator parent;
        parent = org.omg.CosTransactions.CoordinatorHelper.read(_input);
        _obj.commit_subtransaction(parent);
        _output = handler.createReply();
                return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _rollback_subtransaction_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosTransactions.SubtransactionAwareResourceOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        _obj.rollback_subtransaction();
        _output = handler.createReply();
        return _output;
    }


    public String[]  _all_interfaces(
        org.omg.PortableServer.POA poa,
        byte[] objectId
    )
    {
        String __ids[] =  {"IDL:omg.org/CosTransactions/SubtransactionAwareResource:1.0", "IDL:omg.org/CosTransactions/Resource:1.0"};
        return __ids;
    }

    public static void _SubtransactionAwareResource_init_hash(
        java.util.HashMap _hash
    )
    {
        java.lang.Class[] params = {org.omg.CORBA.portable.InputStream.class,
            org.omg.CORBA.portable.ResponseHandler.class,
            org.omg.CosTransactions.SubtransactionAwareResourceOperations.class};
            try {
                _hash.put("commit_subtransaction", org.omg.CosTransactions.SubtransactionAwareResourcePOA.class.getDeclaredMethod("_commit_subtransaction_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("rollback_subtransaction", org.omg.CosTransactions.SubtransactionAwareResourcePOA.class.getDeclaredMethod("_rollback_subtransaction_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
        }

        private static java.util.HashMap _SubtransactionAwareResource_hashtable;

        static
        {
            try { 
                _SubtransactionAwareResource_hashtable = new java.util.HashMap();
                org.omg.CosTransactions.SubtransactionAwareResourcePOA._SubtransactionAwareResource_init_hash(_SubtransactionAwareResource_hashtable);
                org.omg.CosTransactions.ResourcePOA._Resource_init_hash(_SubtransactionAwareResource_hashtable);
            } catch (java.lang.Exception excep) {
                excep.printStackTrace();
            }
        }

}
