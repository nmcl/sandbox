//
// Java code automatically generated by the JART IDL Compiler
// Do not edit this code!
//
// This Holder class is generated to allow support for out and inout
// parameter passing modes

package org.omg.CosTransactions;
public final class ResourceHolder
    implements org.omg.CORBA.portable.Streamable
{
    public org.omg.CosTransactions.Resource value;

    public ResourceHolder() {}

    public ResourceHolder(
        org.omg.CosTransactions.Resource value
    )
    {
        this.value = value;
    }

    public void _read(
        org.omg.CORBA.portable.InputStream _stream
    )
    {
        value = org.omg.CosTransactions.ResourceHelper.read(_stream);
    }

    public void _write(
        org.omg.CORBA.portable.OutputStream _stream
    )
    {
        org.omg.CosTransactions.ResourceHelper.write(_stream, value);
    }

    public org.omg.CORBA.TypeCode _type()
    {
        return org.omg.CosTransactions.ResourceHelper.type();
    }

}
