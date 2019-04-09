package org.omg.CosActivity;

public class PropertyGroupPOATie
    extends org.omg.CosActivity.PropertyGroupPOA
{
        private org.omg.CosActivity.PropertyGroupOperations _delegate;
        private org.omg.PortableServer.POA _poa;

        public PropertyGroupPOATie(
            org.omg.CosActivity.PropertyGroupOperations delegate
        )
        {
            _delegate = delegate;
        }

        public PropertyGroupPOATie(
            org.omg.CosActivity.PropertyGroupOperations delegate,
            org.omg.PortableServer.POA poa
        )
        {
            _delegate = delegate;
            _poa = poa;
        }

        public org.omg.CosActivity.PropertyGroupOperations _delegate()
        {
            return _delegate;
        }

        public void _delegate(
            org.omg.CosActivity.PropertyGroupOperations delegate
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

        public java.lang.String property_group_name()
        {
            return _delegate.property_group_name();
        }

        public void completed()
        {
            _delegate.completed();
        }

        public void suspended()
        {
            _delegate.suspended();
        }

        public void resumed()
        {
            _delegate.resumed();
        }

        public void destroy() throws org.omg.CosActivity.AlreadyDestroyed
        {
            _delegate.destroy();
        }

}
