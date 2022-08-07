/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: RecoveredServerTransaction.javatmpl,v 1.1.2.1.2.3.4.1.6.1.2.1 2001/05/24 12:31:34 nmcl Exp $
 */

/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OrbPortability.h,v 1.1 2000/02/25 14:09:59 nmcl Exp $
 */




/*
 * Try to get around the differences between Ansi CPP and
 * K&R cpp with concatenation.
 */


/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: OrbPortability_stdc.h,v 1.2.4.1.2.3.26.1.2.2.4.2 2001/02/02 11:51:52 nmcl Exp $
 */
































































package com.arjuna.CosRecovery.Transactions;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaCore.Atomic.*;
import com.arjuna.ArjunaCore.Interface.ObjectStore;
import com.arjuna.ArjunaCore.Common.ObjectStoreException;
import com.arjuna.CosTransactions.Interposition.*;
import com.arjuna.CosTransactions.Interposition.Arjuna.OTS_ServerRecoveryTopLevelAction;
import com.arjuna.CosTransactions.OTS_Utility;
import org.omg.CosTransactions.*;
import java.io.IOException;
import java.util.Date;

import com.arjuna.CosRecovery.Contact.StatusChecker;

import org.omg.CORBA  .SystemException;
import org.omg.CORBA.OBJECT_NOT_EXIST;


/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: CosTranExceptions.h,v 1.1.2.1.28.1.14.1 2001/03/20 16:24:46 nmcl Exp $
 */





/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: CosTranNames.h,v 1.1.42.1 2001/03/20 16:24:47 nmcl Exp $
 */




/*
 * Simple renaming macros for portability.
 *
 * Slightly different from C++ version in that we don't need the
 * typedefs. However, we do need some mapping for OTS exceptions which
 * should now be system exceptions.
 */

/*
 * CosTransaction module exceptions
 */






/*
 * We need to check this. WRONG_TRANSACTION is supposed to
 * be a system exception (in the latest draft of the OTS),
 * but two ORBs have it as a user exception.
 */





/**/





/*
 * An attempt at a portable way of including the OTS exceptions,
 * which are either mapped as system exceptions or user exceptions,
 * depending upon the Orb.
 */



import org.omg.CosTransactions.WrongTransaction;

import org.omg.CORBA.INVALID_TRANSACTION;
import org.omg.CORBA.TRANSACTION_REQUIRED;
import org.omg.CORBA.TRANSACTION_ROLLEDBACK;





/**
 * Transaction type only instantiated at recovery time.  This is used
 * to re-activate the state of a server transaction that did not
 * terminate correctly due to failure.
 * <P>
 * @author Dave Ingham (dave@arjuna.com)
 * @version $Id: RecoveredServerTransaction.javatmpl,v 1.1.2.1.2.3.4.1.6.1.2.1 2001/05/24 12:31:34 nmcl Exp $ */

