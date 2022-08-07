package org.omg.CosTransactions;

public class TerminatorPOATie
    extends org.omg.CosTransactions.TerminatorPOA
{
        private org.omg.CosTransactions.TerminatorOperations _delegate;
        private org.omg.PortableServer.POA _poa;

        public TerminatorPOATie(
            org.omg.CosTransactions.TerminatorOperations delegate
        )
        {
            _delegate = delegate;
        }

        public TerminatorPOATie(
            org.omg.CosTransactions.TerminatorOperations delegate,
            org.omg.PortableServer.POA poa
        )
        {
            _delegate = delegate;
            _poa = poa;
        }

        public org.omg.CosTransactions.TerminatorOperations _delegate()
        {
            return _delegate;
        }

        public void _delegate(
            org.omg.CosTransactions.TerminatorOperations delegate
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

        public void commit(
            boolean report_heuristics
        ) throws org.omg.CosTransactions.HeuristicMixed, org.omg.CosTransactions.HeuristicHazard
        {
            _delegate.commit(report_heuristics);
        }

        public void rollback()
        {
            _delegate.rollback();
        }

}
