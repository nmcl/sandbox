package com.arjuna.CosRecovery.Admin;

public class RecoveryManagerPOATie
    extends com.arjuna.CosRecovery.Admin.RecoveryManagerPOA
{
        private com.arjuna.CosRecovery.Admin.RecoveryManagerOperations _delegate;
        private org.omg.PortableServer.POA _poa;

        public RecoveryManagerPOATie(
            com.arjuna.CosRecovery.Admin.RecoveryManagerOperations delegate
        )
        {
            _delegate = delegate;
        }

        public RecoveryManagerPOATie(
            com.arjuna.CosRecovery.Admin.RecoveryManagerOperations delegate,
            org.omg.PortableServer.POA poa
        )
        {
            _delegate = delegate;
            _poa = poa;
        }

        public com.arjuna.CosRecovery.Admin.RecoveryManagerOperations _delegate()
        {
            return _delegate;
        }

        public void _delegate(
            com.arjuna.CosRecovery.Admin.RecoveryManagerOperations delegate
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

        public java.lang.String createIOR(
            java.lang.String id
        )
        {
            return _delegate.createIOR(id);
        }

}
