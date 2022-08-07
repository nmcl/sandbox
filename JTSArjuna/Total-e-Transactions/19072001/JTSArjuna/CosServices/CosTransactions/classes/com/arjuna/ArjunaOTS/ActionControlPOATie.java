package com.arjuna.ArjunaOTS;

public class ActionControlPOATie
    extends com.arjuna.ArjunaOTS.ActionControlPOA
{
        private com.arjuna.ArjunaOTS.ActionControlOperations _delegate;
        private org.omg.PortableServer.POA _poa;

        public ActionControlPOATie(
            com.arjuna.ArjunaOTS.ActionControlOperations delegate
        )
        {
            _delegate = delegate;
        }

        public ActionControlPOATie(
            com.arjuna.ArjunaOTS.ActionControlOperations delegate,
            org.omg.PortableServer.POA poa
        )
        {
            _delegate = delegate;
            _poa = poa;
        }

        public com.arjuna.ArjunaOTS.ActionControlOperations _delegate()
        {
            return _delegate;
        }

        public void _delegate(
            com.arjuna.ArjunaOTS.ActionControlOperations delegate
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

        public org.omg.CosTransactions.Terminator get_terminator() throws org.omg.CosTransactions.Unavailable
        {
            return _delegate.get_terminator();
        }

        public org.omg.CosTransactions.Coordinator get_coordinator() throws org.omg.CosTransactions.Unavailable
        {
            return _delegate.get_coordinator();
        }

        public org.omg.CosTransactions.Control getParentControl() throws org.omg.CosTransactions.Unavailable
        {
            return _delegate.getParentControl();
        }

        public void destroy() throws com.arjuna.ArjunaOTS.ActiveTransaction, com.arjuna.ArjunaOTS.ActiveThreads, com.arjuna.ArjunaOTS.BadControl, com.arjuna.ArjunaOTS.Destroyed
        {
            _delegate.destroy();
        }

}
