package org.omg.CosTransactions;

public class TransactionFactoryPOATie
    extends org.omg.CosTransactions.TransactionFactoryPOA
{
        private org.omg.CosTransactions.TransactionFactoryOperations _delegate;
        private org.omg.PortableServer.POA _poa;

        public TransactionFactoryPOATie(
            org.omg.CosTransactions.TransactionFactoryOperations delegate
        )
        {
            _delegate = delegate;
        }

        public TransactionFactoryPOATie(
            org.omg.CosTransactions.TransactionFactoryOperations delegate,
            org.omg.PortableServer.POA poa
        )
        {
            _delegate = delegate;
            _poa = poa;
        }

        public org.omg.CosTransactions.TransactionFactoryOperations _delegate()
        {
            return _delegate;
        }

        public void _delegate(
            org.omg.CosTransactions.TransactionFactoryOperations delegate
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

}
