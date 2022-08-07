package org.omg.CosTransactions;

abstract public class TransactionFactoryPOA
    extends org.omg.PortableServer.Servant
    implements org.omg.CORBA.portable.InvokeHandler, TransactionFactoryOperations
{
    public org.omg.CosTransactions.TransactionFactory _this()
    {
        return org.omg.CosTransactions.TransactionFactoryHelper.narrow(_this_object());
    }

    public org.omg.CosTransactions.TransactionFactory _this(
        org.omg.CORBA.ORB orb
)
    {
        return org.omg.CosTransactions.TransactionFactoryHelper.narrow(_this_object(orb));
    }

    public org.omg.CORBA.portable.OutputStream _invoke(String opName, 
        org.omg.CORBA.portable.InputStream input, 
        org.omg.CORBA.portable.ResponseHandler handler    )
    {
        java.lang.Object[] _params = {input, handler, this};
        java.lang.reflect.Method _meth = (java.lang.reflect.Method)_TransactionFactory_hashtable.get(opName);
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

    public static org.omg.CORBA.portable.OutputStream _create_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosTransactions.TransactionFactoryOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        int time_out;
        time_out = _input.read_ulong();
        org.omg.CosTransactions.Control _result = _obj.create(time_out);
        _output = handler.createReply();
        org.omg.CosTransactions.ControlHelper.write(_output, _result);
                return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _recreate_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosTransactions.TransactionFactoryOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        org.omg.CosTransactions.PropagationContext ctx;
        ctx = org.omg.CosTransactions.PropagationContextHelper.read(_input);
        org.omg.CosTransactions.Control _result = _obj.recreate(ctx);
        _output = handler.createReply();
        org.omg.CosTransactions.ControlHelper.write(_output, _result);
                return _output;
    }


    public String[]  _all_interfaces(
        org.omg.PortableServer.POA poa,
        byte[] objectId
    )
    {
        String __ids[] =  {"IDL:omg.org/CosTransactions/TransactionFactory:1.0"};
        return __ids;
    }

    public static void _TransactionFactory_init_hash(
        java.util.HashMap _hash
    )
    {
        java.lang.Class[] params = {org.omg.CORBA.portable.InputStream.class,
            org.omg.CORBA.portable.ResponseHandler.class,
            org.omg.CosTransactions.TransactionFactoryOperations.class};
            try {
                _hash.put("create", org.omg.CosTransactions.TransactionFactoryPOA.class.getDeclaredMethod("_create_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("recreate", org.omg.CosTransactions.TransactionFactoryPOA.class.getDeclaredMethod("_recreate_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
        }

        private static java.util.HashMap _TransactionFactory_hashtable;

        static
        {
            try { 
                _TransactionFactory_hashtable = new java.util.HashMap();
                org.omg.CosTransactions.TransactionFactoryPOA._TransactionFactory_init_hash(_TransactionFactory_hashtable);
            } catch (java.lang.Exception excep) {
                excep.printStackTrace();
            }
        }

}
