//
// Java code automatically generated by the JART IDL Compiler
// Do not edit this code!
//
// This Holder class is generated to allow support for out and inout
// parameter passing modes

package org.omg.CosActivity;
public final class ActivityPendingHolder
    implements org.omg.CORBA.portable.Streamable
{
    public org.omg.CosActivity.ActivityPending value;

    public ActivityPendingHolder() {}

    public ActivityPendingHolder(
        org.omg.CosActivity.ActivityPending value
    )
    {
        this.value = value;
    }

    public void _read(
        org.omg.CORBA.portable.InputStream _stream
    )
    {
        value = org.omg.CosActivity.ActivityPendingHelper.read(_stream);
    }

    public void _write(
        org.omg.CORBA.portable.OutputStream _stream
    )
    {
        org.omg.CosActivity.ActivityPendingHelper.write(_stream, value);
    }

    public org.omg.CORBA.TypeCode _type()
    {
        return org.omg.CosActivity.ActivityPendingHelper.type();
    }

}
