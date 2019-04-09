package org.omg.CosActivity;

abstract public class PropertyGroupPOA
    extends org.omg.PortableServer.Servant
    implements org.omg.CORBA.portable.InvokeHandler, PropertyGroupOperations
{
    public org.omg.CosActivity.PropertyGroup _this()
    {
        return org.omg.CosActivity.PropertyGroupHelper.narrow(_this_object());
    }

    public org.omg.CosActivity.PropertyGroup _this(
        org.omg.CORBA.ORB orb
)
    {
        return org.omg.CosActivity.PropertyGroupHelper.narrow(_this_object(orb));
    }

    public org.omg.CORBA.portable.OutputStream _invoke(String opName, 
        org.omg.CORBA.portable.InputStream input, 
        org.omg.CORBA.portable.ResponseHandler handler    )
    {
        java.lang.Object[] _params = {input, handler, this};
        java.lang.reflect.Method _meth = (java.lang.reflect.Method)_PropertyGroup_hashtable.get(opName);
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

    public static org.omg.CORBA.portable.OutputStream __get_property_group_name_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.PropertyGroupOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        java.lang.String _result = _obj.property_group_name();
        _output = handler.createReply();
        _output.write_string(_result);
        return _output;
    }
    public static org.omg.CORBA.portable.OutputStream _completed_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.PropertyGroupOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        _obj.completed();
        _output = handler.createReply();
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _suspended_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.PropertyGroupOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        _obj.suspended();
        _output = handler.createReply();
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _resumed_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.PropertyGroupOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        _obj.resumed();
        _output = handler.createReply();
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _destroy_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivity.PropertyGroupOperations _obj 
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
        String __ids[] =  {"IDL:omg.org/CosActivity/PropertyGroup:1.0"};
        return __ids;
    }

    public static void _PropertyGroup_init_hash(
        java.util.HashMap _hash
    )
    {
        java.lang.Class[] params = {org.omg.CORBA.portable.InputStream.class,
            org.omg.CORBA.portable.ResponseHandler.class,
            org.omg.CosActivity.PropertyGroupOperations.class};
            try {
                _hash.put("_get_property_group_name", org.omg.CosActivity.PropertyGroupPOA.class.getDeclaredMethod("__get_property_group_name_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("completed", org.omg.CosActivity.PropertyGroupPOA.class.getDeclaredMethod("_completed_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("suspended", org.omg.CosActivity.PropertyGroupPOA.class.getDeclaredMethod("_suspended_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("resumed", org.omg.CosActivity.PropertyGroupPOA.class.getDeclaredMethod("_resumed_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("destroy", org.omg.CosActivity.PropertyGroupPOA.class.getDeclaredMethod("_destroy_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
        }

        private static java.util.HashMap _PropertyGroup_hashtable;

        static
        {
            try { 
                _PropertyGroup_hashtable = new java.util.HashMap();
                org.omg.CosActivity.PropertyGroupPOA._PropertyGroup_init_hash(_PropertyGroup_hashtable);
            } catch (java.lang.Exception excep) {
                excep.printStackTrace();
            }
        }

}
