//
// Java code automatically generated by the Orbix 2000 IDL Compiler
// Do not edit this code!
//

package com.arjuna.ArjunaOTS;

public class _ArjunaSubtranAwareResourceStub
    extends org.omg.CORBA.portable.ObjectImpl
    implements ArjunaSubtranAwareResource, org.omg.CosTransactions.SubtransactionAwareResource
{
    final public static java.lang.Class _opsClass = ArjunaSubtranAwareResourceOperations.class;


    public _ArjunaSubtranAwareResourceStub () {}

    public org.omg.CosTransactions.Vote prepare() throws org.omg.CosTransactions.HeuristicMixed, org.omg.CosTransactions.HeuristicHazard
    {
        while (true) {
            if(!_is_local()) {
                org.omg.CORBA.portable.OutputStream _output = null;
                org.omg.CORBA.portable.InputStream _input = null;
                org.omg.CosTransactions.Vote _result;
                try {
                    _output = _request("prepare", true);
                    _input = _invoke(_output);
                    _result = org.omg.CosTransactions.VoteHelper.read(_input);
                    return _result;
                }
                catch (org.omg.CORBA.portable.RemarshalException _exception) {
                    continue;
                }
                catch (org.omg.CORBA.portable.ApplicationException _exception)
                {
                    final java.lang.String _exception_id = _exception.getId();
                        if (_exception_id.equals(org.omg.CosTransactions.HeuristicMixedHelper.id())) {
                            throw org.omg.CosTransactions.HeuristicMixedHelper.read(_exception.getInputStream());
                        }
                        if (_exception_id.equals(org.omg.CosTransactions.HeuristicHazardHelper.id())) {
                            throw org.omg.CosTransactions.HeuristicHazardHelper.read(_exception.getInputStream());
                        }
                    throw new org.omg.CORBA.UNKNOWN("Unexpected User Exception;" + _exception_id);
                }
                finally {
                    _releaseReply(_input);
                }
            }
            else {
                org.omg.CORBA.portable.ServantObject _so = _servant_preinvoke("prepare", _opsClass);
                if (_so == null) {
                    continue;
                }
                ArjunaSubtranAwareResourceOperations _self = (ArjunaSubtranAwareResourceOperations)_so.servant;
                try {
                    return _self.prepare();
                }
                finally {
                    _servant_postinvoke(_so);
                }
            }
        }
    }


    public void rollback() throws org.omg.CosTransactions.HeuristicCommit, org.omg.CosTransactions.HeuristicMixed, org.omg.CosTransactions.HeuristicHazard
    {
        while (true) {
            if(!_is_local()) {
                org.omg.CORBA.portable.OutputStream _output = null;
                org.omg.CORBA.portable.InputStream _input = null;
                try {
                    _output = _request("rollback", true);
                    _input = _invoke(_output);
                    return;
                }
                catch (org.omg.CORBA.portable.RemarshalException _exception) {
                    continue;
                }
                catch (org.omg.CORBA.portable.ApplicationException _exception)
                {
                    final java.lang.String _exception_id = _exception.getId();
                        if (_exception_id.equals(org.omg.CosTransactions.HeuristicCommitHelper.id())) {
                            throw org.omg.CosTransactions.HeuristicCommitHelper.read(_exception.getInputStream());
                        }
                        if (_exception_id.equals(org.omg.CosTransactions.HeuristicMixedHelper.id())) {
                            throw org.omg.CosTransactions.HeuristicMixedHelper.read(_exception.getInputStream());
                        }
                        if (_exception_id.equals(org.omg.CosTransactions.HeuristicHazardHelper.id())) {
                            throw org.omg.CosTransactions.HeuristicHazardHelper.read(_exception.getInputStream());
                        }
                    throw new org.omg.CORBA.UNKNOWN("Unexpected User Exception;" + _exception_id);
                }
                finally {
                    _releaseReply(_input);
                }
            }
            else {
                org.omg.CORBA.portable.ServantObject _so = _servant_preinvoke("rollback", _opsClass);
                if (_so == null) {
                    continue;
                }
                ArjunaSubtranAwareResourceOperations _self = (ArjunaSubtranAwareResourceOperations)_so.servant;
                try {
                    _self.rollback();
                    return;
                }
                finally {
                    _servant_postinvoke(_so);
                }
            }
        }
    }


    public void commit() throws org.omg.CosTransactions.NotPrepared, org.omg.CosTransactions.HeuristicRollback, org.omg.CosTransactions.HeuristicMixed, org.omg.CosTransactions.HeuristicHazard
    {
        while (true) {
            if(!_is_local()) {
                org.omg.CORBA.portable.OutputStream _output = null;
                org.omg.CORBA.portable.InputStream _input = null;
                try {
                    _output = _request("commit", true);
                    _input = _invoke(_output);
                    return;
                }
                catch (org.omg.CORBA.portable.RemarshalException _exception) {
                    continue;
                }
                catch (org.omg.CORBA.portable.ApplicationException _exception)
                {
                    final java.lang.String _exception_id = _exception.getId();
                        if (_exception_id.equals(org.omg.CosTransactions.NotPreparedHelper.id())) {
                            throw org.omg.CosTransactions.NotPreparedHelper.read(_exception.getInputStream());
                        }
                        if (_exception_id.equals(org.omg.CosTransactions.HeuristicRollbackHelper.id())) {
                            throw org.omg.CosTransactions.HeuristicRollbackHelper.read(_exception.getInputStream());
                        }
                        if (_exception_id.equals(org.omg.CosTransactions.HeuristicMixedHelper.id())) {
                            throw org.omg.CosTransactions.HeuristicMixedHelper.read(_exception.getInputStream());
                        }
                        if (_exception_id.equals(org.omg.CosTransactions.HeuristicHazardHelper.id())) {
                            throw org.omg.CosTransactions.HeuristicHazardHelper.read(_exception.getInputStream());
                        }
                    throw new org.omg.CORBA.UNKNOWN("Unexpected User Exception;" + _exception_id);
                }
                finally {
                    _releaseReply(_input);
                }
            }
            else {
                org.omg.CORBA.portable.ServantObject _so = _servant_preinvoke("commit", _opsClass);
                if (_so == null) {
                    continue;
                }
                ArjunaSubtranAwareResourceOperations _self = (ArjunaSubtranAwareResourceOperations)_so.servant;
                try {
                    _self.commit();
                    return;
                }
                finally {
                    _servant_postinvoke(_so);
                }
            }
        }
    }


    public void commit_one_phase() throws org.omg.CosTransactions.HeuristicHazard
    {
        while (true) {
            if(!_is_local()) {
                org.omg.CORBA.portable.OutputStream _output = null;
                org.omg.CORBA.portable.InputStream _input = null;
                try {
                    _output = _request("commit_one_phase", true);
                    _input = _invoke(_output);
                    return;
                }
                catch (org.omg.CORBA.portable.RemarshalException _exception) {
                    continue;
                }
                catch (org.omg.CORBA.portable.ApplicationException _exception)
                {
                    final java.lang.String _exception_id = _exception.getId();
                        if (_exception_id.equals(org.omg.CosTransactions.HeuristicHazardHelper.id())) {
                            throw org.omg.CosTransactions.HeuristicHazardHelper.read(_exception.getInputStream());
                        }
                    throw new org.omg.CORBA.UNKNOWN("Unexpected User Exception;" + _exception_id);
                }
                finally {
                    _releaseReply(_input);
                }
            }
            else {
                org.omg.CORBA.portable.ServantObject _so = _servant_preinvoke("commit_one_phase", _opsClass);
                if (_so == null) {
                    continue;
                }
                ArjunaSubtranAwareResourceOperations _self = (ArjunaSubtranAwareResourceOperations)_so.servant;
                try {
                    _self.commit_one_phase();
                    return;
                }
                finally {
                    _servant_postinvoke(_so);
                }
            }
        }
    }


    public void forget()
    {
        while (true) {
            if(!_is_local()) {
                org.omg.CORBA.portable.OutputStream _output = null;
                org.omg.CORBA.portable.InputStream _input = null;
                try {
                    _output = _request("forget", true);
                    _input = _invoke(_output);
                    return;
                }
                catch (org.omg.CORBA.portable.RemarshalException _exception) {
                    continue;
                }
                catch (org.omg.CORBA.portable.ApplicationException _exception)
                {
                    final java.lang.String _exception_id = _exception.getId();
                    throw new org.omg.CORBA.UNKNOWN("Unexpected User Exception;" + _exception_id);
                }
                finally {
                    _releaseReply(_input);
                }
            }
            else {
                org.omg.CORBA.portable.ServantObject _so = _servant_preinvoke("forget", _opsClass);
                if (_so == null) {
                    continue;
                }
                ArjunaSubtranAwareResourceOperations _self = (ArjunaSubtranAwareResourceOperations)_so.servant;
                try {
                    _self.forget();
                    return;
                }
                finally {
                    _servant_postinvoke(_so);
                }
            }
        }
    }


    public void commit_subtransaction(
        org.omg.CosTransactions.Coordinator parent
    )
    {
        while (true) {
            if(!_is_local()) {
                org.omg.CORBA.portable.OutputStream _output = null;
                org.omg.CORBA.portable.InputStream _input = null;
                try {
                    _output = _request("commit_subtransaction", true);
                    org.omg.CosTransactions.CoordinatorHelper.write(_output, parent);
                    _input = _invoke(_output);
                    return;
                }
                catch (org.omg.CORBA.portable.RemarshalException _exception) {
                    continue;
                }
                catch (org.omg.CORBA.portable.ApplicationException _exception)
                {
                    final java.lang.String _exception_id = _exception.getId();
                    throw new org.omg.CORBA.UNKNOWN("Unexpected User Exception;" + _exception_id);
                }
                finally {
                    _releaseReply(_input);
                }
            }
            else {
                org.omg.CORBA.portable.ServantObject _so = _servant_preinvoke("commit_subtransaction", _opsClass);
                if (_so == null) {
                    continue;
                }
                ArjunaSubtranAwareResourceOperations _self = (ArjunaSubtranAwareResourceOperations)_so.servant;
                try {
                    _self.commit_subtransaction(parent);
                    return;
                }
                finally {
                    _servant_postinvoke(_so);
                }
            }
        }
    }


    public void rollback_subtransaction()
    {
        while (true) {
            if(!_is_local()) {
                org.omg.CORBA.portable.OutputStream _output = null;
                org.omg.CORBA.portable.InputStream _input = null;
                try {
                    _output = _request("rollback_subtransaction", true);
                    _input = _invoke(_output);
                    return;
                }
                catch (org.omg.CORBA.portable.RemarshalException _exception) {
                    continue;
                }
                catch (org.omg.CORBA.portable.ApplicationException _exception)
                {
                    final java.lang.String _exception_id = _exception.getId();
                    throw new org.omg.CORBA.UNKNOWN("Unexpected User Exception;" + _exception_id);
                }
                finally {
                    _releaseReply(_input);
                }
            }
            else {
                org.omg.CORBA.portable.ServantObject _so = _servant_preinvoke("rollback_subtransaction", _opsClass);
                if (_so == null) {
                    continue;
                }
                ArjunaSubtranAwareResourceOperations _self = (ArjunaSubtranAwareResourceOperations)_so.servant;
                try {
                    _self.rollback_subtransaction();
                    return;
                }
                finally {
                    _servant_postinvoke(_so);
                }
            }
        }
    }


    public org.omg.CosTransactions.Vote prepare_subtransaction()
    {
        while (true) {
            if(!_is_local()) {
                org.omg.CORBA.portable.OutputStream _output = null;
                org.omg.CORBA.portable.InputStream _input = null;
                org.omg.CosTransactions.Vote _result;
                try {
                    _output = _request("prepare_subtransaction", true);
                    _input = _invoke(_output);
                    _result = org.omg.CosTransactions.VoteHelper.read(_input);
                    return _result;
                }
                catch (org.omg.CORBA.portable.RemarshalException _exception) {
                    continue;
                }
                catch (org.omg.CORBA.portable.ApplicationException _exception)
                {
                    final java.lang.String _exception_id = _exception.getId();
                    throw new org.omg.CORBA.UNKNOWN("Unexpected User Exception;" + _exception_id);
                }
                finally {
                    _releaseReply(_input);
                }
            }
            else {
                org.omg.CORBA.portable.ServantObject _so = _servant_preinvoke("prepare_subtransaction", _opsClass);
                if (_so == null) {
                    continue;
                }
                ArjunaSubtranAwareResourceOperations _self = (ArjunaSubtranAwareResourceOperations)_so.servant;
                try {
                    return _self.prepare_subtransaction();
                }
                finally {
                    _servant_postinvoke(_so);
                }
            }
        }
    }


    public static final String _interfaces[] =
    {"IDL:omg.org/CosTransactions/Resource:1.0", "IDL:omg.org/CosTransactions/SubtransactionAwareResource:1.0", "IDL:arjuna.com/ArjunaOTS/ArjunaSubtranAwareResource:1.0"};

    public String[] _ids()
    {
        return _interfaces;
    }

}
