package org.omg.CosTransactions;

abstract public class CoordinatorPOA
    extends org.omg.PortableServer.Servant
    implements org.omg.CORBA.portable.InvokeHandler, CoordinatorOperations
{
    public org.omg.CosTransactions.Coordinator _this()
    {
        return org.omg.CosTransactions.CoordinatorHelper.narrow(_this_object());
    }

    public org.omg.CosTransactions.Coordinator _this(
        org.omg.CORBA.ORB orb
)
    {
        return org.omg.CosTransactions.CoordinatorHelper.narrow(_this_object(orb));
    }

    public org.omg.CORBA.portable.OutputStream _invoke(String opName, 
        org.omg.CORBA.portable.InputStream input, 
        org.omg.CORBA.portable.ResponseHandler handler    )
    {
        java.lang.Object[] _params = {input, handler, this};
        java.lang.reflect.Method _meth = (java.lang.reflect.Method)_Coordinator_hashtable.get(opName);
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

    public static org.omg.CORBA.portable.OutputStream _get_status_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosTransactions.CoordinatorOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        org.omg.CosTransactions.Status _result = _obj.get_status();
        _output = handler.createReply();
        org.omg.CosTransactions.StatusHelper.write(_output, _result);
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _get_parent_status_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosTransactions.CoordinatorOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        org.omg.CosTransactions.Status _result = _obj.get_parent_status();
        _output = handler.createReply();
        org.omg.CosTransactions.StatusHelper.write(_output, _result);
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _get_top_level_status_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosTransactions.CoordinatorOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        org.omg.CosTransactions.Status _result = _obj.get_top_level_status();
        _output = handler.createReply();
        org.omg.CosTransactions.StatusHelper.write(_output, _result);
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _is_same_transaction_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosTransactions.CoordinatorOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        org.omg.CosTransactions.Coordinator tc;
        tc = org.omg.CosTransactions.CoordinatorHelper.read(_input);
        boolean _result = _obj.is_same_transaction(tc);
        _output = handler.createReply();
        _output.write_boolean(_result);
                return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _is_related_transaction_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosTransactions.CoordinatorOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        org.omg.CosTransactions.Coordinator tc;
        tc = org.omg.CosTransactions.CoordinatorHelper.read(_input);
        boolean _result = _obj.is_related_transaction(tc);
        _output = handler.createReply();
        _output.write_boolean(_result);
                return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _is_ancestor_transaction_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosTransactions.CoordinatorOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        org.omg.CosTransactions.Coordinator tc;
        tc = org.omg.CosTransactions.CoordinatorHelper.read(_input);
        boolean _result = _obj.is_ancestor_transaction(tc);
        _output = handler.createReply();
        _output.write_boolean(_result);
                return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _is_descendant_transaction_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosTransactions.CoordinatorOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        org.omg.CosTransactions.Coordinator tc;
        tc = org.omg.CosTransactions.CoordinatorHelper.read(_input);
        boolean _result = _obj.is_descendant_transaction(tc);
        _output = handler.createReply();
        _output.write_boolean(_result);
                return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _is_top_level_transaction_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosTransactions.CoordinatorOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        boolean _result = _obj.is_top_level_transaction();
        _output = handler.createReply();
        _output.write_boolean(_result);
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _hash_transaction_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosTransactions.CoordinatorOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        int _result = _obj.hash_transaction();
        _output = handler.createReply();
        _output.write_ulong(_result);
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _hash_top_level_tran_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosTransactions.CoordinatorOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        int _result = _obj.hash_top_level_tran();
        _output = handler.createReply();
        _output.write_ulong(_result);
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _register_resource_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosTransactions.CoordinatorOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            org.omg.CosTransactions.Resource r;
            r = org.omg.CosTransactions.ResourceHelper.read(_input);
            org.omg.CosTransactions.RecoveryCoordinator _result = _obj.register_resource(r);
            _output = handler.createReply();
            org.omg.CosTransactions.RecoveryCoordinatorHelper.write(_output, _result);
                        }
        catch (org.omg.CosTransactions.Inactive _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosTransactions.InactiveHelper.write(_output, _exception0);
            return _output;
        }
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _register_synchronization_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosTransactions.CoordinatorOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            org.omg.CosTransactions.Synchronization sync;
            sync = org.omg.CosTransactions.SynchronizationHelper.read(_input);
            _obj.register_synchronization(sync);
            _output = handler.createReply();
                        }
        catch (org.omg.CosTransactions.Inactive _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosTransactions.InactiveHelper.write(_output, _exception0);
            return _output;
        }
        catch (org.omg.CosTransactions.SynchronizationUnavailable _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosTransactions.SynchronizationUnavailableHelper.write(_output, _exception0);
            return _output;
        }
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _register_subtran_aware_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosTransactions.CoordinatorOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            org.omg.CosTransactions.SubtransactionAwareResource r;
            r = org.omg.CosTransactions.SubtransactionAwareResourceHelper.read(_input);
            _obj.register_subtran_aware(r);
            _output = handler.createReply();
                        }
        catch (org.omg.CosTransactions.Inactive _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosTransactions.InactiveHelper.write(_output, _exception0);
            return _output;
        }
        catch (org.omg.CosTransactions.NotSubtransaction _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosTransactions.NotSubtransactionHelper.write(_output, _exception0);
            return _output;
        }
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _rollback_only_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosTransactions.CoordinatorOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            _obj.rollback_only();
            _output = handler.createReply();
            }
        catch (org.omg.CosTransactions.Inactive _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosTransactions.InactiveHelper.write(_output, _exception0);
            return _output;
        }
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _get_transaction_name_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosTransactions.CoordinatorOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        java.lang.String _result = _obj.get_transaction_name();
        _output = handler.createReply();
        _output.write_string(_result);
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _create_subtransaction_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosTransactions.CoordinatorOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            org.omg.CosTransactions.Control _result = _obj.create_subtransaction();
            _output = handler.createReply();
            org.omg.CosTransactions.ControlHelper.write(_output, _result);
            }
        catch (org.omg.CosTransactions.SubtransactionsUnavailable _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosTransactions.SubtransactionsUnavailableHelper.write(_output, _exception0);
            return _output;
        }
        catch (org.omg.CosTransactions.Inactive _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosTransactions.InactiveHelper.write(_output, _exception0);
            return _output;
        }
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _get_txcontext_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosTransactions.CoordinatorOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            org.omg.CosTransactions.PropagationContext _result = _obj.get_txcontext();
            _output = handler.createReply();
            org.omg.CosTransactions.PropagationContextHelper.write(_output, _result);
            }
        catch (org.omg.CosTransactions.Unavailable _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosTransactions.UnavailableHelper.write(_output, _exception0);
            return _output;
        }
        return _output;
    }


    public String[]  _all_interfaces(
        org.omg.PortableServer.POA poa,
        byte[] objectId
    )
    {
        String __ids[] =  {"IDL:omg.org/CosTransactions/Coordinator:1.0"};
        return __ids;
    }

    public static void _Coordinator_init_hash(
        java.util.HashMap _hash
    )
    {
        java.lang.Class[] params = {org.omg.CORBA.portable.InputStream.class,
            org.omg.CORBA.portable.ResponseHandler.class,
            org.omg.CosTransactions.CoordinatorOperations.class};
            try {
                _hash.put("get_status", org.omg.CosTransactions.CoordinatorPOA.class.getDeclaredMethod("_get_status_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("get_parent_status", org.omg.CosTransactions.CoordinatorPOA.class.getDeclaredMethod("_get_parent_status_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("get_top_level_status", org.omg.CosTransactions.CoordinatorPOA.class.getDeclaredMethod("_get_top_level_status_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("is_same_transaction", org.omg.CosTransactions.CoordinatorPOA.class.getDeclaredMethod("_is_same_transaction_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("is_related_transaction", org.omg.CosTransactions.CoordinatorPOA.class.getDeclaredMethod("_is_related_transaction_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("is_ancestor_transaction", org.omg.CosTransactions.CoordinatorPOA.class.getDeclaredMethod("_is_ancestor_transaction_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("is_descendant_transaction", org.omg.CosTransactions.CoordinatorPOA.class.getDeclaredMethod("_is_descendant_transaction_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("is_top_level_transaction", org.omg.CosTransactions.CoordinatorPOA.class.getDeclaredMethod("_is_top_level_transaction_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("hash_transaction", org.omg.CosTransactions.CoordinatorPOA.class.getDeclaredMethod("_hash_transaction_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("hash_top_level_tran", org.omg.CosTransactions.CoordinatorPOA.class.getDeclaredMethod("_hash_top_level_tran_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("register_resource", org.omg.CosTransactions.CoordinatorPOA.class.getDeclaredMethod("_register_resource_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("register_synchronization", org.omg.CosTransactions.CoordinatorPOA.class.getDeclaredMethod("_register_synchronization_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("register_subtran_aware", org.omg.CosTransactions.CoordinatorPOA.class.getDeclaredMethod("_register_subtran_aware_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("rollback_only", org.omg.CosTransactions.CoordinatorPOA.class.getDeclaredMethod("_rollback_only_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("get_transaction_name", org.omg.CosTransactions.CoordinatorPOA.class.getDeclaredMethod("_get_transaction_name_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("create_subtransaction", org.omg.CosTransactions.CoordinatorPOA.class.getDeclaredMethod("_create_subtransaction_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("get_txcontext", org.omg.CosTransactions.CoordinatorPOA.class.getDeclaredMethod("_get_txcontext_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
        }

        private static java.util.HashMap _Coordinator_hashtable;

        static
        {
            try { 
                _Coordinator_hashtable = new java.util.HashMap();
                org.omg.CosTransactions.CoordinatorPOA._Coordinator_init_hash(_Coordinator_hashtable);
            } catch (java.lang.Exception excep) {
                excep.printStackTrace();
            }
        }

}
