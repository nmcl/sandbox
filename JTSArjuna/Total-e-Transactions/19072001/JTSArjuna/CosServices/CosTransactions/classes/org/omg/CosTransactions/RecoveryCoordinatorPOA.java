package org.omg.CosTransactions;

abstract public class RecoveryCoordinatorPOA
    extends org.omg.PortableServer.Servant
    implements org.omg.CORBA.portable.InvokeHandler, RecoveryCoordinatorOperations
{
    public org.omg.CosTransactions.RecoveryCoordinator _this()
    {
        return org.omg.CosTransactions.RecoveryCoordinatorHelper.narrow(_this_object());
    }

    public org.omg.CosTransactions.RecoveryCoordinator _this(
        org.omg.CORBA.ORB orb
)
    {
        return org.omg.CosTransactions.RecoveryCoordinatorHelper.narrow(_this_object(orb));
    }

    public org.omg.CORBA.portable.OutputStream _invoke(String opName, 
        org.omg.CORBA.portable.InputStream input, 
        org.omg.CORBA.portable.ResponseHandler handler    )
    {
        java.lang.Object[] _params = {input, handler, this};
        java.lang.reflect.Method _meth = (java.lang.reflect.Method)_RecoveryCoordinator_hashtable.get(opName);
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

    public static org.omg.CORBA.portable.OutputStream _replay_completion_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosTransactions.RecoveryCoordinatorOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            org.omg.CosTransactions.Resource r;
            r = org.omg.CosTransactions.ResourceHelper.read(_input);
            org.omg.CosTransactions.Status _result = _obj.replay_completion(r);
            _output = handler.createReply();
            org.omg.CosTransactions.StatusHelper.write(_output, _result);
                        }
        catch (org.omg.CosTransactions.NotPrepared _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosTransactions.NotPreparedHelper.write(_output, _exception0);
            return _output;
        }
        return _output;
    }


    public String[]  _all_interfaces(
        org.omg.PortableServer.POA poa,
        byte[] objectId
    )
    {
        String __ids[] =  {"IDL:omg.org/CosTransactions/RecoveryCoordinator:1.0"};
        return __ids;
    }

    public static void _RecoveryCoordinator_init_hash(
        java.util.HashMap _hash
    )
    {
        java.lang.Class[] params = {org.omg.CORBA.portable.InputStream.class,
            org.omg.CORBA.portable.ResponseHandler.class,
            org.omg.CosTransactions.RecoveryCoordinatorOperations.class};
            try {
                _hash.put("replay_completion", org.omg.CosTransactions.RecoveryCoordinatorPOA.class.getDeclaredMethod("_replay_completion_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
        }

        private static java.util.HashMap _RecoveryCoordinator_hashtable;

        static
        {
            try { 
                _RecoveryCoordinator_hashtable = new java.util.HashMap();
                org.omg.CosTransactions.RecoveryCoordinatorPOA._RecoveryCoordinator_init_hash(_RecoveryCoordinator_hashtable);
            } catch (java.lang.Exception excep) {
                excep.printStackTrace();
            }
        }

}
