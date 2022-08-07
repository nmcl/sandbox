/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OAShutdown.java,v 1.1.8.1.22.1.2.1.2.1 2000/12/21 11:21:27 nmcl Exp $
 */

package com.arjuna.OrbCommon;

import com.arjuna.ArjunaCommon.Common.*;
import java.io.*;

/**
 * Common methods for OA shutdown.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: OAShutdown.java,v 1.1.8.1.22.1.2.1.2.1 2000/12/21 11:21:27 nmcl Exp $
 * @since JTS 1.0.
 */

public abstract class OAShutdown
{
    
public abstract void work ();

public final String name ()
    {
	return _name;
    }
    
protected OAShutdown (String theName)
    {
	_name = theName;

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_ORB_PORTABILITY, "OAShutdown.OAShutdown ("+theName+")");
	}
    }

private String _name;

};
