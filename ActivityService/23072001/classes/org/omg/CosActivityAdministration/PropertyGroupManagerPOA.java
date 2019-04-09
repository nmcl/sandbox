package org.omg.CosActivityAdministration;

abstract public class PropertyGroupManagerPOA
    extends org.omg.PortableServer.Servant
    implements org.omg.CORBA.portable.InvokeHandler, PropertyGroupManagerOperations
{
    public org.omg.CosActivityAdministration.PropertyGroupManager _this()
    {
        return org.omg.CosActivityAdministration.PropertyGroupManagerHelper.narrow(_this_object());
    }

    public org.omg.CosActivityAdministration.PropertyGroupManager _this(
        org.omg.CORBA.ORB orb
)
    {
        return org.omg.CosActivityAdministration.PropertyGroupManagerHelper.narrow(_this_object(orb));
    }

    public org.omg.CORBA.portable.OutputStream _invoke(String opName, 
        org.omg.CORBA.portable.InputStream input, 
        org.omg.CORBA.portable.ResponseHandler handler    )
    {
        java.lang.Object[] _params = {input, handler, this};
        java.lang.reflect.Method _meth = (java.lang.reflect.Method)_PropertyGroupManager_hashtable.get(opName);
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

    public static org.omg.CORBA.portable.OutputStream _create_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivityAdministration.PropertyGroupManagerOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        org.omg.CosActivity.PropertyGroup parent;
        parent = org.omg.CosActivity.PropertyGroupHelper.read(_input);
        byte[] gid;
        gid = org.omg.CosActivity.GlobalIdHelper.read(_input);
        org.omg.CosActivity.PropertyGroup _result = _obj.create(parent,gid);
        _output = handler.createReply();
        org.omg.CosActivity.PropertyGroupHelper.write(_output, _result);
                        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _marshal_request_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivityAdministration.PropertyGroupManagerOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        org.omg.CosActivity.PropertyGroup pg;
        pg = org.omg.CosActivity.PropertyGroupHelper.read(_input);
        org.omg.CosActivity.PropertyGroupIdentity _result = _obj.marshal_request(pg);
        _output = handler.createReply();
        org.omg.CosActivity.PropertyGroupIdentityHelper.write(_output, _result);
                return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _marshal_response_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivityAdministration.PropertyGroupManagerOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        org.omg.CosActivity.PropertyGroup pg;
        pg = org.omg.CosActivity.PropertyGroupHelper.read(_input);
        org.omg.CosActivity.PropertyGroupIdentity _result = _obj.marshal_response(pg);
        _output = handler.createReply();
        org.omg.CosActivity.PropertyGroupIdentityHelper.write(_output, _result);
                return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _unmarshal_request_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivityAdministration.PropertyGroupManagerOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        org.omg.CosActivity.PropertyGroupIdentity mpg;
        mpg = org.omg.CosActivity.PropertyGroupIdentityHelper.read(_input);
        org.omg.CosActivity.PropertyGroup pg;
        pg = org.omg.CosActivity.PropertyGroupHelper.read(_input);
        org.omg.CosActivity.PropertyGroup parent;
        parent = org.omg.CosActivity.PropertyGroupHelper.read(_input);
        byte[] gid;
        gid = org.omg.CosActivity.GlobalIdHelper.read(_input);
        org.omg.CosActivity.PropertyGroup _result = _obj.unmarshal_request(mpg,pg,parent,gid);
        _output = handler.createReply();
        org.omg.CosActivity.PropertyGroupHelper.write(_output, _result);
                                        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _unmarshal_response_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivityAdministration.PropertyGroupManagerOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        org.omg.CosActivity.PropertyGroupIdentity mpg;
        mpg = org.omg.CosActivity.PropertyGroupIdentityHelper.read(_input);
        org.omg.CosActivity.PropertyGroup pg;
        pg = org.omg.CosActivity.PropertyGroupHelper.read(_input);
        _obj.unmarshal_response(mpg,pg);
        _output = handler.createReply();
                        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _destroy_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosActivityAdministration.PropertyGroupManagerOperations _obj 
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
        String __ids[] =  {"IDL:omg.org/CosActivityAdministration/PropertyGroupManager:1.0"};
        return __ids;
    }

    public static void _PropertyGroupManager_init_hash(
        java.util.HashMap _hash
    )
    {
        java.lang.Class[] params = {org.omg.CORBA.portable.InputStream.class,
            org.omg.CORBA.portable.ResponseHandler.class,
            org.omg.CosActivityAdministration.PropertyGroupManagerOperations.class};
            try {
                _hash.put("create", org.omg.CosActivityAdministration.PropertyGroupManagerPOA.class.getDeclaredMethod("_create_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("marshal_request", org.omg.CosActivityAdministration.PropertyGroupManagerPOA.class.getDeclaredMethod("_marshal_request_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("marshal_response", org.omg.CosActivityAdministration.PropertyGroupManagerPOA.class.getDeclaredMethod("_marshal_response_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("unmarshal_request", org.omg.CosActivityAdministration.PropertyGroupManagerPOA.class.getDeclaredMethod("_unmarshal_request_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("unmarshal_response", org.omg.CosActivityAdministration.PropertyGroupManagerPOA.class.getDeclaredMethod("_unmarshal_response_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("destroy", org.omg.CosActivityAdministration.PropertyGroupManagerPOA.class.getDeclaredMethod("_destroy_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
        }

        private static java.util.HashMap _PropertyGroupManager_hashtable;

        static
        {
            try { 
                _PropertyGroupManager_hashtable = new java.util.HashMap();
                org.omg.CosActivityAdministration.PropertyGroupManagerPOA._PropertyGroupManager_init_hash(_PropertyGroupManager_hashtable);
            } catch (java.lang.Exception excep) {
                excep.printStackTrace();
            }
        }

}
