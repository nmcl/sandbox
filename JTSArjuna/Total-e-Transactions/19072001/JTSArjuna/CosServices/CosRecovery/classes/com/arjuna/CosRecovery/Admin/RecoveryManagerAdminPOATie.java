package com.arjuna.CosRecovery.Admin;

public class RecoveryManagerAdminPOATie
    extends com.arjuna.CosRecovery.Admin.RecoveryManagerAdminPOA
{
        private com.arjuna.CosRecovery.Admin.RecoveryManagerAdminOperations _delegate;
        private org.omg.PortableServer.POA _poa;

        public RecoveryManagerAdminPOATie(
            com.arjuna.CosRecovery.Admin.RecoveryManagerAdminOperations delegate
        )
        {
            _delegate = delegate;
        }

        public RecoveryManagerAdminPOATie(
            com.arjuna.CosRecovery.Admin.RecoveryManagerAdminOperations delegate,
            org.omg.PortableServer.POA poa
        )
        {
            _delegate = delegate;
            _poa = poa;
        }

        public com.arjuna.CosRecovery.Admin.RecoveryManagerAdminOperations _delegate()
        {
            return _delegate;
        }

        public void _delegate(
            com.arjuna.CosRecovery.Admin.RecoveryManagerAdminOperations delegate
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

        public int ping()
        {
            return _delegate.ping();
        }

}
