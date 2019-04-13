//
// Java code automatically generated by the Orbix 2000 IDL Compiler
// Do not edit this code!
//
// This Helper file provides several static methods needed to
// manipulate the IDL type

package org.omg.CosActivity;

public class ActivityTokenHelper
{
    public static void insert(
        org.omg.CORBA.Any any,
        org.omg.CosActivity.ActivityToken value
    )
    {
        any.type(type());
        org.omg.CORBA.portable.OutputStream _stream = any.create_output_stream();
        write(_stream, value);
    }

    public static org.omg.CosActivity.ActivityToken extract(
        org.omg.CORBA.Any any
    )
    {
        if (!any.type().equal(type()))
        {
            throw new org.omg.CORBA.BAD_TYPECODE("Error inserting/extracting value from typecode.");
        }
        else
        {
            return read(any.create_input_stream());
        }
    }

    private static org.omg.CORBA.TypeCode _type;
    public static org.omg.CORBA.TypeCode type()
    {
        if (_type == null)
        {
            _type = _orb().create_interface_tc(id(), "ActivityToken");
        }
        return _type;
    }

    public static org.omg.CORBA.ORB _orb()
    {
        return org.omg.CORBA.ORB.init();
    }

    public static String id()
    {
        return "IDL:omg.org/CosActivity/ActivityToken:1.0";
    }

    public static org.omg.CosActivity.ActivityToken read(
        org.omg.CORBA.portable.InputStream _stream
    )
    {
        return (narrow(_stream.read_Object()));
    }

    public static void write(
        org.omg.CORBA.portable.OutputStream _stream,
        org.omg.CosActivity.ActivityToken value
    )
    {
        _stream.write_Object(value);
    }

    public static org.omg.CosActivity.ActivityToken narrow(
        org.omg.CORBA.Object _obj
    ) throws org.omg.CORBA.BAD_PARAM
    {
        if (_obj == null)
        {
            return null;
        }
        if (_obj instanceof org.omg.CosActivity.ActivityToken)
        {
            return (org.omg.CosActivity.ActivityToken)_obj;
        }
        if (_obj._is_a(id()))
        {
            org.omg.CosActivity._ActivityTokenStub _ref = new org.omg.CosActivity._ActivityTokenStub();
            _ref._set_delegate(((org.omg.CORBA.portable.ObjectImpl)_obj)._get_delegate());
            return _ref;
        }
        else
        {
            throw new org.omg.CORBA.BAD_PARAM();
        }
    }

}
