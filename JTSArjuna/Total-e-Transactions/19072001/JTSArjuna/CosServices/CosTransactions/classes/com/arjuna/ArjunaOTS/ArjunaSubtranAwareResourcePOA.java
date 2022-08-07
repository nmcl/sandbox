package com.arjuna.ArjunaOTS;

abstract public class ArjunaSubtranAwareResourcePOA
    extends org.omg.PortableServer.Servant
    implements org.omg.CORBA.portable.InvokeHandler, ArjunaSubtranAwareResourceOperations, org.omg.CosTransactions.SubtransactionAwareResourceOperations
{
    public com.arjuna.ArjunaOTS.ArjunaSubtranAwareResource _this()
    {
        return com.arjuna.ArjunaOTS.ArjunaSubtranAwareResourceHelper.narrow(_this_object());
    }

    public com.arjuna.ArjunaOTS.ArjunaSubtranAwareResource _this(
        org.omg.CORBA.ORB orb
)
    {
        return com.arjuna.ArjunaOTS.ArjunaSubtranAwareResourceHelper.narrow(_this_object(orb));
    }

    public org.omg.CORBA.portable.OutputStream _invoke(String opName, 
        org.omg.CORBA.portable.InputStream input, 
        org.omg.CORBA.portable.ResponseHandler handler    )
    {
        java.lang.Object[] _params = {input, handler, this};
        java.lang.reflect.Method _meth = (java.lang.reflect.Method)_ArjunaSubtranAwareResource_hashtable.get(opName);
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

    public static org.omg.CORBA.portable.OutputStream _prepare_subtransaction_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        com.arjuna.ArjunaOTS.ArjunaSubtranAwareResourceOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        org.omg.CosTransactions.Vote _result = _obj.prepare_subtransaction();
        _output = handler.createReply();
        org.omg.CosTransactions.VoteHelper.write(_output, _result);
        return _output;
    }


    public String[]  _all_interfaces(
        org.omg.PortableServer.POA poa,
        byte[] objectId
    )
    {
        String __ids[] =  {"IDL:arjuna.com/ArjunaOTS/ArjunaSubtranAwareResource:1.0", "IDL:omg.org/CosTransactions/Resource:1.0", "IDL:omg.org/CosTransactions/SubtransactionAwareResource:1.0"};
        return __ids;
    }

    public static void _ArjunaSubtranAwareResource_init_hash(
        java.util.HashMap _hash
    )
    {
        java.lang.Class[] params = {org.omg.CORBA.portable.InputStream.class,
            org.omg.CORBA.portable.ResponseHandler.class,
            com.arjuna.ArjunaOTS.ArjunaSubtranAwareResourceOperations.class};
            try {
                _hash.put("prepare_subtransaction", com.arjuna.ArjunaOTS.ArjunaSubtranAwareResourcePOA.class.getDeclaredMethod("_prepare_subtransaction_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
        }

        private static java.util.HashMap _ArjunaSubtranAwareResource_hashtable;

        static
        {
            try { 
                _ArjunaSubtranAwareResource_hashtable = new java.util.HashMap();
                org.omg.CosTransactions.ResourcePOA._Resource_init_hash(_ArjunaSubtranAwareResource_hashtable);
                org.omg.CosTransactions.SubtransactionAwareResourcePOA._SubtransactionAwareResource_init_hash(_ArjunaSubtranAwareResource_hashtable);
                com.arjuna.ArjunaOTS.ArjunaSubtranAwareResourcePOA._ArjunaSubtranAwareResource_init_hash(_ArjunaSubtranAwareResource_hashtable);
            } catch (java.lang.Exception excep) {
                excep.printStackTrace();
            }
        }

}
