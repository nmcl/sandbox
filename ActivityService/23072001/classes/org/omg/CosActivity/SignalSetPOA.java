package org.omg.CosActivity;

abstract public class SignalSetPOA
    extends org.omg.PortableServer.Servant
    implements org.omg.CORBA.portable.InvokeHandler, SignalSetOperations
{
    public org.omg.CosActivity.SignalSet _this()
    {
        return org.omg.CosActivity.SignalSetHelper.narrow(_this_object());
    }

    public org.omg.CosActivity.SignalSet _this(
        org.omg.CORBA.ORB orb
)
    {
        return org.omg.CosActivity.SignalSetHelper.narrow(_this_object(orb));
    }

    public org.omg.CORBA.portable.OutputStream _invoke(String opName, 
        org.omg.CORBA.portable.InputStream input, 
        org.omg.CORBA.portable.ResponseHandler handler    )
    {
        java.lang.Object[] _params = {input, handler, this};
        java.lang.reflect.Method _meth = (java.lang.reflect.Method)_SignalSet_hashtable.get(opName);
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

    public static org.omg.CORBA.portable.OutputStream __get_signal_set_name_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.SignalSetOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        java.lang.String _result = _obj.signal_set_name();
        _output = handler.createReply();
        _output.write_string(_result);
        return _output;
    }
    public static org.omg.CORBA.portable.OutputStream _get_signal_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.SignalSetOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        org.omg.CORBA.BooleanHolder lastSignal = new org.omg.CORBA.BooleanHolder();
        lastSignal.value = _input.read_boolean();
        org.omg.CosActivity.Signal _result = _obj.get_signal(lastSignal);
        _output = handler.createReply();
        org.omg.CosActivity.SignalHelper.write(_output, _result);
                _output.write_boolean(lastSignal.value);
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _get_outcome_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.SignalSetOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            org.omg.CosActivity.Outcome _result = _obj.get_outcome();
            _output = handler.createReply();
            org.omg.CosActivity.OutcomeHelper.write(_output, _result);
            }
        catch (org.omg.CosActivity.SignalSetActive _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosActivity.SignalSetActiveHelper.write(_output, _exception0);
            return _output;
        }
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _set_response_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.SignalSetOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            org.omg.CosActivity.Outcome response;
            response = org.omg.CosActivity.OutcomeHelper.read(_input);
            org.omg.CORBA.BooleanHolder nextSignal = new org.omg.CORBA.BooleanHolder();
            boolean _result = _obj.set_response(response,nextSignal);
            _output = handler.createReply();
            _output.write_boolean(_result);
                                    _output.write_boolean(nextSignal.value);
            }
        catch (org.omg.CosActivity.SignalSetInactive _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosActivity.SignalSetInactiveHelper.write(_output, _exception0);
            return _output;
        }
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _set_completion_status_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.SignalSetOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        org.omg.CosActivity.CompletionStatus cs;
        cs = org.omg.CosActivity.CompletionStatusHelper.read(_input);
        _obj.set_completion_status(cs);
        _output = handler.createReply();
                return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _get_completion_status_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.SignalSetOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        org.omg.CosActivity.CompletionStatus _result = _obj.get_completion_status();
        _output = handler.createReply();
        org.omg.CosActivity.CompletionStatusHelper.write(_output, _result);
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _set_activity_coordinator_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.SignalSetOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            org.omg.CosActivity.ActivityCoordinator coord;
            coord = org.omg.CosActivity.ActivityCoordinatorHelper.read(_input);
            _obj.set_activity_coordinator(coord);
            _output = handler.createReply();
                        }
        catch (org.omg.CosActivity.SignalSetActive _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosActivity.SignalSetActiveHelper.write(_output, _exception0);
            return _output;
        }
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _destroy_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.SignalSetOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            _obj.destroy();
            _output = handler.createReply();
            }
        catch (org.omg.CosActivity.AlreadyDestroyed _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosActivity.AlreadyDestroyedHelper.write(_output, _exception0);
            return _output;
        }
        return _output;
    }


    public String[]  _all_interfaces(
        org.omg.PortableServer.POA poa,
        byte[] objectId
    )
    {
        String __ids[] =  {"IDL:omg.org/CosActivity/SignalSet:1.0"};
        return __ids;
    }

    public static void _SignalSet_init_hash(
        java.util.HashMap _hash
    )
    {
        java.lang.Class[] params = {org.omg.CORBA.portable.InputStream.class,
            org.omg.CORBA.portable.ResponseHandler.class,
            org.omg.CosActivity.SignalSetOperations.class};
            try {
                _hash.put("_get_signal_set_name", org.omg.CosActivity.SignalSetPOA.class.getDeclaredMethod("__get_signal_set_name_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("get_signal", org.omg.CosActivity.SignalSetPOA.class.getDeclaredMethod("_get_signal_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("get_outcome", org.omg.CosActivity.SignalSetPOA.class.getDeclaredMethod("_get_outcome_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("set_response", org.omg.CosActivity.SignalSetPOA.class.getDeclaredMethod("_set_response_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("set_completion_status", org.omg.CosActivity.SignalSetPOA.class.getDeclaredMethod("_set_completion_status_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("get_completion_status", org.omg.CosActivity.SignalSetPOA.class.getDeclaredMethod("_get_completion_status_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("set_activity_coordinator", org.omg.CosActivity.SignalSetPOA.class.getDeclaredMethod("_set_activity_coordinator_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("destroy", org.omg.CosActivity.SignalSetPOA.class.getDeclaredMethod("_destroy_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
        }

        private static java.util.HashMap _SignalSet_hashtable;

        static
        {
            try { 
                _SignalSet_hashtable = new java.util.HashMap();
                org.omg.CosActivity.SignalSetPOA._SignalSet_init_hash(_SignalSet_hashtable);
            } catch (java.lang.Exception excep) {
                excep.printStackTrace();
            }
        }

}
