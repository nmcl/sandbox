//
// Java code automatically generated by the Orbix 2000 IDL Compiler
// Do not edit this code!
//
// This Helper file provides several static methods needed to
// manipulate the IDL type

package com.arjuna.ArjunaOTS;

public class TransactionTypeHelper
{
    public static void insert(
        org.omg.CORBA.Any any,
        com.arjuna.ArjunaOTS.TransactionType value
    )
    {
        any.type(type());
        org.omg.CORBA.portable.OutputStream _stream = any.create_output_stream();
        write(_stream, value);
    }

    public static com.arjuna.ArjunaOTS.TransactionType extract(
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
            String _e[] = {"TransactionTypeActive", "TransactionTypeTerminated", "TransactionTypeUnresolved"};
            _type = _orb().create_enum_tc(id(), "TransactionType", _e);
        }
        return _type;
    }

    public static org.omg.CORBA.ORB _orb()
    {
        return org.omg.CORBA.ORB.init();
    }

    public static String id()
    {
        return "IDL:arjuna.com/ArjunaOTS/TransactionType:1.0";
    }

    public static com.arjuna.ArjunaOTS.TransactionType read(
        org.omg.CORBA.portable.InputStream _stream
    )
    {
        com.arjuna.ArjunaOTS.TransactionType value = com.arjuna.ArjunaOTS.TransactionType.from_int(_stream.read_ulong());
        return value;
    }

    public static void write(
        org.omg.CORBA.portable.OutputStream _stream,
        com.arjuna.ArjunaOTS.TransactionType value
    )
    {
        _stream.write_ulong(value.value());
    }

}
