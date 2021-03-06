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
// IDL:/TestModule/TranGrid:1.0
//
public class _TranGridImplBase_tie extends _TranGridImplBase
{
    private TranGridOperations delegate_;

    public
    _TranGridImplBase_tie(TranGridOperations op)
    {
        delegate_ = op;
    }

    public TranGridOperations
    _delegate()
    {
        return delegate_;
    }

    public void
    _delegate(TranGridOperations op)
    {
        delegate_ = op;
    }

    //
    // IDL:/TestModule/TranGrid/height:1.0
    //
    public short
    height()
    {
        return delegate_.height();
    }

    //
    // IDL:/TestModule/TranGrid/width:1.0
    //
    public short
    width()
    {
        return delegate_.width();
    }

    //
    // IDL:/TestModule/TranGrid/set:1.0
    //
    public void
    set(short _ob_a0,
        short _ob_a1,
        short _ob_a2)
    {
        delegate_.set(_ob_a0,
                      _ob_a1,
                      _ob_a2);
    }

    //
    // IDL:/TestModule/TranGrid/get:1.0
    //
    public short
    get(short _ob_a0,
        short _ob_a1)
    {
        return delegate_.get(_ob_a0,
                             _ob_a1);
    }

    //
    // IDL:omg.org/CosTransactions/Resource/prepare:1.0
    //
    public org.omg.CosTransactions.Vote
    prepare()
        throws org.omg.CosTransactions.HeuristicMixed,
               org.omg.CosTransactions.HeuristicHazard
    {
        return delegate_.prepare();
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
        delegate_.rollback();
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
        delegate_.commit();
    }

    //
    // IDL:omg.org/CosTransactions/Resource/commit_one_phase:1.0
    //
    public void
    commit_one_phase()
        throws org.omg.CosTransactions.HeuristicHazard
    {
        delegate_.commit_one_phase();
    }

    //
    // IDL:omg.org/CosTransactions/Resource/forget:1.0
    //
    public void
    forget()
    {
        delegate_.forget();
    }
}
