package org.omg.CosActivity;

public class ActionPOATie
    extends org.omg.CosActivity.ActionPOA
{
        private org.omg.CosActivity.ActionOperations _delegate;
        private org.omg.PortableServer.POA _poa;

        public ActionPOATie(
            org.omg.CosActivity.ActionOperations delegate
        )
        {
            _delegate = delegate;
        }

        public ActionPOATie(
            org.omg.CosActivity.ActionOperations delegate,
            org.omg.PortableServer.POA poa
        )
        {
            _delegate = delegate;
            _poa = poa;
        }

        public org.omg.CosActivity.ActionOperations _delegate()
        {
            return _delegate;
        }

        public void _delegate(
            org.omg.CosActivity.ActionOperations delegate
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

        public org.omg.CosActivity.Outcome process_signal(
            org.omg.CosActivity.Signal sig
        ) throws org.omg.CosActivity.ActionError
        {
            return _delegate.process_signal(sig);
        }

        public void destroy() throws org.omg.CosActivity.AlreadyDestroyed
        {
            _delegate.destroy();
        }

}
