package org.omg.CosActivityAdministration;

abstract public class PropertyGroupAttributesPOA
    extends org.omg.PortableServer.Servant
    implements org.omg.CORBA.portable.InvokeHandler, PropertyGroupAttributesOperations
{
    public org.omg.CosActivityAdministration.PropertyGroupAttributes _this()
    {
        return org.omg.CosActivityAdministration.PropertyGroupAttributesHelper.narrow(_this_object());
    }

    public org.omg.CosActivityAdministration.PropertyGroupAttributes _this(
        org.omg.CORBA.ORB orb
)
    {
        return org.omg.CosActivityAdministration.PropertyGroupAttributesHelper.narrow(_this_object(orb));
    }

    public org.omg.CORBA.portable.OutputStream _invoke(String opName, 
        org.omg.CORBA.portable.InputStream input, 
        org.omg.CORBA.portable.ResponseHandler handler    )
    {
        java.lang.Object[] _params = {input, handler, this};
        java.lang.reflect.Method _meth = (java.lang.reflect.Method)_PropertyGroupAttributes_hashtable.get(opName);
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

    public static org.omg.CORBA.portable.OutputStream _get_attribute_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivityAdministration.PropertyGroupAttributesOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            java.lang.String name;
            name = _input.read_string();
            java.lang.String _result = _obj.get_attribute(name);
            _output = handler.createReply();
            _output.write_string(_result);
                        }
        catch (org.omg.CosActivityAdministration.NoSuchAttribute _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosActivityAdministration.NoSuchAttributeHelper.write(_output, _exception0);
            return _output;
        }
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _set_attribute_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivityAdministration.PropertyGroupAttributesOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            java.lang.String name;
            name = _input.read_string();
            java.lang.String value;
            value = _input.read_string();
            _obj.set_attribute(name,value);
            _output = handler.createReply();
                                    }
        catch (org.omg.CosActivityAdministration.AttributeAlreadyExists _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosActivityAdministration.AttributeAlreadyExistsHelper.write(_output, _exception0);
            return _output;
        }
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _replace_attribute_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivityAdministration.PropertyGroupAttributesOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        java.lang.String name;
        name = _input.read_string();
        java.lang.String value;
        value = _input.read_string();
        _obj.replace_attribute(name,value);
        _output = handler.createReply();
                        return _output;
    }


    public String[]  _all_interfaces(
        org.omg.PortableServer.POA poa,
        byte[] objectId
    )
    {
        String __ids[] =  {"IDL:omg.org/CosActivityAdministration/PropertyGroupAttributes:1.0"};
        return __ids;
    }

    public static void _PropertyGroupAttributes_init_hash(
        java.util.HashMap _hash
    )
    {
        java.lang.Class[] params = {org.omg.CORBA.portable.InputStream.class,
            org.omg.CORBA.portable.ResponseHandler.class,
            org.omg.CosActivityAdministration.PropertyGroupAttributesOperations.class};
            try {
                _hash.put("get_attribute", org.omg.CosActivityAdministration.PropertyGroupAttributesPOA.class.getDeclaredMethod("_get_attribute_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("set_attribute", org.omg.CosActivityAdministration.PropertyGroupAttributesPOA.class.getDeclaredMethod("_set_attribute_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("replace_attribute", org.omg.CosActivityAdministration.PropertyGroupAttributesPOA.class.getDeclaredMethod("_replace_attribute_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
        }

        private static java.util.HashMap _PropertyGroupAttributes_hashtable;

        static
        {
            try { 
                _PropertyGroupAttributes_hashtable = new java.util.HashMap();
                org.omg.CosActivityAdministration.PropertyGroupAttributesPOA._PropertyGroupAttributes_init_hash(_PropertyGroupAttributes_hashtable);
            } catch (java.lang.Exception excep) {
                excep.printStackTrace();
            }
        }

}
