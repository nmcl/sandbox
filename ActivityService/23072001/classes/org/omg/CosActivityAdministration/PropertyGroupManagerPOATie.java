package org.omg.CosActivityAdministration;

public class PropertyGroupManagerPOATie
    extends org.omg.CosActivityAdministration.PropertyGroupManagerPOA
{
        private org.omg.CosActivityAdministration.PropertyGroupManagerOperations _delegate;
        private org.omg.PortableServer.POA _poa;

        public PropertyGroupManagerPOATie(
            org.omg.CosActivityAdministration.PropertyGroupManagerOperations delegate
        )
        {
            _delegate = delegate;
        }

        public PropertyGroupManagerPOATie(
            org.omg.CosActivityAdministration.PropertyGroupManagerOperations delegate,
            org.omg.PortableServer.POA poa
        )
        {
            _delegate = delegate;
            _poa = poa;
        }

        public org.omg.CosActivityAdministration.PropertyGroupManagerOperations _delegate()
        {
            return _delegate;
        }

        public void _delegate(
            org.omg.CosActivityAdministration.PropertyGroupManagerOperations delegate
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

        public org.omg.CosActivity.PropertyGroup create(
            org.omg.CosActivity.PropertyGroup parent,
            byte[] gid
        )
        {
            return _delegate.create(parent, gid);
        }

        public org.omg.CosActivity.PropertyGroupIdentity marshal_request(
            org.omg.CosActivity.PropertyGroup pg
        )
        {
            return _delegate.marshal_request(pg);
        }

        public org.omg.CosActivity.PropertyGroupIdentity marshal_response(
            org.omg.CosActivity.PropertyGroup pg
        )
        {
            return _delegate.marshal_response(pg);
        }

        public org.omg.CosActivity.PropertyGroup unmarshal_request(
            org.omg.CosActivity.PropertyGroupIdentity mpg,
            org.omg.CosActivity.PropertyGroup pg,
            org.omg.CosActivity.PropertyGroup parent,
            byte[] gid
        )
        {
            return _delegate.unmarshal_request(mpg, pg, parent, gid);
        }

        public void unmarshal_response(
            org.omg.CosActivity.PropertyGroupIdentity mpg,
            org.omg.CosActivity.PropertyGroup pg
        )
        {
            _delegate.unmarshal_response(mpg, pg);
        }

        public void destroy() throws org.omg.CosActivity.AlreadyDestroyed
        {
            _delegate.destroy();
        }

}
