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
// IDL:/TestModule/ExplicitStack:1.0
//
public class _ExplicitStackImplBase_tie extends _ExplicitStackImplBase
{
    private ExplicitStackOperations delegate_;

    public
    _ExplicitStackImplBase_tie(ExplicitStackOperations op)
    {
        delegate_ = op;
    }

    public ExplicitStackOperations
    _delegate()
    {
        return delegate_;
    }

    public void
    _delegate(ExplicitStackOperations op)
    {
        delegate_ = op;
    }

    //
    // IDL:/TestModule/ExplicitStack/push:1.0
    //
    public int
    push(int _ob_a0,
         org.omg.CosTransactions.Control _ob_a1)
    {
        return delegate_.push(_ob_a0,
                              _ob_a1);
    }

    //
    // IDL:/TestModule/ExplicitStack/pop:1.0
    //
    public int
    pop(org.omg.CORBA.IntHolder _ob_ah0,
        org.omg.CosTransactions.Control _ob_a1)
    {
        return delegate_.pop(_ob_ah0,
                             _ob_a1);
    }

    //
    // IDL:/TestModule/ExplicitStack/printStack:1.0
    //
    public void
    printStack()
    {
        delegate_.printStack();
    }
}
