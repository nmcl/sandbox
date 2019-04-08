package org.omg.CosActivity;

abstract public class ActionPOA
    extends org.omg.PortableServer.Servant
    implements org.omg.CORBA.portable.InvokeHandler, ActionOperations
{
    public org.omg.CosActivity.Action _this()
    {
        return org.omg.CosActivity.ActionHelper.narrow(_this_object());
    }

    public org.omg.CosActivity.Action _this(
        org.omg.CORBA.ORB orb
)
    {
        return org.omg.CosActivity.ActionHelper.narrow(_this_object(orb));
    }

    public org.omg.CORBA.portable.OutputStream _invoke(String opName, 
        org.omg.CORBA.portable.InputStream input, 
        org.omg.CORBA.portable.ResponseHandler handler    )
    {
        java.lang.Object[] _params = {input, handler, this};
        java.lang.reflect.Method _meth = (java.lang.reflect.Method)_Action_hashtable.get(opName);
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

    public static org.omg.CORBA.portable.OutputStream _process_signal_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.ActionOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            org.omg.CosActivity.Signal sig;
            sig = org.omg.CosActivity.SignalHelper.read(_input);
            org.omg.CosActivity.Outcome _result = _obj.process_signal(sig);
            _output = handler.createReply();
            org.omg.CosActivity.OutcomeHelper.write(_output, _result);
                        }
        catch (org.omg.CosActivity.ActionError _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosActivity.ActionErrorHelper.write(_output, _exception0);
            return _output;
        }
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _destroy_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.ActionOperations _obj 
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
        String __ids[] =  {"IDL:omg.org/CosActivity/Action:1.0"};
        return __ids;
    }

    public static void _Action_init_hash(
        java.util.HashMap _hash
    )
    {
        java.lang.Class[] params = {org.omg.CORBA.portable.InputStream.class,
            org.omg.CORBA.portable.ResponseHandler.class,
            org.omg.CosActivity.ActionOperations.class};
            try {
                _hash.put("process_signal", org.omg.CosActivity.ActionPOA.class.getDeclaredMethod("_process_signal_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("destroy", org.omg.CosActivity.ActionPOA.class.getDeclaredMethod("_destroy_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
        }

        private static java.util.HashMap _Action_hashtable;

        static
        {
            try { 
                _Action_hashtable = new java.util.HashMap();
                org.omg.CosActivity.ActionPOA._Action_init_hash(_Action_hashtable);
            } catch (java.lang.Exception excep) {
                excep.printStackTrace();
            }
        }

}
