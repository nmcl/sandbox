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
// IDL:/TestModule/tranobject:1.0
//
final public class tranobjectHolder implements org.omg.CORBA.portable.Streamable
{
    public tranobject value;

    public
    tranobjectHolder()
    {
    }

    public
    tranobjectHolder(tranobject initial)
    {
        value = initial;
    }

    public void
    _read(org.omg.CORBA.portable.InputStream in)
    {
        value = tranobjectHelper.read(in);
    }

    public void
    _write(org.omg.CORBA.portable.OutputStream out)
    {
        tranobjectHelper.write(out, value);
    }

    public org.omg.CORBA.TypeCode
    _type()
    {
        return tranobjectHelper.type();
    }
}
