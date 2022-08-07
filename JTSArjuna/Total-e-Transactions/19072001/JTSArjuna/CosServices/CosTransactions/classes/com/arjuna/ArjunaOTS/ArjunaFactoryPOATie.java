package com.arjuna.ArjunaOTS;

public class ArjunaFactoryPOATie
    extends com.arjuna.ArjunaOTS.ArjunaFactoryPOA
{
        private com.arjuna.ArjunaOTS.ArjunaFactoryOperations _delegate;
        private org.omg.PortableServer.POA _poa;

        public ArjunaFactoryPOATie(
            com.arjuna.ArjunaOTS.ArjunaFactoryOperations delegate
        )
        {
            _delegate = delegate;
        }

        public ArjunaFactoryPOATie(
            com.arjuna.ArjunaOTS.ArjunaFactoryOperations delegate,
            org.omg.PortableServer.POA poa
        )
        {
            _delegate = delegate;
            _poa = poa;
        }

        public com.arjuna.ArjunaOTS.ArjunaFactoryOperations _delegate()
        {
            return _delegate;
        }

        public void _delegate(
            com.arjuna.ArjunaOTS.ArjunaFactoryOperations delegate
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

        public org.omg.CosTransactions.Control create(
            int time_out
        )
        {
            return _delegate.create(time_out);
        }

        public org.omg.CosTransactions.Control recreate(
            org.omg.CosTransactions.PropagationContext ctx
        )
        {
            return _delegate.recreate(ctx);
        }

        public org.omg.CosTransactions.otid_t[] numberOfTransactions(
            com.arjuna.ArjunaOTS.TransactionType t
        ) throws org.omg.CosTransactions.Inactive, org.omg.CosTransactions.NoTransaction
        {
            return _delegate.numberOfTransactions(t);
        }

        public org.omg.CosTransactions.otid_t[] getChildTransactions(
            org.omg.CosTransactions.otid_t parent
        ) throws org.omg.CosTransactions.Inactive, org.omg.CosTransactions.NoTransaction
        {
            return _delegate.getChildTransactions(parent);
        }

        public org.omg.CosTransactions.Status getCurrentStatus(
            org.omg.CosTransactions.otid_t uid
        ) throws org.omg.CosTransactions.NoTransaction
        {
            return _delegate.getCurrentStatus(uid);
        }

        public org.omg.CosTransactions.Status getStatus(
            org.omg.CosTransactions.otid_t uid
        ) throws org.omg.CosTransactions.NoTransaction
        {
            return _delegate.getStatus(uid);
        }

        public com.arjuna.ArjunaOTS.GlobalTransactionInfo getGlobalInfo()
        {
            return _delegate.getGlobalInfo();
        }

        public com.arjuna.ArjunaOTS.TransactionInfo getTransactionInfo(
            org.omg.CosTransactions.otid_t uid
        ) throws org.omg.CosTransactions.NoTransaction
        {
            return _delegate.getTransactionInfo(uid);
        }

        public org.omg.CosTransactions.Control getTransaction(
            org.omg.CosTransactions.otid_t uid
        ) throws org.omg.CosTransactions.NoTransaction
        {
            return _delegate.getTransaction(uid);
        }

}
