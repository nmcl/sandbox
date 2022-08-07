package org.omg.CosTransactions;

public class ControlPOATie
    extends org.omg.CosTransactions.ControlPOA
{
        private org.omg.CosTransactions.ControlOperations _delegate;
        private org.omg.PortableServer.POA _poa;

        public ControlPOATie(
            org.omg.CosTransactions.ControlOperations delegate
        )
        {
            _delegate = delegate;
        }

        public ControlPOATie(
            org.omg.CosTransactions.ControlOperations delegate,
            org.omg.PortableServer.POA poa
        )
        {
            _delegate = delegate;
            _poa = poa;
        }

        public org.omg.CosTransactions.ControlOperations _delegate()
        {
            return _delegate;
        }

        public void _delegate(
            org.omg.CosTransactions.ControlOperations delegate
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

}
