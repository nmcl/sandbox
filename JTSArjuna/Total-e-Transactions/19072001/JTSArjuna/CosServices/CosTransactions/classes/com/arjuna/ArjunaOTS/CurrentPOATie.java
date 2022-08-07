package com.arjuna.ArjunaOTS;

public class CurrentPOATie
    extends com.arjuna.ArjunaOTS.CurrentPOA
{
        private com.arjuna.ArjunaOTS.CurrentOperations _delegate;
        private org.omg.PortableServer.POA _poa;

        public CurrentPOATie(
            com.arjuna.ArjunaOTS.CurrentOperations delegate
        )
        {
            _delegate = delegate;
        }

        public CurrentPOATie(
            com.arjuna.ArjunaOTS.CurrentOperations delegate,
            org.omg.PortableServer.POA poa
        )
        {
            _delegate = delegate;
            _poa = poa;
        }

        public com.arjuna.ArjunaOTS.CurrentOperations _delegate()
        {
            return _delegate;
        }

        public void _delegate(
            com.arjuna.ArjunaOTS.CurrentOperations delegate
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

        public void begin() throws org.omg.CosTransactions.SubtransactionsUnavailable
        {
            _delegate.begin();
        }

        public void commit(
            boolean report_heuristics
        ) throws org.omg.CosTransactions.NoTransaction, org.omg.CosTransactions.HeuristicMixed, org.omg.CosTransactions.HeuristicHazard
        {
            _delegate.commit(report_heuristics);
        }

        public void rollback() throws org.omg.CosTransactions.NoTransaction
        {
            _delegate.rollback();
        }

        public void rollback_only() throws org.omg.CosTransactions.NoTransaction
        {
            _delegate.rollback_only();
        }

        public org.omg.CosTransactions.Status get_status()
        {
            return _delegate.get_status();
        }

        public java.lang.String get_transaction_name()
        {
            return _delegate.get_transaction_name();
        }

        public void set_timeout(
            int seconds
        )
        {
            _delegate.set_timeout(seconds);
        }

        public org.omg.CosTransactions.Control get_control()
        {
            return _delegate.get_control();
        }

        public org.omg.CosTransactions.Control suspend()
        {
            return _delegate.suspend();
        }

        public void resume(
            org.omg.CosTransactions.Control which
        ) throws org.omg.CosTransactions.InvalidControl
        {
            _delegate.resume(which);
        }

        public int get_timeout()
        {
            return _delegate.get_timeout();
        }

}
