package org.omg.CosActivity;

abstract public class ActivityCoordinatorPOA
    extends org.omg.PortableServer.Servant
    implements org.omg.CORBA.portable.InvokeHandler, ActivityCoordinatorOperations
{
    public org.omg.CosActivity.ActivityCoordinator _this()
    {
        return org.omg.CosActivity.ActivityCoordinatorHelper.narrow(_this_object());
    }

    public org.omg.CosActivity.ActivityCoordinator _this(
        org.omg.CORBA.ORB orb
)
    {
        return org.omg.CosActivity.ActivityCoordinatorHelper.narrow(_this_object(orb));
    }

    public org.omg.CORBA.portable.OutputStream _invoke(String opName, 
        org.omg.CORBA.portable.InputStream input, 
        org.omg.CORBA.portable.ResponseHandler handler    )
    {
        java.lang.Object[] _params = {input, handler, this};
        java.lang.reflect.Method _meth = (java.lang.reflect.Method)_ActivityCoordinator_hashtable.get(opName);
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

    public static org.omg.CORBA.portable.OutputStream _complete_activity_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.ActivityCoordinatorOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            java.lang.String signal_set_name;
            signal_set_name = _input.read_string();
            org.omg.CosActivity.CompletionStatus cs;
            cs = org.omg.CosActivity.CompletionStatusHelper.read(_input);
            org.omg.CosActivity.Outcome _result = _obj.complete_activity(signal_set_name,cs);
            _output = handler.createReply();
            org.omg.CosActivity.OutcomeHelper.write(_output, _result);
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
        catch (org.omg.CosActivity.SignalSetUnknown _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosActivity.SignalSetUnknownHelper.write(_output, _exception0);
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


    public static org.omg.CORBA.portable.OutputStream _process_signal_set_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.ActivityCoordinatorOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            java.lang.String signal_set_name;
            signal_set_name = _input.read_string();
            org.omg.CosActivity.CompletionStatus cs;
            cs = org.omg.CosActivity.CompletionStatusHelper.read(_input);
            org.omg.CosActivity.Outcome _result = _obj.process_signal_set(signal_set_name,cs);
            _output = handler.createReply();
            org.omg.CosActivity.OutcomeHelper.write(_output, _result);
                                    }
        catch (org.omg.CosActivity.SignalSetUnknown _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosActivity.SignalSetUnknownHelper.write(_output, _exception0);
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
        org.omg.CosActivity.ActivityCoordinatorOperations _obj 
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
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _remove_signal_set_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.ActivityCoordinatorOperations _obj 
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
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _add_action_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.ActivityCoordinatorOperations _obj 
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
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _remove_action_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.ActivityCoordinatorOperations _obj 
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
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _add_actions_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.ActivityCoordinatorOperations _obj 
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
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _remove_actions_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.ActivityCoordinatorOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        org.omg.CosActivity.Action[] acts;
        acts = org.omg.CosActivity.ActionSeqHelper.read(_input);
        org.omg.CosActivity.Action[] _result = _obj.remove_actions(acts);
        _output = handler.createReply();
        org.omg.CosActivity.ActionSeqHelper.write(_output, _result);
                return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _add_global_action_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.ActivityCoordinatorOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        org.omg.CosActivity.Action act;
        act = org.omg.CosActivity.ActionHelper.read(_input);
        int priority;
        priority = _input.read_long();
        _obj.add_global_action(act,priority);
        _output = handler.createReply();
                        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _remove_global_action_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.ActivityCoordinatorOperations _obj 
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
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _get_number_registered_actions_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.ActivityCoordinatorOperations _obj 
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
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _get_actions_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.ActivityCoordinatorOperations _obj 
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
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _get_parent_coordinator_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.ActivityCoordinatorOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        org.omg.CosActivity.ActivityCoordinator _result = _obj.get_parent_coordinator();
        _output = handler.createReply();
        org.omg.CosActivity.ActivityCoordinatorHelper.write(_output, _result);
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _get_global_id_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.ActivityCoordinatorOperations _obj 
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
        org.omg.CosActivity.ActivityCoordinatorOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        org.omg.CosActivity.Status _result = _obj.get_status();
        _output = handler.createReply();
        org.omg.CosActivity.StatusHelper.write(_output, _result);
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _get_parent_status_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.ActivityCoordinatorOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        org.omg.CosActivity.Status _result = _obj.get_parent_status();
        _output = handler.createReply();
        org.omg.CosActivity.StatusHelper.write(_output, _result);
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _get_activity_name_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.ActivityCoordinatorOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        java.lang.String _result = _obj.get_activity_name();
        _output = handler.createReply();
        _output.write_string(_result);
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _is_same_activity_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.ActivityCoordinatorOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        org.omg.CosActivity.ActivityCoordinator ac;
        ac = org.omg.CosActivity.ActivityCoordinatorHelper.read(_input);
        boolean _result = _obj.is_same_activity(ac);
        _output = handler.createReply();
        _output.write_boolean(_result);
                return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _hash_activity_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.ActivityCoordinatorOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        int _result = _obj.hash_activity();
        _output = handler.createReply();
        _output.write_ulong(_result);
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _destroy_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.ActivityCoordinatorOperations _obj 
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
        String __ids[] =  {"IDL:omg.org/CosActivity/ActivityCoordinator:1.0"};
        return __ids;
    }

    public static void _ActivityCoordinator_init_hash(
        java.util.HashMap _hash
    )
    {
        java.lang.Class[] params = {org.omg.CORBA.portable.InputStream.class,
            org.omg.CORBA.portable.ResponseHandler.class,
            org.omg.CosActivity.ActivityCoordinatorOperations.class};
            try {
                _hash.put("complete_activity", org.omg.CosActivity.ActivityCoordinatorPOA.class.getDeclaredMethod("_complete_activity_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("process_signal_set", org.omg.CosActivity.ActivityCoordinatorPOA.class.getDeclaredMethod("_process_signal_set_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("add_signal_set", org.omg.CosActivity.ActivityCoordinatorPOA.class.getDeclaredMethod("_add_signal_set_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("remove_signal_set", org.omg.CosActivity.ActivityCoordinatorPOA.class.getDeclaredMethod("_remove_signal_set_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("add_action", org.omg.CosActivity.ActivityCoordinatorPOA.class.getDeclaredMethod("_add_action_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("remove_action", org.omg.CosActivity.ActivityCoordinatorPOA.class.getDeclaredMethod("_remove_action_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("add_actions", org.omg.CosActivity.ActivityCoordinatorPOA.class.getDeclaredMethod("_add_actions_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("remove_actions", org.omg.CosActivity.ActivityCoordinatorPOA.class.getDeclaredMethod("_remove_actions_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("add_global_action", org.omg.CosActivity.ActivityCoordinatorPOA.class.getDeclaredMethod("_add_global_action_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("remove_global_action", org.omg.CosActivity.ActivityCoordinatorPOA.class.getDeclaredMethod("_remove_global_action_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("get_number_registered_actions", org.omg.CosActivity.ActivityCoordinatorPOA.class.getDeclaredMethod("_get_number_registered_actions_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("get_actions", org.omg.CosActivity.ActivityCoordinatorPOA.class.getDeclaredMethod("_get_actions_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("get_parent_coordinator", org.omg.CosActivity.ActivityCoordinatorPOA.class.getDeclaredMethod("_get_parent_coordinator_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("get_global_id", org.omg.CosActivity.ActivityCoordinatorPOA.class.getDeclaredMethod("_get_global_id_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("get_status", org.omg.CosActivity.ActivityCoordinatorPOA.class.getDeclaredMethod("_get_status_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("get_parent_status", org.omg.CosActivity.ActivityCoordinatorPOA.class.getDeclaredMethod("_get_parent_status_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("get_activity_name", org.omg.CosActivity.ActivityCoordinatorPOA.class.getDeclaredMethod("_get_activity_name_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("is_same_activity", org.omg.CosActivity.ActivityCoordinatorPOA.class.getDeclaredMethod("_is_same_activity_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("hash_activity", org.omg.CosActivity.ActivityCoordinatorPOA.class.getDeclaredMethod("_hash_activity_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("destroy", org.omg.CosActivity.ActivityCoordinatorPOA.class.getDeclaredMethod("_destroy_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
        }

        private static java.util.HashMap _ActivityCoordinator_hashtable;

        static
        {
            try { 
                _ActivityCoordinator_hashtable = new java.util.HashMap();
                org.omg.CosActivity.ActivityCoordinatorPOA._ActivityCoordinator_init_hash(_ActivityCoordinator_hashtable);
            } catch (java.lang.Exception excep) {
                excep.printStackTrace();
            }
        }

}
