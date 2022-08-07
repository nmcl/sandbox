package org.omg.CosTransactions;

public class TransactionalObjectPOATie
    extends org.omg.CosTransactions.TransactionalObjectPOA
{
        private org.omg.CosTransactions.TransactionalObjectOperations _delegate;
        private org.omg.PortableServer.POA _poa;

        public TransactionalObjectPOATie(
            org.omg.CosTransactions.TransactionalObjectOperations delegate
        )
        {
            _delegate = delegate;
        }

        public TransactionalObjectPOATie(
            org.omg.CosTransactions.TransactionalObjectOperations delegate,
            org.omg.PortableServer.POA poa
        )
        {
            _delegate = delegate;
            _poa = poa;
        }

        public org.omg.CosTransactions.TransactionalObjectOperations _delegate()
        {
            return _delegate;
        }

        public void _delegate(
            org.omg.CosTransactions.TransactionalObjectOperations delegate
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

}
