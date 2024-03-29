/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: GlobalAction.java,v 1.1 2001/07/23 13:45:01 nmcl Exp $
 */

import com.arjuna.ActivityTests.*;
import com.arjuna.OrbCommon.ORBInterface;
import com.arjuna.OrbCommon.OAInterface;
import com.arjuna.Activity.*;
import org.omg.CosActivity.*;
import java.io.*;

public class GlobalAction
{
    
public static void main (String[] args)
    {
	try
	{
	    ORBInterface.initORB(args, null);
	    OAInterface.initOA();

	    CosActivityCurrentImple current = CosActivity.current();
	    CosActivityCoordinationCurrentImple coordCurrent = (CosActivityCoordinationCurrentImple) current;
	    PrintWriter strm = new PrintWriter(System.out);
	    SignalSet ss1 = (new fooSignalSet()).getReference();
	    SignalSet ss2 = (new barSignalSet()).getReference();
	    DummyAction act1 = new DummyAction(DummyAction.NO_EXCEPTION, 0);
	    DummyAction act2 = new DummyAction(DummyAction.NO_EXCEPTION, 1);

	    /*
	     * Start a top-level activity.
	     */

	    current.begin(0);
	    
	    System.out.println("Created activity: "+current.get_activity_name()+"\n");

	    /*
	     * Now register the two SignalSets.
	     */

	    coordCurrent.add_signal_set(ss1);
	    coordCurrent.add_signal_set(ss2);

	    /*
	     * Now register a normal action.
	     */

	    coordCurrent.add_action(act1.getReference(), ss1.signal_set_name(), 0);

	    /*
	     * Now register a global action.
	     */

	    coordCurrent.add_global_action(act2.getReference(), 0);

	    current.set_completion_signal_set(ss1.signal_set_name());
	    
	    current.complete_with_status(org.omg.CosActivity.CompletionStatus.CompletionStatusSuccess);
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