public class RecoveredServerTransaction extends OTS_ServerTransaction
			implements RecoveringTransaction
{
    public RecoveredServerTransaction ( Uid actionUid )
    {
	this(actionUid, "");
    }

    public RecoveredServerTransaction ( Uid actionUid, String changedTypeName)
    {
	super(actionUid);

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_CRASH_RECOVERY,
						 "RecoveredServerTransaction("+get_uid()+") created");
	}

	// Don't bother trying to activate a transaction that isn't in
	// the store. This saves an error message.
	_recoveryStatus = RecoveryStatus.ACTIVATE_FAILED;

	String effectiveTypeName = typeName();
	
	if ( changedTypeName.length() < 1) {
	    _typeName = null;
	} else {
	    _typeName = changedTypeName;
	    effectiveTypeName = changedTypeName;
	}

	_originalProcessUid = new Uid(Uid.nullUid());
	try
	{
	    if ((store().currentState(actionUid, effectiveTypeName) != ObjectStore.OS_UNKNOWN))
	    {
		if (activate())
		    _recoveryStatus = RecoveryStatus.ACTIVATED;
		else
		    ErrorStream.warning().println("RecoveredServerTransaction - activate of "+actionUid+" failed!");
	    }
	}
	catch (Exception e)
	{
	    ErrorStream.warning().println("RecoveredServerTransaction - activate of "+actionUid+" failed: "+e);
	}
    }

    public void finalize ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.DESTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_CRASH_RECOVERY,
						 "RecoveredServerTransaction("+get_uid()+") destroyed");
	}
	super.finalize();
    }

    /**
     * Get the status of the transaction. If we successfully activated
     * the transaction then we return whatever the transaction reports
     * otherwise we return RolledBack as we're using presumed abort.
     */
    public synchronized Status get_status () throws SystemException
    {
	Status theStatus = Status.StatusUnknown;
	if (_recoveryStatus == RecoveryStatus.ACTIVATE_FAILED)
	    theStatus = Status.StatusRolledBack;
	else
	    theStatus = super.get_status();

	return theStatus;
    }

    /**
     * Allows a new Resource to be added to the transaction. Typically
     * this is used to replace a Resource that has failed and cannot
     * be recovered on it's original IOR.
     */
    public void addResourceRecord (Uid rcUid, Resource r)
    {
	Coordinator coord = null;
	AbstractRecord corbaRec = createOTSRecord(true, r, coord, rcUid);
	addRecord(corbaRec);
    }


    /**
     * Causes phase 2 of the commit protocol to be replayed.
     */
    public void replayPhase2()
    {
	_recoveryStatus = RecoveryStatus.REPLAYING;

	Status theStatus = get_status();

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_CRASH_RECOVERY,
						 "RecoveredServerTransaction.replayPhase2("+get_uid()+")"
						 + " - status = "+OTS_Utility.printStatus(theStatus));
	}

	if (theStatus == Status.StatusPrepared)
	{
	    /*
	     * We need to get the status from the our parent transaction
	     * in the interposition hierarchy.
	     */
	    theStatus = getStatusFromParent();
	    if (DebugController.enabled())
	    {
		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						     FacilityCode.FAC_CRASH_RECOVERY,
						     "RecoveredServerTransaction.replayPhase2("+get_uid()+")"
						     + " - status = "+OTS_Utility.printStatus(theStatus));
	    }

	}

	if ( (theStatus == Status.StatusCommitting) ||
	     (theStatus == Status.StatusCommitted) )
	{
	  phase2Commit(_reportHeuristics);
	}
	else if ( (theStatus == Status.StatusRolledBack) ||
		  (theStatus == Status.StatusRollingBack) ||
		  (theStatus == Status.StatusMarkedRollback) )
	{
	  phase2Abort(_reportHeuristics);
	}
	else if ( theStatus == Status.StatusUnknown )
	{
	    if (DebugController.enabled())
	    {
		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
				 FacilityCode.FAC_RECOVERY_NORMAL,
				 "ServerTransaction "+get_uid()+" unable determine status - retry later");
	    }
	}
	else
	{
	  ErrorStream.warning().println("RecoveredServerTransaction.replayPhase2: unexpected Status: "
							  +OTS_Utility.printStatus(theStatus));
	}

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_CRASH_RECOVERY,
						 "RecoveredServerTransaction.replayPhase2("+get_uid()+") finished");
	}
	_recoveryStatus = RecoveryStatus.REPLAYED;
    }

    /**
     * Get the status of recovery for this transaction
     */
    public int getRecoveryStatus ()
    {
	return _recoveryStatus;
    }

/**
 *  
 */
