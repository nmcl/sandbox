//
// Java code automatically generated by the JART IDL Compiler
// Do not edit this code!
//
// This Holder class is generated to allow support for out and inout
// parameter passing modes

package org.omg.CosActivity;
public final class ActivityCoordinatorHolder
    implements org.omg.CORBA.portable.Streamable
{
    public org.omg.CosActivity.ActivityCoordinator value;

    public ActivityCoordinatorHolder() {}

    public ActivityCoordinatorHolder(
        org.omg.CosActivity.ActivityCoordinator value
    )
    {
        this.value = value;
    }

    public void _read(
        org.omg.CORBA.portable.InputStream _stream
    )
    {
        value = org.omg.CosActivity.ActivityCoordinatorHelper.read(_stream);
    }

    public void _write(
        org.omg.CORBA.portable.OutputStream _stream
    )
    {
        org.omg.CosActivity.ActivityCoordinatorHelper.write(_stream, value);
    }

    public org.omg.CORBA.TypeCode _type()
    {
        return org.omg.CosActivity.ActivityCoordinatorHelper.type();
    }

}
