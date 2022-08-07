package org.omg.CosTransactions;

abstract public class ResourcePOA
    extends org.omg.PortableServer.Servant
    implements org.omg.CORBA.portable.InvokeHandler, ResourceOperations
{
    public org.omg.CosTransactions.Resource _this()
    {
        return org.omg.CosTransactions.ResourceHelper.narrow(_this_object());
    }

    public org.omg.CosTransactions.Resource _this(
        org.omg.CORBA.ORB orb
)
    {
        return org.omg.CosTransactions.ResourceHelper.narrow(_this_object(orb));
    }

    public org.omg.CORBA.portable.OutputStream _invoke(String opName, 
        org.omg.CORBA.portable.InputStream input, 
        org.omg.CORBA.portable.ResponseHandler handler    )
    {
        java.lang.Object[] _params = {input, handler, this};
        java.lang.reflect.Method _meth = (java.lang.reflect.Method)_Resource_hashtable.get(opName);
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

    public static org.omg.CORBA.portable.OutputStream _prepare_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosTransactions.ResourceOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            org.omg.CosTransactions.Vote _result = _obj.prepare();
            _output = handler.createReply();
            org.omg.CosTransactions.VoteHelper.write(_output, _result);
            }
        catch (org.omg.CosTransactions.HeuristicMixed _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosTransactions.HeuristicMixedHelper.write(_output, _exception0);
            return _output;
        }
        catch (org.omg.CosTransactions.HeuristicHazard _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosTransactions.HeuristicHazardHelper.write(_output, _exception0);
            return _output;
        }
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _rollback_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosTransactions.ResourceOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            _obj.rollback();
            _output = handler.createReply();
            }
        catch (org.omg.CosTransactions.HeuristicCommit _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosTransactions.HeuristicCommitHelper.write(_output, _exception0);
            return _output;
        }
        catch (org.omg.CosTransactions.HeuristicMixed _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosTransactions.HeuristicMixedHelper.write(_output, _exception0);
            return _output;
        }
        catch (org.omg.CosTransactions.HeuristicHazard _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosTransactions.HeuristicHazardHelper.write(_output, _exception0);
            return _output;
        }
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _commit_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosTransactions.ResourceOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            _obj.commit();
            _output = handler.createReply();
            }
        catch (org.omg.CosTransactions.NotPrepared _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosTransactions.NotPreparedHelper.write(_output, _exception0);
            return _output;
        }
        catch (org.omg.CosTransactions.HeuristicRollback _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosTransactions.HeuristicRollbackHelper.write(_output, _exception0);
            return _output;
        }
        catch (org.omg.CosTransactions.HeuristicMixed _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosTransactions.HeuristicMixedHelper.write(_output, _exception0);
            return _output;
        }
        catch (org.omg.CosTransactions.HeuristicHazard _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosTransactions.HeuristicHazardHelper.write(_output, _exception0);
            return _output;
        }
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _commit_one_phase_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosTransactions.ResourceOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            _obj.commit_one_phase();
            _output = handler.createReply();
            }
        catch (org.omg.CosTransactions.HeuristicHazard _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosTransactions.HeuristicHazardHelper.write(_output, _exception0);
            return _output;
        }
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _forget_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosTransactions.ResourceOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        _obj.forget();
        _output = handler.createReply();
        return _output;
    }


    public String[]  _all_interfaces(
        org.omg.PortableServer.POA poa,
        byte[] objectId
    )
    {
        String __ids[] =  {"IDL:omg.org/CosTransactions/Resource:1.0"};
        return __ids;
    }

    public static void _Resource_init_hash(
        java.util.HashMap _hash
    )
    {
        java.lang.Class[] params = {org.omg.CORBA.portable.InputStream.class,
            org.omg.CORBA.portable.ResponseHandler.class,
            org.omg.CosTransactions.ResourceOperations.class};
            try {
                _hash.put("prepare", org.omg.CosTransactions.ResourcePOA.class.getDeclaredMethod("_prepare_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("rollback", org.omg.CosTransactions.ResourcePOA.class.getDeclaredMethod("_rollback_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("commit", org.omg.CosTransactions.ResourcePOA.class.getDeclaredMethod("_commit_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("commit_one_phase", org.omg.CosTransactions.ResourcePOA.class.getDeclaredMethod("_commit_one_phase_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("forget", org.omg.CosTransactions.ResourcePOA.class.getDeclaredMethod("_forget_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
        }

        private static java.util.HashMap _Resource_hashtable;

        static
        {
            try { 
                _Resource_hashtable = new java.util.HashMap();
                org.omg.CosTransactions.ResourcePOA._Resource_init_hash(_Resource_hashtable);
            } catch (java.lang.Exception excep) {
                excep.printStackTrace();
            }
        }

}