public Status getOriginalStatus()
{
    if (_recoveryStatus != RecoveryStatus.ACTIVATE_FAILED)
    {
	try
	{
	    return StatusChecker.get_status(get_uid(), _originalProcessUid);
	}
	catch (Inactive ex)
	{
	    // shouldn't happen!!

	    return Status.StatusUnknown;
	}
    }
    else
    {
	// if it can't be activated, we cant get the process uid
	return Status.StatusUnknown;
    }

}


    private Status getStatusFromParent ()
    {
      org.omg.CosTransactions.Status theStatus = org.omg.CosTransactions.Status.StatusUnknown;

      if ((super._recoveryCoordinator != null) && (get_status() == org.omg.CosTransactions.Status.StatusPrepared))
      {
	OTS_ServerControl sc = new OTS_ServerControl((OTS_ServerTransaction) this);
	OTS_ServerRecoveryTopLevelAction tla = new OTS_ServerRecoveryTopLevelAction(sc);

	if (tla.valid())
	{
	  try
	  {
	    theStatus = super._recoveryCoordinator.replay_completion(tla.getReference());
	    if (DebugController.enabled())
	    {
		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
			     FacilityCode.FAC_CRASH_RECOVERY,
			     "RecoveredServerTransaction.getStatusFromParent - replay_completion status = " + OTS_Utility.printStatus(theStatus));
	    }
	  }
	  catch (OBJECT_NOT_EXIST ex)
	  {
	      // i believe this state should be notran - ots explicitly objnotexist is
	      // rollback
	    theStatus = org.omg.CosTransactions.Status.StatusUnknown;
	    if (DebugController.enabled())
	    {
		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
			     FacilityCode.FAC_CRASH_RECOVERY,
			     "RecoveredServerTransaction.getStatusFromParent - replay_completion got object_not_exist = " + OTS_Utility.printStatus(theStatus));
	    }
	  }

	  catch (NotPrepared ex1)
	  {
	    ErrorStream.warning().println("RecoveredServerTransaction: caught NotPrepared");

	    theStatus = org.omg.CosTransactions.Status.StatusActive;
	  }
	  catch (Exception e)
	  {
	    // Unknown error, so better to do nothing at this stage.
	    ErrorStream.warning().println("RecoveredServerTransaction: caught unexpected exception: "+e);
	  }
	}
	else
	  ErrorStream.warning().println("RecoveredServerTransaction "+get_uid()+" is invalid!");

	// Make sure we "delete" these objects when we are finished
	// with them or there will be a memory leak. If they are deleted
	// "early", and the root coordinator needs them then it will find
	// them unavailable, and will have to retry recovery later.

	sc = null;
	tla = null;
      }
      else
      {
	if (DebugController.enabled())
	    {
		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
			     FacilityCode.FAC_CRASH_RECOVERY,
			     "RecoveredServerTransaction.getStatusFromParent - no recovcoord or status not prepared");
	    }

      }
    
      return theStatus;
    }

    public boolean allCompleted()
    {
	return false;
    }

    public String type()
    {
	if (_typeName == null) {
	    return super.type();
	} else {
	    return _typeName;
	}
    }

    public void removeOldStoreEntry()
    {
	try {
	    store().remove_committed(get_uid(), super.type());
	} catch (ObjectStoreException ex) {
	    ErrorStream.warning().println("RecoveredServerTransaction.removeOldStoreEntry " + ex);
	}
    }
    
    public boolean assumeComplete()
    {
	_typeName = AssumedCompleteServerTransaction.typeName();
	return true;
    }
    /**
     * Override StateManager packHeader so it gets the original processUid, not
     * this process's
     *
     * @since JTS 2.1.
     */

protected void packHeader (OutputObjectState os, Uid txId,
			   Uid processUid) throws IOException
    {
	/*
	 * If there is a transaction present than pack the process Uid of
	 * this JVM and the tx id. Otherwise pack a null Uid.
	 */
	
	super.packHeader(os, txId, _originalProcessUid);
    }

    /**
     * Override StateManager's unpackHeader to save the processUid of the
     * original process
     *
     * @since JTS 2.1.
     */

protected void unpackHeader (InputObjectState os, Uid txId,
			     Uid processUid) throws IOException
    {
	_originalProcessUid = new Uid(Uid.nullUid());
	super.unpackHeader(os, txId, _originalProcessUid);
    }

public boolean save_state (OutputObjectState objectState, int ot)
{
    // do the other stuff
    boolean result = super.save_state(objectState,ot);
    
    // iff assumed complete, include the time (this should happen only once)
    if (_typeName != null && result) {
	Date lastActiveTime = new Date();
	try {
	    objectState.packLong(lastActiveTime.getTime());
	} catch (java.io.IOException ex) {
	}
    }
    return result;
}

/** do not admit to being inactive */
public Date getLastActiveTime()
{
    return null;
}


    private String _typeName;

    private boolean  _reportHeuristics = false;
    private int	 _recoveryStatus = RecoveryStatus.NEW;
    protected Uid _originalProcessUid;
}
