//
// Java code automatically generated by the JART IDL Compiler
// Do not edit this code!
//
// This Holder class is generated to allow support for out and inout
// parameter passing modes

package org.omg.CosTransactions;
public final class SubtransactionsUnavailableHolder
    implements org.omg.CORBA.portable.Streamable
{
    public org.omg.CosTransactions.SubtransactionsUnavailable value;

    public SubtransactionsUnavailableHolder() {}

    public SubtransactionsUnavailableHolder(
        org.omg.CosTransactions.SubtransactionsUnavailable value
    )
    {
        this.value = value;
    }

    public void _read(
        org.omg.CORBA.portable.InputStream _stream
    )
    {
        value = org.omg.CosTransactions.SubtransactionsUnavailableHelper.read(_stream);
    }

    public void _write(
        org.omg.CORBA.portable.OutputStream _stream
    )
    {
        org.omg.CosTransactions.SubtransactionsUnavailableHelper.write(_stream, value);
    }

    public org.omg.CORBA.TypeCode _type()
    {
        return org.omg.CosTransactions.SubtransactionsUnavailableHelper.type();
    }

}
