package com.arjuna.ArjunaOTS;

public class OTSAbstractRecordPOATie
    extends com.arjuna.ArjunaOTS.OTSAbstractRecordPOA
{
        private com.arjuna.ArjunaOTS.OTSAbstractRecordOperations _delegate;
        private org.omg.PortableServer.POA _poa;

        public OTSAbstractRecordPOATie(
            com.arjuna.ArjunaOTS.OTSAbstractRecordOperations delegate
        )
        {
            _delegate = delegate;
        }

        public OTSAbstractRecordPOATie(
            com.arjuna.ArjunaOTS.OTSAbstractRecordOperations delegate,
            org.omg.PortableServer.POA poa
        )
        {
            _delegate = delegate;
            _poa = poa;
        }

        public com.arjuna.ArjunaOTS.OTSAbstractRecordOperations _delegate()
        {
            return _delegate;
        }

        public void _delegate(
            com.arjuna.ArjunaOTS.OTSAbstractRecordOperations delegate
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

        public void commit_subtransaction(
            org.omg.CosTransactions.Coordinator parent
        )
        {
            _delegate.commit_subtransaction(parent);
        }

        public void rollback_subtransaction()
        {
            _delegate.rollback_subtransaction();
        }

        public org.omg.CosTransactions.Vote prepare_subtransaction()
        {
            return _delegate.prepare_subtransaction();
        }

        public int typeId()
        {
            return _delegate.typeId();
        }

        public java.lang.String uid()
        {
            return _delegate.uid();
        }

        public boolean propagateOnAbort()
        {
            return _delegate.propagateOnAbort();
        }

        public boolean propagateOnCommit()
        {
            return _delegate.propagateOnCommit();
        }

        public boolean saveRecord()
        {
            return _delegate.saveRecord();
        }

        public void merge(
            com.arjuna.ArjunaOTS.OTSAbstractRecord record
        )
        {
            _delegate.merge(record);
        }

        public void alter(
            com.arjuna.ArjunaOTS.OTSAbstractRecord record
        )
        {
            _delegate.alter(record);
        }

        public boolean shouldAdd(
            com.arjuna.ArjunaOTS.OTSAbstractRecord record
        )
        {
            return _delegate.shouldAdd(record);
        }

        public boolean shouldAlter(
            com.arjuna.ArjunaOTS.OTSAbstractRecord record
        )
        {
            return _delegate.shouldAlter(record);
        }

        public boolean shouldMerge(
            com.arjuna.ArjunaOTS.OTSAbstractRecord record
        )
        {
            return _delegate.shouldMerge(record);
        }

        public boolean shouldReplace(
            com.arjuna.ArjunaOTS.OTSAbstractRecord record
        )
        {
            return _delegate.shouldReplace(record);
        }

}
