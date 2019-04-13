package org.omg.CosActivityAdministration;

abstract public class CurrentPOA
    extends org.omg.PortableServer.Servant
    implements org.omg.CORBA.portable.InvokeHandler, CurrentOperations, org.omg.CosActivity.CurrentOperations
{
    public org.omg.CosActivityAdministration.Current _this()
    {
        return org.omg.CosActivityAdministration.CurrentHelper.narrow(_this_object());
    }

    public org.omg.CosActivityAdministration.Current _this(
        org.omg.CORBA.ORB orb
)
    {
        return org.omg.CosActivityAdministration.CurrentHelper.narrow(_this_object(orb));
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

    public static org.omg.CORBA.portable.OutputStream _register_property_group_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivityAdministration.CurrentOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            java.lang.String property_group_name;
            property_group_name = _input.read_string();
            org.omg.CosActivityAdministration.PropertyGroupManager manager;
            manager = org.omg.CosActivityAdministration.PropertyGroupManagerHelper.read(_input);
            org.omg.CosActivityAdministration.PropertyGroupAttributes attributes;
            attributes = org.omg.CosActivityAdministration.PropertyGroupAttributesHelper.read(_input);
            _obj.register_property_group(property_group_name,manager,attributes);
            _output = handler.createReply();
                                                }
        catch (org.omg.CosActivityAdministration.PropertyGroupAlreadyRegistered _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosActivityAdministration.PropertyGroupAlreadyRegisteredHelper.write(_output, _exception0);
            return _output;
        }
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _unregister_property_group_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivityAdministration.CurrentOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            java.lang.String property_group_name;
            property_group_name = _input.read_string();
            _obj.unregister_property_group(property_group_name);
            _output = handler.createReply();
                        }
        catch (org.omg.CosActivityAdministration.PropertyGroupNotRegistered _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosActivityAdministration.PropertyGroupNotRegisteredHelper.write(_output, _exception0);
            return _output;
        }
        return _output;
    }


    public String[]  _all_interfaces(
        org.omg.PortableServer.POA poa,
        byte[] objectId
    )
    {
        String __ids[] =  {"IDL:omg.org/CosActivityAdministration/Current:1.0", "IDL:omg.org/CosActivity/Current:1.0"};
        return __ids;
    }

    public static void _Current_init_hash(
        java.util.HashMap _hash
    )
    {
        java.lang.Class[] params = {org.omg.CORBA.portable.InputStream.class,
            org.omg.CORBA.portable.ResponseHandler.class,
            org.omg.CosActivityAdministration.CurrentOperations.class};
            try {
                _hash.put("register_property_group", org.omg.CosActivityAdministration.CurrentPOA.class.getDeclaredMethod("_register_property_group_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("unregister_property_group", org.omg.CosActivityAdministration.CurrentPOA.class.getDeclaredMethod("_unregister_property_group_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
        }

        private static java.util.HashMap _Current_hashtable;

        static
        {
            try { 
                _Current_hashtable = new java.util.HashMap();
                org.omg.CosActivityAdministration.CurrentPOA._Current_init_hash(_Current_hashtable);
                org.omg.CosActivity.CurrentPOA._Current_init_hash(_Current_hashtable);
            } catch (java.lang.Exception excep) {
                excep.printStackTrace();
            }
        }

}
