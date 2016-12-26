// **********************************************************************
//
// Generated by the ORBacus IDL to Java Translator
//
// Copyright (c) 1999
// Object-Oriented Concepts, Inc.
// Billerica, MA, USA
//
// Copyright (c) 1999
// Object-Oriented Concepts GmbH
// Ettlingen, Germany
//
// All Rights Reserved
//
// **********************************************************************

// Version: 3.1.3
// License: non-commercial

package TestModule;

//
// IDL:/TestModule/SetGet:1.0
//
public class StubForSetGet extends org.omg.CORBA.portable.ObjectImpl
                           implements SetGet
{
    static final String[] _ob_ids_ =
    {
        "IDL:/TestModule/SetGet:1.0",
        "IDL:omg.org/CosTransactions/SubtransactionAwareResource:1.0",
        "IDL:omg.org/CosTransactions/Resource:1.0"
    };

    public String[]
    _ids()
    {
        return _ob_ids_;
    }

    //
    // IDL:/TestModule/SetGet/set:1.0
    //
    public void
    set(short _ob_a0,
        org.omg.CosTransactions.Control _ob_a1)
    {
        org.omg.CORBA.Request _ob_req = _request("set");

        org.omg.CORBA.Any _ob_aa0 = _ob_req.add_in_arg();
        _ob_aa0.insert_short(_ob_a0);
        org.omg.CORBA.Any _ob_aa1 = _ob_req.add_in_arg();
        org.omg.CosTransactions.ControlHelper.insert(_ob_aa1, _ob_a1);

        _ob_req.invoke();

        java.lang.Exception _ob_ex = _ob_req.env().exception();
        if(_ob_ex != null)
            throw (org.omg.CORBA.SystemException)_ob_ex;
    }

    //
    // IDL:/TestModule/SetGet/get:1.0
    //
    public short
    get(org.omg.CosTransactions.Control _ob_a0)
    {
        org.omg.CORBA.Request _ob_req = _request("get");

        org.omg.CORBA.Any _ob_aa0 = _ob_req.add_in_arg();
        org.omg.CosTransactions.ControlHelper.insert(_ob_aa0, _ob_a0);

        _ob_req.set_return_type(_orb().get_primitive_tc(org.omg.CORBA.TCKind.tk_short));

        _ob_req.invoke();

        java.lang.Exception _ob_ex = _ob_req.env().exception();
        if(_ob_ex != null)
            throw (org.omg.CORBA.SystemException)_ob_ex;

        org.omg.CORBA.Any _ob_ra = _ob_req.return_value();
        return _ob_ra.extract_short();
    }

    //
    // IDL:omg.org/CosTransactions/SubtransactionAwareResource/commit_subtransaction:1.0
    //
    public void
    commit_subtransaction(org.omg.CosTransactions.Coordinator _ob_a0)
    {
        org.omg.CORBA.Request _ob_req = _request("commit_subtransaction");

        org.omg.CORBA.Any _ob_aa0 = _ob_req.add_in_arg();
        org.omg.CosTransactions.CoordinatorHelper.insert(_ob_aa0, _ob_a0);

        _ob_req.invoke();

        java.lang.Exception _ob_ex = _ob_req.env().exception();
        if(_ob_ex != null)
            throw (org.omg.CORBA.SystemException)_ob_ex;
    }

    //
    // IDL:omg.org/CosTransactions/SubtransactionAwareResource/rollback_subtransaction:1.0
    //
    public void
    rollback_subtransaction()
    {
        org.omg.CORBA.Request _ob_req = _request("rollback_subtransaction");

        _ob_req.invoke();

        java.lang.Exception _ob_ex = _ob_req.env().exception();
        if(_ob_ex != null)
            throw (org.omg.CORBA.SystemException)_ob_ex;
    }

    //
    // IDL:omg.org/CosTransactions/Resource/prepare:1.0
    //
    public org.omg.CosTransactions.Vote
    prepare()
        throws org.omg.CosTransactions.HeuristicMixed,
               org.omg.CosTransactions.HeuristicHazard
    {
        org.omg.CORBA.Request _ob_req = _request("prepare");

        _ob_req.exceptions().add(org.omg.CosTransactions.HeuristicMixedHelper.type());
        _ob_req.exceptions().add(org.omg.CosTransactions.HeuristicHazardHelper.type());

        _ob_req.set_return_type(org.omg.CosTransactions.VoteHelper.type());

        _ob_req.invoke();

        java.lang.Exception _ob_ex = _ob_req.env().exception();
        if(_ob_ex != null)
        {
            org.omg.CORBA.UnknownUserException _ob_uex;
            try
            {
                _ob_uex = (org.omg.CORBA.UnknownUserException)_ob_ex;
            }
            catch(ClassCastException _ob_dummy)
            {
                throw (org.omg.CORBA.SystemException)_ob_ex;
            }

            try
            {
                throw org.omg.CosTransactions.HeuristicMixedHelper.extract(_ob_uex.except);
            }
            catch(org.omg.CORBA.BAD_OPERATION _ob_dummy)
            {
            }
            try
            {
                throw org.omg.CosTransactions.HeuristicHazardHelper.extract(_ob_uex.except);
            }
            catch(org.omg.CORBA.BAD_OPERATION _ob_dummy)
            {
            }

            throw new org.omg.CORBA.UNKNOWN();
        }

        org.omg.CORBA.Any _ob_ra = _ob_req.return_value();
        return org.omg.CosTransactions.VoteHelper.extract(_ob_ra);
    }

    //
    // IDL:omg.org/CosTransactions/Resource/rollback:1.0
    //
    public void
    rollback()
        throws org.omg.CosTransactions.HeuristicCommit,
               org.omg.CosTransactions.HeuristicMixed,
               org.omg.CosTransactions.HeuristicHazard
    {
        org.omg.CORBA.Request _ob_req = _request("rollback");

        _ob_req.exceptions().add(org.omg.CosTransactions.HeuristicCommitHelper.type());
        _ob_req.exceptions().add(org.omg.CosTransactions.HeuristicMixedHelper.type());
        _ob_req.exceptions().add(org.omg.CosTransactions.HeuristicHazardHelper.type());

        _ob_req.invoke();

        java.lang.Exception _ob_ex = _ob_req.env().exception();
        if(_ob_ex != null)
        {
            org.omg.CORBA.UnknownUserException _ob_uex;
            try
            {
                _ob_uex = (org.omg.CORBA.UnknownUserException)_ob_ex;
            }
            catch(ClassCastException _ob_dummy)
            {
                throw (org.omg.CORBA.SystemException)_ob_ex;
            }

            try
            {
                throw org.omg.CosTransactions.HeuristicCommitHelper.extract(_ob_uex.except);
            }
            catch(org.omg.CORBA.BAD_OPERATION _ob_dummy)
            {
            }
            try
            {
                throw org.omg.CosTransactions.HeuristicMixedHelper.extract(_ob_uex.except);
            }
            catch(org.omg.CORBA.BAD_OPERATION _ob_dummy)
            {
            }
            try
            {
                throw org.omg.CosTransactions.HeuristicHazardHelper.extract(_ob_uex.except);
            }
            catch(org.omg.CORBA.BAD_OPERATION _ob_dummy)
            {
            }

            throw new org.omg.CORBA.UNKNOWN();
        }
    }

    //
    // IDL:omg.org/CosTransactions/Resource/commit:1.0
    //
    public void
    commit()
        throws org.omg.CosTransactions.NotPrepared,
               org.omg.CosTransactions.HeuristicRollback,
               org.omg.CosTransactions.HeuristicMixed,
               org.omg.CosTransactions.HeuristicHazard
    {
        org.omg.CORBA.Request _ob_req = _request("commit");

        _ob_req.exceptions().add(org.omg.CosTransactions.NotPreparedHelper.type());
        _ob_req.exceptions().add(org.omg.CosTransactions.HeuristicRollbackHelper.type());
        _ob_req.exceptions().add(org.omg.CosTransactions.HeuristicMixedHelper.type());
        _ob_req.exceptions().add(org.omg.CosTransactions.HeuristicHazardHelper.type());

        _ob_req.invoke();

        java.lang.Exception _ob_ex = _ob_req.env().exception();
        if(_ob_ex != null)
        {
            org.omg.CORBA.UnknownUserException _ob_uex;
            try
            {
                _ob_uex = (org.omg.CORBA.UnknownUserException)_ob_ex;
            }
            catch(ClassCastException _ob_dummy)
            {
                throw (org.omg.CORBA.SystemException)_ob_ex;
            }

            try
            {
                throw org.omg.CosTransactions.NotPreparedHelper.extract(_ob_uex.except);
            }
            catch(org.omg.CORBA.BAD_OPERATION _ob_dummy)
            {
            }
            try
            {
                throw org.omg.CosTransactions.HeuristicRollbackHelper.extract(_ob_uex.except);
            }
            catch(org.omg.CORBA.BAD_OPERATION _ob_dummy)
            {
            }
            try
            {
                throw org.omg.CosTransactions.HeuristicMixedHelper.extract(_ob_uex.except);
            }
            catch(org.omg.CORBA.BAD_OPERATION _ob_dummy)
            {
            }
            try
            {
                throw org.omg.CosTransactions.HeuristicHazardHelper.extract(_ob_uex.except);
            }
            catch(org.omg.CORBA.BAD_OPERATION _ob_dummy)
            {
            }

            throw new org.omg.CORBA.UNKNOWN();
        }
    }

    //
    // IDL:omg.org/CosTransactions/Resource/commit_one_phase:1.0
    //
    public void
    commit_one_phase()
        throws org.omg.CosTransactions.HeuristicHazard
    {
        org.omg.CORBA.Request _ob_req = _request("commit_one_phase");

        _ob_req.exceptions().add(org.omg.CosTransactions.HeuristicHazardHelper.type());

        _ob_req.invoke();

        java.lang.Exception _ob_ex = _ob_req.env().exception();
        if(_ob_ex != null)
        {
            org.omg.CORBA.UnknownUserException _ob_uex;
            try
            {
                _ob_uex = (org.omg.CORBA.UnknownUserException)_ob_ex;
            }
            catch(ClassCastException _ob_dummy)
            {
                throw (org.omg.CORBA.SystemException)_ob_ex;
            }

            try
            {
                throw org.omg.CosTransactions.HeuristicHazardHelper.extract(_ob_uex.except);
            }
            catch(org.omg.CORBA.BAD_OPERATION _ob_dummy)
            {
            }

            throw new org.omg.CORBA.UNKNOWN();
        }
    }

    //
    // IDL:omg.org/CosTransactions/Resource/forget:1.0
    //
    public void
    forget()
    {
        org.omg.CORBA.Request _ob_req = _request("forget");

        _ob_req.invoke();

        java.lang.Exception _ob_ex = _ob_req.env().exception();
        if(_ob_ex != null)
            throw (org.omg.CORBA.SystemException)_ob_ex;
    }
}
