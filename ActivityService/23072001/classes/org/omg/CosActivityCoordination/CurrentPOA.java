package org.omg.CosActivityCoordination;

abstract public class CurrentPOA
    extends org.omg.PortableServer.Servant
    implements org.omg.CORBA.portable.InvokeHandler, CurrentOperations, org.omg.CosActivityAdministration.CurrentOperations
{
    public org.omg.CosActivityCoordination.Current _this()
    {
        return org.omg.CosActivityCoordination.CurrentHelper.narrow(_this_object());
    }

    public org.omg.CosActivityCoordination.Current _this(
        org.omg.CORBA.ORB orb
)
    {
        return org.omg.CosActivityCoordination.CurrentHelper.narrow(_this_object(orb));
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

    public static org.omg.CORBA.portable.OutputStream _broadcast_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivityCoordination.CurrentOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            java.lang.String signal_set_name;
            signal_set_name = _input.read_string();
            org.omg.CosActivity.CompletionStatus cs;
            cs = org.omg.CosActivity.CompletionStatusHelper.read(_input);
            org.omg.CosActivity.Outcome _result = _obj.broadcast(signal_set_name,cs);
            _output = handler.createReply();
            org.omg.CosActivity.OutcomeHelper.write(_output, _result);
                                    }
        catch (org.omg.CosActivity.SignalSetUnknown _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosActivity.SignalSetUnknownHelper.write(_output, _exception0);
            return _output;
        }
        catch (org.omg.CosActivity.NoActivity _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosActivity.NoActivityHelper.write(_output, _exception0);
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


    public static org.omg.CORBA.portable.OutputStream _add_signal_set_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivityCoordination.CurrentOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            org.omg.CosActivity.SignalSet signal_set;
            signal_set = org.omg.CosActivity.SignalSetHelper.read(_input);
            _obj.add_signal_set(signal_set);
            _output = handler.createReply();
                        }
        catch (org.omg.CosActivity.SignalSetAlreadyRegistered _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosActivity.SignalSetAlreadyRegisteredHelper.write(_output, _exception0);
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


    public static org.omg.CORBA.portable.OutputStream _remove_signal_set_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivityCoordination.CurrentOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            java.lang.String signal_set_name;
            signal_set_name = _input.read_string();
            _obj.remove_signal_set(signal_set_name);
            _output = handler.createReply();
                        }
        catch (org.omg.CosActivity.SignalSetUnknown _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosActivity.SignalSetUnknownHelper.write(_output, _exception0);
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


    public static org.omg.CORBA.portable.OutputStream _add_action_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivityCoordination.CurrentOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            org.omg.CosActivity.Action act;
            act = org.omg.CosActivity.ActionHelper.read(_input);
            java.lang.String signal_set_name;
            signal_set_name = _input.read_string();
            int priority;
            priority = _input.read_long();
            _obj.add_action(act,signal_set_name,priority);
            _output = handler.createReply();
                                                }
        catch (org.omg.CosActivity.SignalSetUnknown _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosActivity.SignalSetUnknownHelper.write(_output, _exception0);
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


    public static org.omg.CORBA.portable.OutputStream _remove_action_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivityCoordination.CurrentOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            org.omg.CosActivity.Action act;
            act = org.omg.CosActivity.ActionHelper.read(_input);
            _obj.remove_action(act);
            _output = handler.createReply();
                        }
        catch (org.omg.CosActivity.ActionNotFound _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosActivity.ActionNotFoundHelper.write(_output, _exception0);
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


    public static org.omg.CORBA.portable.OutputStream _add_actions_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivityCoordination.CurrentOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            org.omg.CosActivity.Action[] acts;
            acts = org.omg.CosActivity.ActionSeqHelper.read(_input);
            java.lang.String signal_set_name;
            signal_set_name = _input.read_string();
            int priority;
            priority = _input.read_long();
            _obj.add_actions(acts,signal_set_name,priority);
            _output = handler.createReply();
                                                }
        catch (org.omg.CosActivity.SignalSetUnknown _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosActivity.SignalSetUnknownHelper.write(_output, _exception0);
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


    public static org.omg.CORBA.portable.OutputStream _remove_actions_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivityCoordination.CurrentOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            org.omg.CosActivity.Action[] acts;
            acts = org.omg.CosActivity.ActionSeqHelper.read(_input);
            org.omg.CosActivity.Action[] _result = _obj.remove_actions(acts);
            _output = handler.createReply();
            org.omg.CosActivity.ActionSeqHelper.write(_output, _result);
                        }
        catch (org.omg.CosActivity.NoActivity _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosActivity.NoActivityHelper.write(_output, _exception0);
            return _output;
        }
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _add_global_action_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivityCoordination.CurrentOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            org.omg.CosActivity.Action act;
            act = org.omg.CosActivity.ActionHelper.read(_input);
            int priority;
            priority = _input.read_long();
            _obj.add_global_action(act,priority);
            _output = handler.createReply();
                                    }
        catch (org.omg.CosActivity.NoActivity _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosActivity.NoActivityHelper.write(_output, _exception0);
            return _output;
        }
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _remove_global_action_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivityCoordination.CurrentOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            org.omg.CosActivity.Action act;
            act = org.omg.CosActivity.ActionHelper.read(_input);
            _obj.remove_global_action(act);
            _output = handler.createReply();
                        }
        catch (org.omg.CosActivity.ActionNotFound _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosActivity.ActionNotFoundHelper.write(_output, _exception0);
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


    public static org.omg.CORBA.portable.OutputStream _get_number_registered_actions_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivityCoordination.CurrentOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            java.lang.String signal_set_name;
            signal_set_name = _input.read_string();
            int _result = _obj.get_number_registered_actions(signal_set_name);
            _output = handler.createReply();
            _output.write_long(_result);
                        }
        catch (org.omg.CosActivity.SignalSetUnknown _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosActivity.SignalSetUnknownHelper.write(_output, _exception0);
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


    public static org.omg.CORBA.portable.OutputStream _get_actions_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivityCoordination.CurrentOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            java.lang.String signal_set_name;
            signal_set_name = _input.read_string();
            org.omg.CosActivity.Action[] _result = _obj.get_actions(signal_set_name);
            _output = handler.createReply();
            org.omg.CosActivity.ActionSeqHelper.write(_output, _result);
                        }
        catch (org.omg.CosActivity.SignalSetUnknown _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosActivity.SignalSetUnknownHelper.write(_output, _exception0);
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
        String __ids[] =  {"IDL:omg.org/CosActivityCoordination/Current:1.0", "IDL:omg.org/CosActivityAdministration/Current:1.0", "IDL:omg.org/CosActivity/Current:1.0"};
        return __ids;
    }

    public static void _Current_init_hash(
        java.util.HashMap _hash
    )
    {
        java.lang.Class[] params = {org.omg.CORBA.portable.InputStream.class,
            org.omg.CORBA.portable.ResponseHandler.class,
            org.omg.CosActivityCoordination.CurrentOperations.class};
            try {
                _hash.put("broadcast", org.omg.CosActivityCoordination.CurrentPOA.class.getDeclaredMethod("_broadcast_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("add_signal_set", org.omg.CosActivityCoordination.CurrentPOA.class.getDeclaredMethod("_add_signal_set_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("remove_signal_set", org.omg.CosActivityCoordination.CurrentPOA.class.getDeclaredMethod("_remove_signal_set_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("add_action", org.omg.CosActivityCoordination.CurrentPOA.class.getDeclaredMethod("_add_action_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("remove_action", org.omg.CosActivityCoordination.CurrentPOA.class.getDeclaredMethod("_remove_action_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("add_actions", org.omg.CosActivityCoordination.CurrentPOA.class.getDeclaredMethod("_add_actions_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("remove_actions", org.omg.CosActivityCoordination.CurrentPOA.class.getDeclaredMethod("_remove_actions_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("add_global_action", org.omg.CosActivityCoordination.CurrentPOA.class.getDeclaredMethod("_add_global_action_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("remove_global_action", org.omg.CosActivityCoordination.CurrentPOA.class.getDeclaredMethod("_remove_global_action_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("get_number_registered_actions", org.omg.CosActivityCoordination.CurrentPOA.class.getDeclaredMethod("_get_number_registered_actions_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("get_actions", org.omg.CosActivityCoordination.CurrentPOA.class.getDeclaredMethod("_get_actions_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
        }

        private static java.util.HashMap _Current_hashtable;

        static
        {
            try { 
                _Current_hashtable = new java.util.HashMap();
                org.omg.CosActivityCoordination.CurrentPOA._Current_init_hash(_Current_hashtable);
                org.omg.CosActivityAdministration.CurrentPOA._Current_init_hash(_Current_hashtable);
                org.omg.CosActivity.CurrentPOA._Current_init_hash(_Current_hashtable);
            } catch (java.lang.Exception excep) {
                excep.printStackTrace();
            }
        }

}
