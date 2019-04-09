package org.omg.CosActivityAdministration;

public class PropertyGroupAttributesPOATie
    extends org.omg.CosActivityAdministration.PropertyGroupAttributesPOA
{
        private org.omg.CosActivityAdministration.PropertyGroupAttributesOperations _delegate;
        private org.omg.PortableServer.POA _poa;

        public PropertyGroupAttributesPOATie(
            org.omg.CosActivityAdministration.PropertyGroupAttributesOperations delegate
        )
        {
            _delegate = delegate;
        }

        public PropertyGroupAttributesPOATie(
            org.omg.CosActivityAdministration.PropertyGroupAttributesOperations delegate,
            org.omg.PortableServer.POA poa
        )
        {
            _delegate = delegate;
            _poa = poa;
        }

        public org.omg.CosActivityAdministration.PropertyGroupAttributesOperations _delegate()
        {
            return _delegate;
        }

        public void _delegate(
            org.omg.CosActivityAdministration.PropertyGroupAttributesOperations delegate
        )
        {
            _delegate = delegate;
        }

        public org.omg.PortableServer.POA _default_POA()
        {
            if (_poa != null)
            {
                return _poa;
            }

            else
            {
                return super._default_POA();
            }

        }

        public java.lang.String get_attribute(
            java.lang.String name
        ) throws org.omg.CosActivityAdministration.NoSuchAttribute
        {
            return _delegate.get_attribute(name);
        }

        public void set_attribute(
            java.lang.String name,
            java.lang.String value
        ) throws org.omg.CosActivityAdministration.AttributeAlreadyExists
        {
            _delegate.set_attribute(name, value);
        }

        public void replace_attribute(
            java.lang.String name,
            java.lang.String value
        )
        {
            _delegate.replace_attribute(name, value);
        }

}
