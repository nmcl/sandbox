//
// Java code automatically generated by the JART IDL Compiler
// Do not edit this code!
//
// This Holder class is generated to allow support for out and inout
// parameter passing modes

package org.omg.CosActivityAdministration;
public final class PropertyGroupAlreadyRegisteredHolder
    implements org.omg.CORBA.portable.Streamable
{
    public org.omg.CosActivityAdministration.PropertyGroupAlreadyRegistered value;

    public PropertyGroupAlreadyRegisteredHolder() {}

    public PropertyGroupAlreadyRegisteredHolder(
        org.omg.CosActivityAdministration.PropertyGroupAlreadyRegistered value
    )
    {
        this.value = value;
    }

    public void _read(
        org.omg.CORBA.portable.InputStream _stream
    )
    {
        value = org.omg.CosActivityAdministration.PropertyGroupAlreadyRegisteredHelper.read(_stream);
    }

    public void _write(
        org.omg.CORBA.portable.OutputStream _stream
    )
    {
        org.omg.CosActivityAdministration.PropertyGroupAlreadyRegisteredHelper.write(_stream, value);
    }

    public org.omg.CORBA.TypeCode _type()
    {
        return org.omg.CosActivityAdministration.PropertyGroupAlreadyRegisteredHelper.type();
    }

}
