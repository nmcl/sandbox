package org.omg.CosTransactions;

public class ResourcePOATie
    extends org.omg.CosTransactions.ResourcePOA
{
        private org.omg.CosTransactions.ResourceOperations _delegate;
        private org.omg.PortableServer.POA _poa;

        public ResourcePOATie(
            org.omg.CosTransactions.ResourceOperations delegate
        )
        {
            _delegate = delegate;
        }

        public ResourcePOATie(
            org.omg.CosTransactions.ResourceOperations delegate,
            org.omg.PortableServer.POA poa
        )
        {
            _delegate = delegate;
            _poa = poa;
        }

        public org.omg.CosTransactions.ResourceOperations _delegate()
        {
            return _delegate;
        }

        public void _delegate(
            org.omg.CosTransactions.ResourceOperations delegate
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

        public org.omg.CosTransactions.Vote prepare() throws org.omg.CosTransactions.HeuristicMixed, org.omg.CosTransactions.HeuristicHazard
        {
            return _delegate.prepare();
        }

        public void rollback() throws org.omg.CosTransactions.HeuristicCommit, org.omg.CosTransactions.HeuristicMixed, org.omg.CosTransactions.HeuristicHazard
        {
            _delegate.rollback();
        }

        public void commit() throws org.omg.CosTransactions.NotPrepared, org.omg.CosTransactions.HeuristicRollback, org.omg.CosTransactions.HeuristicMixed, org.omg.CosTransactions.HeuristicHazard
        {
            _delegate.commit();
        }

        public void commit_one_phase() throws org.omg.CosTransactions.HeuristicHazard
        {
            _delegate.commit_one_phase();
        }

        public void forget()
        {
            _delegate.forget();
        }

}
