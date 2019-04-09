//
// Java code automatically generated by the Orbix 2000 IDL Compiler
// Do not edit this code!
//
// This Helper file provides several static methods needed to
// manipulate the IDL type

package org.omg.CosActivity;

public class PropertyGroupIdentityHelper
{
    public static void insert(
        org.omg.CORBA.Any any,
        org.omg.CosActivity.PropertyGroupIdentity value
    )
    {
        any.type(type());
        org.omg.CORBA.portable.OutputStream _stream = any.create_output_stream();
        write(_stream, value);
    }

    public static org.omg.CosActivity.PropertyGroupIdentity extract(
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
            org.omg.CORBA.StructMember[] _s = new org.omg.CORBA.StructMember[2];
            org.omg.CORBA.TypeCode _tc_property_group_name_0 = _orb().get_primitive_tc(org.omg.CORBA.TCKind.tk_string);
            _s[0] = new org.omg.CORBA.StructMember("property_group_name", _tc_property_group_name_0, null);
            org.omg.CORBA.TypeCode _tc_context_data_0 = _orb().get_primitive_tc(org.omg.CORBA.TCKind.tk_any);
            _s[1] = new org.omg.CORBA.StructMember("context_data", _tc_context_data_0, null);
            _type = _orb().create_struct_tc(id(),"PropertyGroupIdentity",_s);
        }
        return _type;
    }

    public static org.omg.CORBA.ORB _orb()
    {
        return org.omg.CORBA.ORB.init();
    }

    public static String id()
    {
        return "IDL:omg.org/CosActivity/PropertyGroupIdentity:1.0";
    }

    public static org.omg.CosActivity.PropertyGroupIdentity read(
        org.omg.CORBA.portable.InputStream _stream
    )
    {
        org.omg.CosActivity.PropertyGroupIdentity value = new org.omg.CosActivity.PropertyGroupIdentity();
        value.property_group_name = _stream.read_string();
        value.context_data = _stream.read_any();

        return value;
    }

    public static void write(
        org.omg.CORBA.portable.OutputStream _stream,
        org.omg.CosActivity.PropertyGroupIdentity value
    )
    {
        _stream.write_string(value.property_group_name);
        _stream.write_any(value.context_data);
    }

}
