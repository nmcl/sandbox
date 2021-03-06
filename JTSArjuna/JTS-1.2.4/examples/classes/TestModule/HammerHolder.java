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
// IDL:/TestModule/Hammer:1.0
//
final public class HammerHolder implements org.omg.CORBA.portable.Streamable
{
    public Hammer value;

    public
    HammerHolder()
    {
    }

    public
    HammerHolder(Hammer initial)
    {
        value = initial;
    }

    public void
    _read(org.omg.CORBA.portable.InputStream in)
    {
        value = HammerHelper.read(in);
    }

    public void
    _write(org.omg.CORBA.portable.OutputStream out)
    {
        HammerHelper.write(out, value);
    }

    public org.omg.CORBA.TypeCode
    _type()
    {
        return HammerHelper.type();
    }
}
