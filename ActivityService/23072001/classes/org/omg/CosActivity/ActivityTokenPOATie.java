package org.omg.CosActivity;

public class ActivityTokenPOATie
    extends org.omg.CosActivity.ActivityTokenPOA
{
        private org.omg.CosActivity.ActivityTokenOperations _delegate;
        private org.omg.PortableServer.POA _poa;

        public ActivityTokenPOATie(
            org.omg.CosActivity.ActivityTokenOperations delegate
        )
        {
            _delegate = delegate;
        }

        public ActivityTokenPOATie(
            org.omg.CosActivity.ActivityTokenOperations delegate,
            org.omg.PortableServer.POA poa
        )
        {
            _delegate = delegate;
            _poa = poa;
        }

        public org.omg.CosActivity.ActivityTokenOperations _delegate()
        {
            return _delegate;
        }

        public void _delegate(
            org.omg.CosActivity.ActivityTokenOperations delegate
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

        public org.omg.CosActivity.ActivityContext get_context()
        {
            return _delegate.get_context();
        }

        public void destroy() throws org.omg.CosActivity.AlreadyDestroyed
        {
            _delegate.destroy();
        }

}
