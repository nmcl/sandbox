package org.omg.CosActivity;

abstract public class SubordinateSignalSetPOA
    extends org.omg.PortableServer.Servant
    implements org.omg.CORBA.portable.InvokeHandler, SubordinateSignalSetOperations, org.omg.CosActivity.SignalSetOperations
{
    public org.omg.CosActivity.SubordinateSignalSet _this()
    {
        return org.omg.CosActivity.SubordinateSignalSetHelper.narrow(_this_object());
    }

    public org.omg.CosActivity.SubordinateSignalSet _this(
        org.omg.CORBA.ORB orb
)
    {
        return org.omg.CosActivity.SubordinateSignalSetHelper.narrow(_this_object(orb));
    }

    public org.omg.CORBA.portable.OutputStream _invoke(String opName, 
        org.omg.CORBA.portable.InputStream input, 
        org.omg.CORBA.portable.ResponseHandler handler    )
    {
        java.lang.Object[] _params = {input, handler, this};
        java.lang.reflect.Method _meth = (java.lang.reflect.Method)_SubordinateSignalSet_hashtable.get(opName);
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

    public static org.omg.CORBA.portable.OutputStream _set_signal_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.SubordinateSignalSetOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        org.omg.CosActivity.Signal sig;
        sig = org.omg.CosActivity.SignalHelper.read(_input);
        _obj.set_signal(sig);
        _output = handler.createReply();
                return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _get_current_outcome_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.SubordinateSignalSetOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            org.omg.CosActivity.Outcome _result = _obj.get_current_outcome();
            _output = handler.createReply();
            org.omg.CosActivity.OutcomeHelper.write(_output, _result);
            }
        catch (org.omg.CosActivity.SignalSetInactive _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosActivity.SignalSetInactiveHelper.write(_output, _exception0);
            return _output;
        }
        return _output;
    }


    public String[]  _all_interfaces(
        org.omg.PortableServer.POA poa,
        byte[] objectId
    )
    {
        String __ids[] =  {"IDL:omg.org/CosActivity/SubordinateSignalSet:1.0", "IDL:omg.org/CosActivity/SignalSet:1.0"};
        return __ids;
    }

    public static void _SubordinateSignalSet_init_hash(
        java.util.HashMap _hash
    )
    {
        java.lang.Class[] params = {org.omg.CORBA.portable.InputStream.class,
            org.omg.CORBA.portable.ResponseHandler.class,
            org.omg.CosActivity.SubordinateSignalSetOperations.class};
            try {
                _hash.put("set_signal", org.omg.CosActivity.SubordinateSignalSetPOA.class.getDeclaredMethod("_set_signal_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("get_current_outcome", org.omg.CosActivity.SubordinateSignalSetPOA.class.getDeclaredMethod("_get_current_outcome_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
        }

        private static java.util.HashMap _SubordinateSignalSet_hashtable;

        static
        {
            try { 
                _SubordinateSignalSet_hashtable = new java.util.HashMap();
                org.omg.CosActivity.SubordinateSignalSetPOA._SubordinateSignalSet_init_hash(_SubordinateSignalSet_hashtable);
                org.omg.CosActivity.SignalSetPOA._SignalSet_init_hash(_SubordinateSignalSet_hashtable);
            } catch (java.lang.Exception excep) {
                excep.printStackTrace();
            }
        }

}
