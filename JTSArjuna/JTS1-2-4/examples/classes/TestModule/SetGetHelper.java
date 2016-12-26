// **********************************************************************
//
// Generated by the ORBacus IDL to Java Translator
//
// Copyright (c) 1999
// Object-Oriented Concepts, Inc.
// Billerica, MA, USA
//
// Copyright (c) 1999
// Object-Oriented Concepts GmbH
// Ettlingen, Germany
//
// All Rights Reserved
//
// **********************************************************************

// Version: 3.1.3
// License: non-commercial

package TestModule;

//
// IDL:/TestModule/SetGet:1.0
//
final public class SetGetHelper
{
    public static void
    insert(org.omg.CORBA.Any any, SetGet val)
    {
        org.omg.CORBA.portable.OutputStream out = any.create_output_stream();
        write(out, val);
        any.read_value(out.create_input_stream(), type());
    }

    public static SetGet
    extract(org.omg.CORBA.Any any)
    {
        if(any.type().equal(type()))
            return read(any.create_input_stream());
        else
            throw new org.omg.CORBA.BAD_OPERATION();
    }

    private static org.omg.CORBA.TypeCode typeCode_;

    public static org.omg.CORBA.TypeCode
    type()
    {
        if(typeCode_ == null)
        {
            org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init();
            typeCode_ = orb.create_interface_tc(id(), "SetGet");
        }

        return typeCode_;
    }

    public static String
    id()
    {
        return "IDL:/TestModule/SetGet:1.0";
    }

    public static SetGet
    read(org.omg.CORBA.portable.InputStream in)
    {
        org.omg.CORBA.Object obj = in.read_Object();
        if(obj == null)
            return null;

        try
        {
            return (SetGet)obj;
        }
        catch(ClassCastException ex)
        {
        }

        org.omg.CORBA.portable.ObjectImpl impl;
        impl = (org.omg.CORBA.portable.ObjectImpl)obj;
        StubForSetGet stub = new StubForSetGet();
        stub._set_delegate(impl._get_delegate());
        return stub;
    }

    public static void
    write(org.omg.CORBA.portable.OutputStream out, SetGet val)
    {
        out.write_Object(val);
    }

    public static SetGet
    narrow(org.omg.CORBA.Object obj)
    {
        if(obj != null)
        {
            try
            {
                return (SetGet)obj;
            }
            catch(ClassCastException ex)
            {
            }

            if(obj._is_a(id()))
            {
                org.omg.CORBA.portable.ObjectImpl impl;
                impl = (org.omg.CORBA.portable.ObjectImpl)obj;
                StubForSetGet stub = new StubForSetGet();
                stub._set_delegate(impl._get_delegate());
                return stub;
            }
        }

        return null;
    }
}
