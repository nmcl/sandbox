package org.omg.CosActivity;

abstract public class CurrentPOA
    extends org.omg.PortableServer.Servant
    implements org.omg.CORBA.portable.InvokeHandler, CurrentOperations
{
    public org.omg.CosActivity.Current _this()
    {
        return org.omg.CosActivity.CurrentHelper.narrow(_this_object());
    }

    public org.omg.CosActivity.Current _this(
        org.omg.CORBA.ORB orb
)
    {
        return org.omg.CosActivity.CurrentHelper.narrow(_this_object(orb));
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
        org.omg.CosActivity.CurrentOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            int timeout;
            timeout = _input.read_long();
            _obj.begin(timeout);
            _output = handler.createReply();
                        }
        catch (org.omg.CosActivity.InvalidState _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosActivity.InvalidStateHelper.write(_output, _exception0);
            return _output;
        }
        catch (org.omg.CosActivity.TimeoutOutOfRange _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosActivity.TimeoutOutOfRangeHelper.write(_output, _exception0);
            return _output;
        }
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _complete_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.CurrentOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            org.omg.CosActivity.Outcome _result = _obj.complete();
            _output = handler.createReply();
            org.omg.CosActivity.OutcomeHelper.write(_output, _result);
            }
        catch (org.omg.CosActivity.NoActivity _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosActivity.NoActivityHelper.write(_output, _exception0);
            return _output;
        }
        catch (org.omg.CosActivity.ActivityPending _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosActivity.ActivityPendingHelper.write(_output, _exception0);
            return _output;
        }
        catch (org.omg.CosActivity.ChildContextPending _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosActivity.ChildContextPendingHelper.write(_output, _exception0);
            return _output;
        }
        catch (org.omg.CosActivity.ActivityNotProcessed _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosActivity.ActivityNotProcessedHelper.write(_output, _exception0);
            return _output;
        }
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _complete_with_status_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.CurrentOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            org.omg.CosActivity.CompletionStatus cs;
            cs = org.omg.CosActivity.CompletionStatusHelper.read(_input);
            org.omg.CosActivity.Outcome _result = _obj.complete_with_status(cs);
            _output = handler.createReply();
            org.omg.CosActivity.OutcomeHelper.write(_output, _result);
                        }
        catch (org.omg.CosActivity.NoActivity _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosActivity.NoActivityHelper.write(_output, _exception0);
            return _output;
        }
        catch (org.omg.CosActivity.ActivityPending _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosActivity.ActivityPendingHelper.write(_output, _exception0);
            return _output;
        }
        catch (org.omg.CosActivity.ChildContextPending _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosActivity.ChildContextPendingHelper.write(_output, _exception0);
            return _output;
        }
        catch (org.omg.CosActivity.InvalidState _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosActivity.InvalidStateHelper.write(_output, _exception0);
            return _output;
        }
        catch (org.omg.CosActivity.ActivityNotProcessed _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosActivity.ActivityNotProcessedHelper.write(_output, _exception0);
            return _output;
        }
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _set_completion_status_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.CurrentOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            org.omg.CosActivity.CompletionStatus cs;
            cs = org.omg.CosActivity.CompletionStatusHelper.read(_input);
            _obj.set_completion_status(cs);
            _output = handler.createReply();
                        }
        catch (org.omg.CosActivity.NoActivity _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosActivity.NoActivityHelper.write(_output, _exception0);
            return _output;
        }
        catch (org.omg.CosActivity.InvalidState _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosActivity.InvalidStateHelper.write(_output, _exception0);
            return _output;
        }
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _get_completion_status_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.CurrentOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            org.omg.CosActivity.CompletionStatus _result = _obj.get_completion_status();
            _output = handler.createReply();
            org.omg.CosActivity.CompletionStatusHelper.write(_output, _result);
            }
        catch (org.omg.CosActivity.NoActivity _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosActivity.NoActivityHelper.write(_output, _exception0);
            return _output;
        }
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _set_completion_signal_set_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.CurrentOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            java.lang.String signal_set_name;
            signal_set_name = _input.read_string();
            _obj.set_completion_signal_set(signal_set_name);
            _output = handler.createReply();
                        }
        catch (org.omg.CosActivity.NoActivity _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosActivity.NoActivityHelper.write(_output, _exception0);
            return _output;
        }
        catch (org.omg.CosActivity.SignalSetUnknown _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosActivity.SignalSetUnknownHelper.write(_output, _exception0);
            return _output;
        }
        catch (org.omg.CosActivity.InvalidState _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosActivity.InvalidStateHelper.write(_output, _exception0);
            return _output;
        }
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _get_completion_signal_set_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.CurrentOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            java.lang.String _result = _obj.get_completion_signal_set();
            _output = handler.createReply();
            _output.write_string(_result);
            }
        catch (org.omg.CosActivity.NoActivity _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosActivity.NoActivityHelper.write(_output, _exception0);
            return _output;
        }
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _suspend_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.CurrentOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            org.omg.CosActivity.ActivityToken _result = _obj.suspend();
            _output = handler.createReply();
            org.omg.CosActivity.ActivityTokenHelper.write(_output, _result);
            }
        catch (org.omg.CosActivity.InvalidParentContext _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosActivity.InvalidParentContextHelper.write(_output, _exception0);
            return _output;
        }
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _resume_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.CurrentOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            org.omg.CosActivity.ActivityToken at;
            at = org.omg.CosActivity.ActivityTokenHelper.read(_input);
            _obj.resume(at);
            _output = handler.createReply();
                        }
        catch (org.omg.CosActivity.InvalidToken _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosActivity.InvalidTokenHelper.write(_output, _exception0);
            return _output;
        }
        catch (org.omg.CosActivity.InvalidParentContext _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosActivity.InvalidParentContextHelper.write(_output, _exception0);
            return _output;
        }
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _suspend_all_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.CurrentOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        org.omg.CosActivity.ActivityToken _result = _obj.suspend_all();
        _output = handler.createReply();
        org.omg.CosActivity.ActivityTokenHelper.write(_output, _result);
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _resume_all_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.CurrentOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            org.omg.CosActivity.ActivityToken at;
            at = org.omg.CosActivity.ActivityTokenHelper.read(_input);
            _obj.resume_all(at);
            _output = handler.createReply();
                        }
        catch (org.omg.CosActivity.InvalidToken _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosActivity.InvalidTokenHelper.write(_output, _exception0);
            return _output;
        }
        catch (org.omg.CosActivity.InvalidParentContext _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosActivity.InvalidParentContextHelper.write(_output, _exception0);
            return _output;
        }
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _get_global_id_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.CurrentOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        byte[] _result = _obj.get_global_id();
        _output = handler.createReply();
        org.omg.CosActivity.GlobalIdHelper.write(_output, _result);
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _get_status_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.CurrentOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        org.omg.CosActivity.Status _result = _obj.get_status();
        _output = handler.createReply();
        org.omg.CosActivity.StatusHelper.write(_output, _result);
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _get_activity_name_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.CurrentOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        java.lang.String _result = _obj.get_activity_name();
        _output = handler.createReply();
        _output.write_string(_result);
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _set_timeout_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.CurrentOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            int seconds;
            seconds = _input.read_long();
            _obj.set_timeout(seconds);
            _output = handler.createReply();
                        }
        catch (org.omg.CosActivity.TimeoutOutOfRange _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosActivity.TimeoutOutOfRangeHelper.write(_output, _exception0);
            return _output;
        }
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _get_timeout_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.CurrentOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        int _result = _obj.get_timeout();
        _output = handler.createReply();
        _output.write_long(_result);
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _get_context_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.CurrentOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        org.omg.CosActivity.ActivityContext _result = _obj.get_context();
        _output = handler.createReply();
        org.omg.CosActivity.ActivityContextHelper.write(_output, _result);
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _recreate_context_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.CurrentOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            org.omg.CosActivity.ActivityContext ctx;
            ctx = org.omg.CosActivity.ActivityContextHelper.read(_input);
            _obj.recreate_context(ctx);
            _output = handler.createReply();
                        }
        catch (org.omg.CosActivity.InvalidContext _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosActivity.InvalidContextHelper.write(_output, _exception0);
            return _output;
        }
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _get_coordinator_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.CurrentOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        org.omg.CosActivity.ActivityCoordinator _result = _obj.get_coordinator();
        _output = handler.createReply();
        org.omg.CosActivity.ActivityCoordinatorHelper.write(_output, _result);
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _get_parent_coordinator_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.CurrentOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        org.omg.CosActivity.ActivityCoordinator _result = _obj.get_parent_coordinator();
        _output = handler.createReply();
        org.omg.CosActivity.ActivityCoordinatorHelper.write(_output, _result);
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _get_identity_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.CurrentOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        org.omg.CosActivity.ActivityIdentity _result = _obj.get_identity();
        _output = handler.createReply();
        org.omg.CosActivity.ActivityIdentityHelper.write(_output, _result);
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _get_token_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.CurrentOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        org.omg.CosActivity.ActivityToken _result = _obj.get_token();
        _output = handler.createReply();
        org.omg.CosActivity.ActivityTokenHelper.write(_output, _result);
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _get_property_group_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.CurrentOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            java.lang.String name;
            name = _input.read_string();
            org.omg.CosActivity.PropertyGroup _result = _obj.get_property_group(name);
            _output = handler.createReply();
            org.omg.CosActivity.PropertyGroupHelper.write(_output, _result);
                        }
        catch (org.omg.CosActivity.PropertyGroupUnknown _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosActivity.PropertyGroupUnknownHelper.write(_output, _exception0);
            return _output;
        }
        catch (org.omg.CosActivity.NoActivity _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosActivity.NoActivityHelper.write(_output, _exception0);
            return _output;
        }
        return _output;
    }


    public String[]  _all_interfaces(
        org.omg.PortableServer.POA poa,
        byte[] objectId
    )
    {
        String __ids[] =  {"IDL:omg.org/CosActivity/Current:1.0"};
        return __ids;
    }

    public static void _Current_init_hash(
        java.util.HashMap _hash
    )
    {
        java.lang.Class[] params = {org.omg.CORBA.portable.InputStream.class,
            org.omg.CORBA.portable.ResponseHandler.class,
            org.omg.CosActivity.CurrentOperations.class};
            try {
                _hash.put("begin", org.omg.CosActivity.CurrentPOA.class.getDeclaredMethod("_begin_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("complete", org.omg.CosActivity.CurrentPOA.class.getDeclaredMethod("_complete_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("complete_with_status", org.omg.CosActivity.CurrentPOA.class.getDeclaredMethod("_complete_with_status_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("set_completion_status", org.omg.CosActivity.CurrentPOA.class.getDeclaredMethod("_set_completion_status_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("get_completion_status", org.omg.CosActivity.CurrentPOA.class.getDeclaredMethod("_get_completion_status_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("set_completion_signal_set", org.omg.CosActivity.CurrentPOA.class.getDeclaredMethod("_set_completion_signal_set_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("get_completion_signal_set", org.omg.CosActivity.CurrentPOA.class.getDeclaredMethod("_get_completion_signal_set_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("suspend", org.omg.CosActivity.CurrentPOA.class.getDeclaredMethod("_suspend_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("resume", org.omg.CosActivity.CurrentPOA.class.getDeclaredMethod("_resume_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("suspend_all", org.omg.CosActivity.CurrentPOA.class.getDeclaredMethod("_suspend_all_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("resume_all", org.omg.CosActivity.CurrentPOA.class.getDeclaredMethod("_resume_all_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("get_global_id", org.omg.CosActivity.CurrentPOA.class.getDeclaredMethod("_get_global_id_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("get_status", org.omg.CosActivity.CurrentPOA.class.getDeclaredMethod("_get_status_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("get_activity_name", org.omg.CosActivity.CurrentPOA.class.getDeclaredMethod("_get_activity_name_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("set_timeout", org.omg.CosActivity.CurrentPOA.class.getDeclaredMethod("_set_timeout_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("get_timeout", org.omg.CosActivity.CurrentPOA.class.getDeclaredMethod("_get_timeout_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("get_context", org.omg.CosActivity.CurrentPOA.class.getDeclaredMethod("_get_context_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("recreate_context", org.omg.CosActivity.CurrentPOA.class.getDeclaredMethod("_recreate_context_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("get_coordinator", org.omg.CosActivity.CurrentPOA.class.getDeclaredMethod("_get_coordinator_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("get_parent_coordinator", org.omg.CosActivity.CurrentPOA.class.getDeclaredMethod("_get_parent_coordinator_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("get_identity", org.omg.CosActivity.CurrentPOA.class.getDeclaredMethod("_get_identity_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("get_token", org.omg.CosActivity.CurrentPOA.class.getDeclaredMethod("_get_token_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("get_property_group", org.omg.CosActivity.CurrentPOA.class.getDeclaredMethod("_get_property_group_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
        }

        private static java.util.HashMap _Current_hashtable;

        static
        {
            try { 
                _Current_hashtable = new java.util.HashMap();
                org.omg.CosActivity.CurrentPOA._Current_init_hash(_Current_hashtable);
            } catch (java.lang.Exception excep) {
                excep.printStackTrace();
            }
        }

}
