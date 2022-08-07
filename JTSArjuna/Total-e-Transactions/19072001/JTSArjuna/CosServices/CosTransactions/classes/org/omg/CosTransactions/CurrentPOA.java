package org.omg.CosTransactions;

abstract public class CurrentPOA
    extends org.omg.PortableServer.Servant
    implements org.omg.CORBA.portable.InvokeHandler, CurrentOperations
{
    public org.omg.CosTransactions.Current _this()
    {
        return org.omg.CosTransactions.CurrentHelper.narrow(_this_object());
    }

    public org.omg.CosTransactions.Current _this(
        org.omg.CORBA.ORB orb
)
    {
        return org.omg.CosTransactions.CurrentHelper.narrow(_this_object(orb));
    }

    public org.omg.CORBA.portable.OutputStream _invoke(String opName, 
        org.omg.CORBA.portable.InputStream input, 
        org.omg.CORBA.portable.ResponseHandler handler    )
    {
        java.lang.Object[] _params = {input, handler, this};
        java.lang.reflect.Method _meth = (java.lang.reflect.Method)_Current_hashtable.get(opName);
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

    public static org.omg.CORBA.portable.OutputStream _begin_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosTransactions.CurrentOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            _obj.begin();
            _output = handler.createReply();
            }
        catch (org.omg.CosTransactions.SubtransactionsUnavailable _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosTransactions.SubtransactionsUnavailableHelper.write(_output, _exception0);
            return _output;
        }
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _commit_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosTransactions.CurrentOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            boolean report_heuristics;
            report_heuristics = _input.read_boolean();
            _obj.commit(report_heuristics);
            _output = handler.createReply();
                        }
        catch (org.omg.CosTransactions.NoTransaction _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosTransactions.NoTransactionHelper.write(_output, _exception0);
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


    public static org.omg.CORBA.portable.OutputStream _rollback_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosTransactions.CurrentOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            _obj.rollback();
            _output = handler.createReply();
            }
        catch (org.omg.CosTransactions.NoTransaction _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosTransactions.NoTransactionHelper.write(_output, _exception0);
            return _output;
        }
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _rollback_only_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosTransactions.CurrentOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            _obj.rollback_only();
            _output = handler.createReply();
            }
        catch (org.omg.CosTransactions.NoTransaction _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosTransactions.NoTransactionHelper.write(_output, _exception0);
            return _output;
        }
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _get_status_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosTransactions.CurrentOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        org.omg.CosTransactions.Status _result = _obj.get_status();
        _output = handler.createReply();
        org.omg.CosTransactions.StatusHelper.write(_output, _result);
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _get_transaction_name_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosTransactions.CurrentOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        java.lang.String _result = _obj.get_transaction_name();
        _output = handler.createReply();
        _output.write_string(_result);
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _set_timeout_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosTransactions.CurrentOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        int seconds;
        seconds = _input.read_ulong();
        _obj.set_timeout(seconds);
        _output = handler.createReply();
                return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _get_control_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosTransactions.CurrentOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        org.omg.CosTransactions.Control _result = _obj.get_control();
        _output = handler.createReply();
        org.omg.CosTransactions.ControlHelper.write(_output, _result);
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _suspend_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosTransactions.CurrentOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        org.omg.CosTransactions.Control _result = _obj.suspend();
        _output = handler.createReply();
        org.omg.CosTransactions.ControlHelper.write(_output, _result);
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _resume_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosTransactions.CurrentOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            org.omg.CosTransactions.Control which;
            which = org.omg.CosTransactions.ControlHelper.read(_input);
            _obj.resume(which);
            _output = handler.createReply();
                        }
        catch (org.omg.CosTransactions.InvalidControl _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosTransactions.InvalidControlHelper.write(_output, _exception0);
            return _output;
        }
        return _output;
    }


    public String[]  _all_interfaces(
        org.omg.PortableServer.POA poa,
        byte[] objectId
    )
    {
        String __ids[] =  {"IDL:omg.org/CosTransactions/Current:1.0"};
        return __ids;
    }

    public static void _Current_init_hash(
        java.util.HashMap _hash
    )
    {
        java.lang.Class[] params = {org.omg.CORBA.portable.InputStream.class,
            org.omg.CORBA.portable.ResponseHandler.class,
            org.omg.CosTransactions.CurrentOperations.class};
            try {
                _hash.put("begin", org.omg.CosTransactions.CurrentPOA.class.getDeclaredMethod("_begin_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("commit", org.omg.CosTransactions.CurrentPOA.class.getDeclaredMethod("_commit_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("rollback", org.omg.CosTransactions.CurrentPOA.class.getDeclaredMethod("_rollback_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("rollback_only", org.omg.CosTransactions.CurrentPOA.class.getDeclaredMethod("_rollback_only_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("get_status", org.omg.CosTransactions.CurrentPOA.class.getDeclaredMethod("_get_status_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("get_transaction_name", org.omg.CosTransactions.CurrentPOA.class.getDeclaredMethod("_get_transaction_name_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("set_timeout", org.omg.CosTransactions.CurrentPOA.class.getDeclaredMethod("_set_timeout_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("get_control", org.omg.CosTransactions.CurrentPOA.class.getDeclaredMethod("_get_control_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("suspend", org.omg.CosTransactions.CurrentPOA.class.getDeclaredMethod("_suspend_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("resume", org.omg.CosTransactions.CurrentPOA.class.getDeclaredMethod("_resume_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
        }

        private static java.util.HashMap _Current_hashtable;

        static
        {
            try { 
                _Current_hashtable = new java.util.HashMap();
                org.omg.CosTransactions.CurrentPOA._Current_init_hash(_Current_hashtable);
            } catch (java.lang.Exception excep) {
                excep.printStackTrace();
            }
        }

}
