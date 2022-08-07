package org.omg.CosTransactions;

public class RecoveryCoordinatorPOATie
    extends org.omg.CosTransactions.RecoveryCoordinatorPOA
{
        private org.omg.CosTransactions.RecoveryCoordinatorOperations _delegate;
        private org.omg.PortableServer.POA _poa;

        public RecoveryCoordinatorPOATie(
            org.omg.CosTransactions.RecoveryCoordinatorOperations delegate
        )
        {
            _delegate = delegate;
        }

        public RecoveryCoordinatorPOATie(
            org.omg.CosTransactions.RecoveryCoordinatorOperations delegate,
            org.omg.PortableServer.POA poa
        )
        {
            _delegate = delegate;
            _poa = poa;
        }

        public org.omg.CosTransactions.RecoveryCoordinatorOperations _delegate()
        {
            return _delegate;
        }

        public void _delegate(
            org.omg.CosTransactions.RecoveryCoordinatorOperations delegate
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

        public org.omg.CosTransactions.Status replay_completion(
            org.omg.CosTransactions.Resource r
        ) throws org.omg.CosTransactions.NotPrepared
        {
            return _delegate.replay_completion(r);
        }

}
