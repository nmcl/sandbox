/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_Synchronization.java,v 1.1.8.1.2.1.22.1.4.1 2000/12/21 11:21:17 nmcl Exp $
 */

package com.arjuna.CosTransactions;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaCommon.Template.*;
import org.omg.CosTransactions.Synchronization;

/**
 * CosTransactions::Synchronizations are maintained in a separate list
 * from the standard transaction intentions list.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: OTS_Synchronization.java,v 1.1.8.1.2.1.22.1.4.1 2000/12/21 11:21:17 nmcl Exp $
 * @since JTS 1.0.
 */

public class OTS_Synchronization implements ListElement
{
    
public OTS_Synchronization (Synchronization ptr)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_OTS, "OTS_Synchronization::OTS_Synchronization ( "+ptr+" )");
	}

	_ptr = ptr;
	_uid = new Uid();
    }

public void finalize ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.DESTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_OTS, "OTS_Synchronization.finalize ()");
	}

	_ptr = null;
	_uid = null;
    }

public final Synchronization contents ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_OTS, "OTS_Synchronization::contents - for "+_ptr);
	}

	return _ptr;
    }

public final Uid get_uid ()
    {
	return _uid;
    }

private Uid             _uid;
private Synchronization _ptr;

};
