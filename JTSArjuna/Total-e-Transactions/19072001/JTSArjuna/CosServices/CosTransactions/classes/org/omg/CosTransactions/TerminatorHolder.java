//
// Java code automatically generated by the JART IDL Compiler
// Do not edit this code!
//
// This Holder class is generated to allow support for out and inout
// parameter passing modes

package org.omg.CosTransactions;
public final class TerminatorHolder
    implements org.omg.CORBA.portable.Streamable
{
    public org.omg.CosTransactions.Terminator value;

    public TerminatorHolder() {}

    public TerminatorHolder(
        org.omg.CosTransactions.Terminator value
    )
    {
        this.value = value;
    }

    public void _read(
        org.omg.CORBA.portable.InputStream _stream
    )
    {
        value = org.omg.CosTransactions.TerminatorHelper.read(_stream);
    }

    public void _write(
        org.omg.CORBA.portable.OutputStream _stream
    )
    {
        org.omg.CosTransactions.TerminatorHelper.write(_stream, value);
    }

    public org.omg.CORBA.TypeCode _type()
    {
        return org.omg.CosTransactions.TerminatorHelper.type();
    }

}
