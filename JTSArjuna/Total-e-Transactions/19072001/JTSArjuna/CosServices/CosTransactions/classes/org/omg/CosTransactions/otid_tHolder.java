//
// Java code automatically generated by the JART IDL Compiler
// Do not edit this code!
//
// This Holder class is generated to allow support for out and inout
// parameter passing modes

package org.omg.CosTransactions;
public final class otid_tHolder
    implements org.omg.CORBA.portable.Streamable
{
    public org.omg.CosTransactions.otid_t value;

    public otid_tHolder() {}

    public otid_tHolder(
        org.omg.CosTransactions.otid_t value
    )
    {
        this.value = value;
    }

    public void _read(
        org.omg.CORBA.portable.InputStream _stream
    )
    {
        value = org.omg.CosTransactions.otid_tHelper.read(_stream);
    }

    public void _write(
        org.omg.CORBA.portable.OutputStream _stream
    )
    {
        org.omg.CosTransactions.otid_tHelper.write(_stream, value);
    }

    public org.omg.CORBA.TypeCode _type()
    {
        return org.omg.CosTransactions.otid_tHelper.type();
    }

}
