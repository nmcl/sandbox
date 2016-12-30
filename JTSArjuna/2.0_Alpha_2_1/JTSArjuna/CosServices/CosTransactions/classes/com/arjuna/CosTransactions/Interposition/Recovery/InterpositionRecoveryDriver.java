/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: InterpositionRecoveryDriver.java,v 1.1.2.2 2000/06/28 13:24:58 nmcl Exp $
 */

package com.arjuna.CosTransactions.Interposition.Recovery;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaLite.Interface.ObjectStore;
import com.arjuna.CosTransactions.Interposition.Arjuna.OTS_ServerTopLevelAction;

//import com.arjuna.CosRecovery.RecoveryDriver

import com.arjuna.ArjunaLite.Common.ObjectStoreException;

interface RecoveryDriver
{

public boolean recover ();
public String id ();

};

public class InterpositionRecoveryDriver implements RecoveryDriver
{

public boolean recover ()
    {
	String[] types = OTS_InterpositionRecoveryFactory.types();
    
	if (types == null)
	    return true;  // nothing to recover!

	ObjectStore objStore = new ObjectStore();
	boolean result = false;
	
	/*
	 * For each type of registered interposition type,
	 * read the Uids and create a new instance. This instance
	 * is then responsible for its own recovery, and state
	 * management, i.e., if it eventually completes, then it
	 * will remove its state. The interposition type is
	 * also responsible for recovering the subordinate coordinator
	 * transaction.
	 */

	for (int i = 0; i < types.length; i++)
	{
	    String theType = types[i];
	    
	    if (theType != null)
	    {
		InputObjectState uids = new InputObjectState();

		try
		{
		    if (!objStore.allObjUids(theType, uids))
			uids = null;
		}
		catch (Exception e)
		{
		    uids = null;
		}

		if (uids != null)
		{
		    Uid theUid = new Uid();

		    result = true;

		    while (theUid.notEquals(Uid.nullUid()))
		    {
			try
			{
			    theUid.unpack(uids);
			}
			catch (Exception e)
			{
			    theUid = Uid.nullUid();
			}

			if (theUid.notEquals(Uid.nullUid()))
			{
			    /*
			     * Ask the interposition factory to create us an instance and
			     * then try to recover it.
			     *
			     * Assuming recovery eventually succeeds, this should also
			     * cause the state to be removed from the object store.
			     */
		    
			    if (!OTS_InterpositionRecoveryFactory.recover(theType, theUid))
			    {
				ErrorStream.stream(ErrorStream.WARNING).println("InterpositionRecoveryDriver.recover failed for "+theType+" and "+theUid);

				result = false;
			    }
			}
		    }

		    /*
		     * By the time we get here, all recoverable two-phase commit
		     * transactions should have been resolved, and their states
		     * removed. Any states left must be from interpositions which
		     * cannot yet be resolved.
		     */
		}
	    }
	}

	/*
	 * Now check for any outstanding server transactions. If they
	 * are Uids *and* we didn't fail to recover them, then recover
	 * back - if they'd got through prepare we would have an entry
	 * within the object store. If they were for one-phase commit calls
	 * then there will be no interposition state corresponding to them
	 * (since the interposition outcome is the transaction outcome),
	 * but there will be a transaction state.
	 */

	/*
	 * However, since these are transactions, it makes more sense for
	 * standard crash recovery to do it.
	 */
    
	return result;
    }

public String id ()
    {
	return "InterpositionDriver";
    }

};
