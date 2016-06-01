/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: SynchronizationAction.java,v 1.1 2001/07/23 13:45:02 nmcl Exp $
 */

import com.arjuna.OrbCommon.ORBInterface;
import com.arjuna.OrbCommon.OAInterface;
import com.arjuna.Activity.*;
import org.omg.CosActivity.*;
import com.arjuna.ActivityTests.*;
import java.io.*;

import org.omg.CORBA.BAD_OPERATION;

public class SynchronizationAction
{
    
public static void main (String[] args)
    {
	try
	{
	    ORBInterface.initORB(args, null);
	    OAInterface.initOA();

	    CosActivityCurrentImple current = CosActivity.current();
	    CosActivityCoordinationCurrentImple coordCurrent = (CosActivityCoordinationCurrentImple) current;
	    DummyAction act = new DummyAction(DummyAction.NO_EXCEPTION, 0);
	    
	    current.begin(0);

	    current.set_completion_status(CompletionStatus.CompletionStatusSuccess);

	    coordCurrent.add_action(act.getReference(), com.arjuna.Activity.SignalSets.SystemDefined.Synchronization, 0);
	    
	    current.complete();

	    System.out.println();

	    act = new DummyAction(DummyAction.ACTION_ERROR, 0);

	    current.begin(0);

	    current.set_completion_status(CompletionStatus.CompletionStatusSuccess);
	    
	    coordCurrent.add_action(act.getReference(), com.arjuna.Activity.SignalSets.SystemDefined.Synchronization, 0);
	    
	    /*
	     * The completion status should not be the same as requested,
	     * so ACTIVITY_COMPLETED should be thrown.
	     */

	    try
	    {
		current.complete();    

		System.err.println("Error - did not receive ACTIVITY_COMPLETED exception.");
		
		System.exit(0);
	    }
	    catch (BAD_OPERATION e)
	    {
	    }

	    System.out.println();

	    act = new DummyAction(DummyAction.UNKNOWN_EXCEPTION, 0);

	    current.begin(0);

	    current.set_completion_status(CompletionStatus.CompletionStatusSuccess);
	    
	    coordCurrent.add_action(act.getReference(), com.arjuna.Activity.SignalSets.SystemDefined.Synchronization, 0);
	    
	    /*
	     * The completion status should not be the same as requested,
	     * so ACTIVITY_COMPLETED should be thrown.
	     */

	    try
	    {
		current.complete();    

		System.err.println("Error - did not receive ACTIVITY_COMPLETED exception.");
		
		System.exit(0);
	    }
	    catch (BAD_OPERATION e)
	    {
	    }

	    /*
	     * The following tests should ensure we only get a postCompletion
	     * call.
	     */

	    System.out.println();

	    act = new DummyAction(DummyAction.NO_EXCEPTION, 0);

	    current.begin(0);

	    current.set_completion_status(CompletionStatus.CompletionStatusFailOnly);
	    
	    coordCurrent.add_action(act.getReference(), com.arjuna.Activity.SignalSets.SystemDefined.Synchronization, 0);
	    
	    current.complete();    

	    System.out.println();

	    act = new DummyAction(DummyAction.NO_EXCEPTION, 0);

	    current.begin(0);

	    current.set_completion_status(CompletionStatus.CompletionStatusFail);
	    
	    coordCurrent.add_action(act.getReference(), com.arjuna.Activity.SignalSets.SystemDefined.Synchronization, 0);
	    
	    current.complete();

	    /*
	     * Now try to order some actions.
	     */

	    System.out.println();
	    
	    DummyAction act1 = new DummyAction(DummyAction.NO_EXCEPTION, 1);
	    DummyAction act2 = new DummyAction(DummyAction.NO_EXCEPTION, 2);
	    DummyAction act3 = new DummyAction(DummyAction.NO_EXCEPTION, 3);

	    current.begin(0);

	    current.set_completion_status(CompletionStatus.CompletionStatusSuccess);
	    
	    coordCurrent.add_action(act1.getReference(), com.arjuna.Activity.SignalSets.SystemDefined.Synchronization, 1);
	    coordCurrent.add_action(act2.getReference(), com.arjuna.Activity.SignalSets.SystemDefined.Synchronization, 2);
	    coordCurrent.add_action(act3.getReference(), com.arjuna.Activity.SignalSets.SystemDefined.Synchronization, 3);
	    
	    current.complete();
	}
	catch (Exception e)
	{
	    e.printStackTrace();
	}

	OAInterface.shutdownOA();
	ORBInterface.shutdownORB();

	System.exit(0);
    }

};
