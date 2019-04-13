/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ChildLifetimeAction.java,v 1.1 2001/07/23 13:45:01 nmcl Exp $
 */

import com.arjuna.OrbCommon.ORBInterface;
import com.arjuna.OrbCommon.OAInterface;
import com.arjuna.Activity.*;
import org.omg.CosActivity.*;
import com.arjuna.ActivityTests.*;
import java.io.*;

public class ChildLifetimeAction
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

	    coordCurrent.add_action(act.getReference(), com.arjuna.Activity.SignalSets.SystemDefined.ChildLifetime, 0);
	    
	    current.begin(0);
	    
	    current.complete();
	    
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
