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
// IDL:/TestModule/Hammer:1.0
//
public interface HammerOperations
{

    //
    // IDL:/TestModule/Hammer/incr:1.0
    //
    public boolean
    incr(int value,
         org.omg.CosTransactions.Control c);

    //
    // IDL:/TestModule/Hammer/set:1.0
    //
    public boolean
    set(int value,
        org.omg.CosTransactions.Control c);

    //
    // IDL:/TestModule/Hammer/get:1.0
    //
    public boolean
    get(org.omg.CORBA.IntHolder value,
        org.omg.CosTransactions.Control c);
}
