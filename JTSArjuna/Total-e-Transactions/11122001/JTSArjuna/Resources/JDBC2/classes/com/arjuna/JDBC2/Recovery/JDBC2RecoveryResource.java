/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: JDBC2RecoveryResource.java,v 1.1.2.1.22.2.8.1.2.1.38.1 2001/07/19 14:16:07 nmcl Exp $
 */

package com.arjuna.JDBC2.Recovery;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.jta.JTA_ResourceRecord;
import java.util.*;
import javax.transaction.xa.*;

/*
 * Default visibility.
 */

class JDBC2RecoveryResource extends JTA_ResourceRecord
{

public JDBC2RecoveryResource (Uid u)
    {
	super(u);
    }

public JDBC2RecoveryResource (Uid u, XAResource res)
    {
	super(u);

	super._theXAResource = res;
    }

public final XAResource getXAResource ()
    {
	return super._theXAResource;
    }
    
public final boolean recoverable ()
    {
	/*
	 * If we don't have an XAResource then we cannot recover at
	 * this stage. The XAResource will have to be provided for
	 * us and then we can retry.
	 */

	if (super._theXAResource == null)
	    return false;
	else
	    return true;
    }
    
public boolean recover ()
    {
	return super.recover();
    }

/**
 * Is the XAException a non-error when received in reply to commit or
 * rollback ?
 *
 * In recovery, the commit/rollback may have been sent before (possibly only
 * just before, in another thread) - in which case the RM will not recognise
 * the XID but id doesn't matter
 */

protected boolean notAProblem (XAException ex, boolean commit)
    {
	if (ex.errorCode == XAException.XAER_NOTA)
	{ 
	    if (DebugController.enabled())
	    {
		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						     FacilityCode.FAC_CRASH_RECOVERY, "JDBC2RecoveryResource.notAProblem - no error with XAER_NOTA on "
						     + (commit ? " commit" : " rollback"));
	    }
	    
	    return true;
	}
	return false;
    }

}
