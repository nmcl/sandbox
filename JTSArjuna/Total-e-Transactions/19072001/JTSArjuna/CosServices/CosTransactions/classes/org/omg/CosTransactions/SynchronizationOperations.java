//
// Java code automatically generated by the Orbix 2000 IDL Compiler
// Do not edit this code!
//

package org.omg.CosTransactions;

public interface SynchronizationOperations
extends 
    org.omg.CosTransactions.TransactionalObjectOperations
{
     void before_completion();

     void after_completion(
        org.omg.CosTransactions.Status s
    );

}
