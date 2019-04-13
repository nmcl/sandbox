//
// Java code automatically generated by the Orbix 2000 IDL Compiler
// Do not edit this code!
//
// This Helper file provides several static methods needed to
// manipulate the IDL type

package org.omg.CosActivity;

public class ActivityContextHelper
{
    public static void insert(
        org.omg.CORBA.Any any,
        org.omg.CosActivity.ActivityContext value
    )
    {
        any.type(type());
        org.omg.CORBA.portable.OutputStream _stream = any.create_output_stream();
        write(_stream, value);
    }

    public static org.omg.CosActivity.ActivityContext extract(
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
            org.omg.CORBA.TypeCode _tc_hierarchy_0 = org.omg.CosActivity.ActivityContextPackage.hierarchyHelper.type();
            _s[0] = new org.omg.CORBA.StructMember("hierarchy", _tc_hierarchy_0, null);
            org.omg.CORBA.TypeCode _tc_invocation_specific_data_0 = _orb().get_primitive_tc(org.omg.CORBA.TCKind.tk_any);
            _s[1] = new org.omg.CORBA.StructMember("invocation_specific_data", _tc_invocation_specific_data_0, null);
            _type = _orb().create_struct_tc(id(),"ActivityContext",_s);
        }
        return _type;
    }

    public static org.omg.CORBA.ORB _orb()
    {
        return org.omg.CORBA.ORB.init();
    }

    public static String id()
    {
        return "IDL:omg.org/CosActivity/ActivityContext:1.0";
    }

    public static org.omg.CosActivity.ActivityContext read(
        org.omg.CORBA.portable.InputStream _stream
    )
    {
        org.omg.CosActivity.ActivityContext value = new org.omg.CosActivity.ActivityContext();
        value.hierarchy = org.omg.CosActivity.ActivityContextPackage.hierarchyHelper.read(_stream);
        value.invocation_specific_data = _stream.read_any();

        return value;
    }

    public static void write(
        org.omg.CORBA.portable.OutputStream _stream,
        org.omg.CosActivity.ActivityContext value
    )
    {
        org.omg.CosActivity.ActivityContextPackage.hierarchyHelper.write(_stream, value.hierarchy);
        _stream.write_any(value.invocation_specific_data);
    }

}
