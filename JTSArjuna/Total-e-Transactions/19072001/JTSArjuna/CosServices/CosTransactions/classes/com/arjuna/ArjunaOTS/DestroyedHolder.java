//
// Java code automatically generated by the JART IDL Compiler
// Do not edit this code!
//
// This Holder class is generated to allow support for out and inout
// parameter passing modes

package com.arjuna.ArjunaOTS;
public final class DestroyedHolder
    implements org.omg.CORBA.portable.Streamable
{
    public com.arjuna.ArjunaOTS.Destroyed value;

    public DestroyedHolder() {}

    public DestroyedHolder(
        com.arjuna.ArjunaOTS.Destroyed value
    )
    {
        this.value = value;
    }

    public void _read(
        org.omg.CORBA.portable.InputStream _stream
    )
    {
        value = com.arjuna.ArjunaOTS.DestroyedHelper.read(_stream);
    }

    public void _write(
        org.omg.CORBA.portable.OutputStream _stream
    )
    {
        com.arjuna.ArjunaOTS.DestroyedHelper.write(_stream, value);
    }

    public org.omg.CORBA.TypeCode _type()
    {
        return com.arjuna.ArjunaOTS.DestroyedHelper.type();
    }

}
