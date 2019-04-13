package org.omg.CosActivity;

abstract public class ActivityTokenPOA
    extends org.omg.PortableServer.Servant
    implements org.omg.CORBA.portable.InvokeHandler, ActivityTokenOperations
{
    public org.omg.CosActivity.ActivityToken _this()
    {
        return org.omg.CosActivity.ActivityTokenHelper.narrow(_this_object());
    }

    public org.omg.CosActivity.ActivityToken _this(
        org.omg.CORBA.ORB orb
)
    {
        return org.omg.CosActivity.ActivityTokenHelper.narrow(_this_object(orb));
    }

    public org.omg.CORBA.portable.OutputStream _invoke(String opName, 
        org.omg.CORBA.portable.InputStream input, 
        org.omg.CORBA.portable.ResponseHandler handler    )
    {
        java.lang.Object[] _params = {input, handler, this};
        java.lang.reflect.Method _meth = (java.lang.reflect.Method)_ActivityToken_hashtable.get(opName);
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

    public static org.omg.CORBA.portable.OutputStream _get_context_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.ActivityTokenOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        org.omg.CosActivity.ActivityContext _result = _obj.get_context();
        _output = handler.createReply();
        org.omg.CosActivity.ActivityContextHelper.write(_output, _result);
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _destroy_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.ActivityTokenOperations _obj 
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
        String __ids[] =  {"IDL:omg.org/CosActivity/ActivityToken:1.0"};
        return __ids;
    }

    public static void _ActivityToken_init_hash(
        java.util.HashMap _hash
    )
    {
        java.lang.Class[] params = {org.omg.CORBA.portable.InputStream.class,
            org.omg.CORBA.portable.ResponseHandler.class,
            org.omg.CosActivity.ActivityTokenOperations.class};
            try {
                _hash.put("get_context", org.omg.CosActivity.ActivityTokenPOA.class.getDeclaredMethod("_get_context_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("destroy", org.omg.CosActivity.ActivityTokenPOA.class.getDeclaredMethod("_destroy_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
        }

        private static java.util.HashMap _ActivityToken_hashtable;

        static
        {
            try { 
                _ActivityToken_hashtable = new java.util.HashMap();
                org.omg.CosActivity.ActivityTokenPOA._ActivityToken_init_hash(_ActivityToken_hashtable);
            } catch (java.lang.Exception excep) {
                excep.printStackTrace();
            }
        }

}
