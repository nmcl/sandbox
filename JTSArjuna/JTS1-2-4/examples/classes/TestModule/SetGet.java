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
public interface SetGet extends org.omg.CosTransactions.SubtransactionAwareResource
{

    //
    // IDL:/TestModule/SetGet/set:1.0
    //
    public void
    set(short n,
        org.omg.CosTransactions.Control cont);

    //
    // IDL:/TestModule/SetGet/get:1.0
    //
    public short
    get(org.omg.CosTransactions.Control cont);
}
