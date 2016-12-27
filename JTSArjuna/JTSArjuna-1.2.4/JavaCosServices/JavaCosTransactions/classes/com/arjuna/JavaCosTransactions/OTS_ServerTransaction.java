/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OTS_ServerTransaction.java,v 1.5 1998/07/06 13:28:43 nmcl Exp $
 */

package com.arjuna.JavaCosTransactions;

import com.arjuna.JavaGandiva.Common.Uid;
import com.arjuna.JavaArjunaLite.Atomic.PrepareOutcome;
import org.omg.CosTransactions.*;

/*
 * This looks like an OTS_Transaction, but is only create for
 * interposition purposes. The classes OTS_ServerTopLevelAction
 * and OTS_ServerNestedAction use instances of this class
 * to drive the server-side protocol.
 */

public class OTS_ServerTransaction extends OTS_Transaction
{

public OTS_ServerTransaction (Uid actUid, Control myParent)
    {
	this(actUid, myParent, null);
    }
    
public OTS_ServerTransaction (Uid actUid, Control myParent, OTS_Transaction parentImpl)
    {
	super(actUid, myParent, parentImpl);

	_savingUid = new Uid();
    }

public String type ()
    {
	return "/StateManager/BasicAction/OTS_Transaction/OTS_ServerTransaction";    
    }

public Uid getSavingUid ()
    {
	return _savingUid;
    }
    
public final int doPrepare ()
    {
	int res = super.prepare(true);

	if (res == PrepareOutcome.READONLY)
	{
	    /*
	     * Coordinator will not talk to us again.
	     */

	    doPhase2Commit();
	}

	return res;
    }
    
public final int doPhase2Commit ()
    {
	super.phase2Commit(true);

	if (parentTransaction != null)
	    parentTransaction.removeChildAction(this);
	
	return super.status();
    }

public final int doPhase2Abort ()
    {
	super.phase2Abort(true);

	if (parentTransaction != null)
	    parentTransaction.removeChildAction(this);
	
	return super.status();
    }

private Uid _savingUid;
    
};
