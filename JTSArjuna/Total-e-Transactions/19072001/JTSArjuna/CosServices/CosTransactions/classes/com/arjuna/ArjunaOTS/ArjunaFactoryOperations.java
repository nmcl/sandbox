//
// Java code automatically generated by the Orbix 2000 IDL Compiler
// Do not edit this code!
//

package com.arjuna.ArjunaOTS;

public interface ArjunaFactoryOperations
extends 
    org.omg.CosTransactions.TransactionFactoryOperations
{
     org.omg.CosTransactions.otid_t[] numberOfTransactions(
        com.arjuna.ArjunaOTS.TransactionType t
    ) throws org.omg.CosTransactions.Inactive, org.omg.CosTransactions.NoTransaction;

     org.omg.CosTransactions.otid_t[] getChildTransactions(
        org.omg.CosTransactions.otid_t parent
    ) throws org.omg.CosTransactions.Inactive, org.omg.CosTransactions.NoTransaction;

     org.omg.CosTransactions.Status getCurrentStatus(
        org.omg.CosTransactions.otid_t uid
    ) throws org.omg.CosTransactions.NoTransaction;

     org.omg.CosTransactions.Status getStatus(
        org.omg.CosTransactions.otid_t uid
    ) throws org.omg.CosTransactions.NoTransaction;

     com.arjuna.ArjunaOTS.GlobalTransactionInfo getGlobalInfo();

     com.arjuna.ArjunaOTS.TransactionInfo getTransactionInfo(
        org.omg.CosTransactions.otid_t uid
    ) throws org.omg.CosTransactions.NoTransaction;

     org.omg.CosTransactions.Control getTransaction(
        org.omg.CosTransactions.otid_t uid
    ) throws org.omg.CosTransactions.NoTransaction;

}
