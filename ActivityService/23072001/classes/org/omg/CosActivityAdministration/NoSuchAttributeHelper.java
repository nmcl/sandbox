//
// Java code automatically generated by the Orbix 2000 IDL Compiler
// Do not edit this code!
//
// This Helper file provides several static methods needed to
// manipulate the IDL type

package org.omg.CosActivityAdministration;

public class NoSuchAttributeHelper
{
    public static void insert(
        org.omg.CORBA.Any any,
        org.omg.CosActivityAdministration.NoSuchAttribute value
    )
    {
        any.type(type());
        org.omg.CORBA.portable.OutputStream _stream = any.create_output_stream();
        write(_stream, value);
    }

    public static org.omg.CosActivityAdministration.NoSuchAttribute extract(
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
            org.omg.CORBA.StructMember[] _s = new org.omg.CORBA.StructMember[0];
            _type = _orb().create_exception_tc(id(),"NoSuchAttribute",_s);
        }
        return _type;
    }

    public static org.omg.CORBA.ORB _orb()
    {
        return org.omg.CORBA.ORB.init();
    }

    public static String id()
    {
        return "IDL:omg.org/CosActivityAdministration/NoSuchAttribute:1.0";
    }

    public static org.omg.CosActivityAdministration.NoSuchAttribute read(
        org.omg.CORBA.portable.InputStream _stream
    )
    {
        // read and discard the repository id
        _stream.read_string();
        org.omg.CosActivityAdministration.NoSuchAttribute value = new org.omg.CosActivityAdministration.NoSuchAttribute();

        return value;
    }

    public static void write(
        org.omg.CORBA.portable.OutputStream _stream,
        org.omg.CosActivityAdministration.NoSuchAttribute value
    )
    {
        _stream.write_string(id());
    }

}
