//
// Java code automatically generated by the Orbix 2000 IDL Compiler
// Do not edit this code!
//

package org.omg.CosActivity;

public interface SubordinateSignalSetOperations
extends 
    org.omg.CosActivity.SignalSetOperations
{
     void set_signal(
        org.omg.CosActivity.Signal sig
    );

     org.omg.CosActivity.Outcome get_current_outcome() throws org.omg.CosActivity.SignalSetInactive;

}
