//
// Java code automatically generated by the JART IDL Compiler
// Do not edit this code!
//
// This Holder class is generated to allow support for out and inout
// parameter passing modes

package org.omg.CosTransactions;
public final class ControlHolder
    implements org.omg.CORBA.portable.Streamable
{
    public org.omg.CosTransactions.Control value;

    public ControlHolder() {}

    public ControlHolder(
        org.omg.CosTransactions.Control value
    )
    {
        this.value = value;
    }

    public void _read(
        org.omg.CORBA.portable.InputStream _stream
    )
    {
        value = org.omg.CosTransactions.ControlHelper.read(_stream);
    }

    public void _write(
        org.omg.CORBA.portable.OutputStream _stream
    )
    {
        org.omg.CosTransactions.ControlHelper.write(_stream, value);
    }

    public org.omg.CORBA.TypeCode _type()
    {
        return org.omg.CosTransactions.ControlHelper.type();
    }

}
