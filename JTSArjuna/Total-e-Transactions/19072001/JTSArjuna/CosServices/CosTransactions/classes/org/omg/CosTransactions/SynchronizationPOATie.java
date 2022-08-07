package org.omg.CosTransactions;

public class SynchronizationPOATie
    extends org.omg.CosTransactions.SynchronizationPOA
{
        private org.omg.CosTransactions.SynchronizationOperations _delegate;
        private org.omg.PortableServer.POA _poa;

        public SynchronizationPOATie(
            org.omg.CosTransactions.SynchronizationOperations delegate
        )
        {
            _delegate = delegate;
        }

        public SynchronizationPOATie(
            org.omg.CosTransactions.SynchronizationOperations delegate,
            org.omg.PortableServer.POA poa
        )
        {
            _delegate = delegate;
            _poa = poa;
        }

        public org.omg.CosTransactions.SynchronizationOperations _delegate()
        {
            return _delegate;
        }

        public void _delegate(
            org.omg.CosTransactions.SynchronizationOperations delegate
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

        public void before_completion()
        {
            _delegate.before_completion();
        }

        public void after_completion(
            org.omg.CosTransactions.Status s
        )
        {
            _delegate.after_completion(s);
        }

}
