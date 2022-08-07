package org.omg.CosTransactions;

public class CoordinatorPOATie
    extends org.omg.CosTransactions.CoordinatorPOA
{
        private org.omg.CosTransactions.CoordinatorOperations _delegate;
        private org.omg.PortableServer.POA _poa;

        public CoordinatorPOATie(
            org.omg.CosTransactions.CoordinatorOperations delegate
        )
        {
            _delegate = delegate;
        }

        public CoordinatorPOATie(
            org.omg.CosTransactions.CoordinatorOperations delegate,
            org.omg.PortableServer.POA poa
        )
        {
            _delegate = delegate;
            _poa = poa;
        }

        public org.omg.CosTransactions.CoordinatorOperations _delegate()
        {
            return _delegate;
        }

        public void _delegate(
            org.omg.CosTransactions.CoordinatorOperations delegate
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

        public org.omg.CosTransactions.Status get_status()
        {
            return _delegate.get_status();
        }

        public org.omg.CosTransactions.Status get_parent_status()
        {
            return _delegate.get_parent_status();
        }

        public org.omg.CosTransactions.Status get_top_level_status()
        {
            return _delegate.get_top_level_status();
        }

        public boolean is_same_transaction(
            org.omg.CosTransactions.Coordinator tc
        )
        {
            return _delegate.is_same_transaction(tc);
        }

        public boolean is_related_transaction(
            org.omg.CosTransactions.Coordinator tc
        )
        {
            return _delegate.is_related_transaction(tc);
        }

        public boolean is_ancestor_transaction(
            org.omg.CosTransactions.Coordinator tc
        )
        {
            return _delegate.is_ancestor_transaction(tc);
        }

        public boolean is_descendant_transaction(
            org.omg.CosTransactions.Coordinator tc
        )
        {
            return _delegate.is_descendant_transaction(tc);
        }

        public boolean is_top_level_transaction()
        {
            return _delegate.is_top_level_transaction();
        }

        public int hash_transaction()
        {
            return _delegate.hash_transaction();
        }

        public int hash_top_level_tran()
        {
            return _delegate.hash_top_level_tran();
        }

        public org.omg.CosTransactions.RecoveryCoordinator register_resource(
            org.omg.CosTransactions.Resource r
        ) throws org.omg.CosTransactions.Inactive
        {
            return _delegate.register_resource(r);
        }

        public void register_synchronization(
            org.omg.CosTransactions.Synchronization sync
        ) throws org.omg.CosTransactions.Inactive, org.omg.CosTransactions.SynchronizationUnavailable
        {
            _delegate.register_synchronization(sync);
        }

        public void register_subtran_aware(
            org.omg.CosTransactions.SubtransactionAwareResource r
        ) throws org.omg.CosTransactions.Inactive, org.omg.CosTransactions.NotSubtransaction
        {
            _delegate.register_subtran_aware(r);
        }

        public void rollback_only() throws org.omg.CosTransactions.Inactive
        {
            _delegate.rollback_only();
        }

        public java.lang.String get_transaction_name()
        {
            return _delegate.get_transaction_name();
        }

        public org.omg.CosTransactions.Control create_subtransaction() throws org.omg.CosTransactions.SubtransactionsUnavailable, org.omg.CosTransactions.Inactive
        {
            return _delegate.create_subtransaction();
        }

        public org.omg.CosTransactions.PropagationContext get_txcontext() throws org.omg.CosTransactions.Unavailable
        {
            return _delegate.get_txcontext();
        }

}
