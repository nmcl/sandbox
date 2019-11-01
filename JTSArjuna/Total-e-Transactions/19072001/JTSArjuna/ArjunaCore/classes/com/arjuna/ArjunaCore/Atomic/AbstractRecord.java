/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Hewlett Packard Arjuna Labs,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: AbstractRecord.java,v 1.1.2.3.4.2.4.1.32.1.4.1 2001/06/28 14:00:59 nmcl Exp $
 */

package com.arjuna.ArjunaCore.Atomic;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaCommon.Interface.Inventory;
import java.io.PrintWriter;

import java.io.IOException;

/**
 * Abstract Record Class
 * This class provides an abstract template that defines the interface
 * that the atomic action system uses to notify objects that various
 * state transitions have occurred as the 2PC protocol executes.
 * Record types derived from this class manage certain properties
 * of objects such as recovery information, concurrency control
 * information etc, and all must redifine the operations defined
 * here as abstract to take appropriate action.
 *
 * Many functions are declared pure virtual to force a definition to occur
 * in any derived class. These are currently all functions dealing with 
 * atomic action coordination as well as the following list management 
 * functions:
 *     typeIs:   returns the record type of the instance. This is one of
 *               the values of the enumerated type Record_type
 *     value:    Some arbitrary value associated with the record instance
 *     merge:    Used when two records need to merge togethor. Currently
 *               this is only used by CadaverRecords to merge information
 *               from PersistenceRecords
 *     shouldAdd:
 *               returns TRUE is the record should be added to the list
 *               FALSE if it should be discarded
 *     shouldMerge:
 *               returns TRUE is the two records should be merged into a
 *               single record, FALSE if it should be discarded
 *     shouldReplace:
 *               returns TRUE if the record should replace an existing
 *               one, FALSE otherwise.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: AbstractRecord.java,v 1.1.2.3.4.2.4.1.32.1.4.1 2001/06/28 14:00:59 nmcl Exp $
 * @since JTS 1.0.
 */

public abstract class AbstractRecord extends StateManager
{

    /**
     * @return <code>RecordType</code> value.
     */

public abstract int typeIs ();

    /**
     * @return <code>Object</code> to be used to order.
     */

public abstract Object value ();
public abstract void setValue (Object o);

    /**
     * Only used for crash recovery, so most records don't need it.
     *
     * @return <code>ClassName</code> to identify this abstract record.
     */

public ClassName className ()
    {
	return new ClassName(""+typeIs());
    }

    /**
     * Atomic action interface - one operation per two-phase commit state.
     */

    /**
     * A rollback of a nested transaction has occurred.
     *
     * @return <code>PrepareOutcome</code> to indicate success/failure.
     * @see com.arjuna.ArjunaCore.Atomic.PrepareOutcome
     */

public abstract int nestedAbort ();

    /**
     * A commit of a nested transaction has occurred.
     *
     * @return <code>PrepareOutcome</code> to indicate success/failure.
     * @see com.arjuna.ArjunaCore.Atomic.PrepareOutcome
     */

public abstract int nestedCommit ();

    /**
     * A prepare for a nested transaction has occurred.
     *
     * @return <code>PrepareOutcome</code> to indicate success/failure.
     * @see com.arjuna.ArjunaCore.Atomic.PrepareOutcome
     */

public abstract int nestedPrepare ();

    /**
     * A rollback of a top-level transaction has occurred.
     *
     * @return <code>PrepareOutcome</code> to indicate success/failure.
     * @see com.arjuna.ArjunaCore.Atomic.PrepareOutcome
     */

public abstract int topLevelAbort ();

    /**
     * A commit of a top-level transaction has occurred.
     *
     * @return <code>PrepareOutcome</code> to indicate success/failure.
     * @see com.arjuna.ArjunaCore.Atomic.PrepareOutcome
     */

public abstract int topLevelCommit ();

    /**
     * A prepare for a top-level transaction has occurred.
     *
     * @return <code>PrepareOutcome</code> to indicate success/failure.
     * @see com.arjuna.ArjunaCore.Atomic.PrepareOutcome
     */

public abstract int topLevelPrepare ();

    /**
     * Return the Uid of this abstract record so that it can be ordered
     * in the abstract record list.
     *
     * @return <code>Uid</Uid>
     */

public Uid order ()
    {
	return uidOfObject;
    }

    /**
     * Return the type of the abstract record. Used in ordering the instances
     * in the record list.
     *
     * @return <code>String</code> representing type.
     */

public String getTypeOfObject ()
    {
	return typeOfObject;
    }

    /**
     * Determine if records are discarded on action abort or must be
     * propagated to parents.
     *
     * @return <code>true</code> if the record should be propagated
     * to the parent transaction if the current transaction rolls back,
     * <code>false</code> otherwise. The default is <code>false</code>.
     */

public boolean propagateOnAbort ()
    {
	return false;
    }

    /**
     * Determine if records are discarded on action commit or must be
     * propagated to parents.
     *
     * @return <code>true</code> if the record should be propagated
     * to the parent transaction if the current transaction commits,
     * <code>false</code> otherwise. The default is <code>true</code>.
     */

public boolean propagateOnCommit ()
    {
	return true;
    }

    /**
     * Operators for comparing and sequencing instances of classes derived
     * from AbstractRecords.
     * Records are ordered primarily based upon the value of 'order',
     * followed by 'typeIs'.
     */

    /**
     * Determine if two records are equal in that both are the same type
     * and have the same order value (determined via 'order()').
     *
     * @return <code>true</code> if equal, <code>false</code> otherwise.
     */

public final boolean equals (AbstractRecord ar)
    {
	return ((com.arjuna.ArjunaCore.Common.Configuration.useAlternativeOrdering()) ? typeEquals(ar) : orderEquals(ar));
    }

    /**
     * Determine if two records are less than in that both are the same type
     * and their Uids are less than.
     *
     * @return <code>true</code> if equal, <code>false</code> otherwise.
     */

public final boolean lessThan (AbstractRecord ar)
    {
	return ((com.arjuna.ArjunaCore.Common.Configuration.useAlternativeOrdering()) ? typeLessThan(ar) : orderLessThan(ar));
    }

    /**
     * Determine if two records are greater than in that both are the same type
     * and their Uids are greater than.
     *
     * @return <code>true</code> if equal, <code>false</code> otherwise.
     */

public final boolean greaterThan (AbstractRecord ar)
    {
	return ((com.arjuna.ArjunaCore.Common.Configuration.useAlternativeOrdering()) ? typeGreaterThan(ar) : orderGreaterThan(ar));
    }
    
    /**
     * Cleanup is called if a top-level action is detected to be an orphan
     * NOTE nested actions are never orphans since their parents would
     * be aborted we may as well abort them as well
     *
     * @return <code>PrepareOutcome</code> as default is the same as
     * topLevelAbort.
     */

public int topLevelCleanup ()
    {
	return topLevelAbort();
    }

    /**
     * Cleanup is called if a nested is detected to be an orphan
     * NOTE nested actions are never orphans since their parents would
     * be aborted we may as well abort them as well
     *
     * @return <code>PrepareOutcome</code> as default is the same as
     * nestedAbort.
     */

public int nestedCleanup ()
    {
	return nestedAbort();
    }

    /**
     * Should this record be saved in the intentions list?
     *
     * @return <code>true</code> if it should be saved, <code>false</code>
     * otherwise. <code>false</code> is the default.
     */

public boolean doSave ()
    {
	return false;
    }

    /**
     * Re-implementation of abstract methods inherited from base class.
     */

public String type ()
    {
	return "/StateManager/AbstractRecord";
    }

public void print (PrintWriter strm)
    {
	strm.println("Uid of Managed Object: "+uidOfObject);
	strm.println("Type of Managed Object: "+typeOfObject);
	super.print(strm);
    }

    /**
     * save_state and restore_state here are used by the corresponding
     * operations of persistence records for crash recovery purposes.
     *
     * @return <code>true</code> if successful, <code>false</code>
     * otherwise.
     */

public boolean restore_state (InputObjectState os, int i)
    {
	typeOfObject = null;
	
	try
	{
	    uidOfObject.unpack(os);
	    typeOfObject = os.unpackString();

	    return true;
	}
	catch (IOException e)
	{
	    return false;
	}
    }

    /**
     * save_state and restore_state here are used by the corresponding
     * operations of persistence records for crash recovery purposes.
     *
     * @return <code>true</code> if successful, <code>false</code>
     * otherwise.
     */

public boolean save_state (OutputObjectState os, int i)
    {
	try
	{
	    uidOfObject.pack(os);
	    os.packString(typeOfObject);

	    return true;
	}
	catch (IOException e)
	{
	    return false;
	}
    }
    
    /*
     * OTS specific methods which we cannot hide!
     */

    /**
     * Forget any heuristic outcome which this implementation may have
     * produced.
     *
     * @return <code>true</code> by default.
     */

public boolean forgetHeuristic ()
    {
	return true;
    }

    /**
     * Perform a nested one phase commit.
     *
     * @return <code>PrepareOutcome</code>
     */

public int nestedOnePhaseCommit ()
    {
	int res = nestedPrepare();

	switch (res)
	{
	case PrepareOutcome.PREP_OK:
	    return nestedCommit();
	case PrepareOutcome.PREP_READONLY:
	    return PrepareOutcome.FINISH_OK;
	default:
	    return PrepareOutcome.FINISH_ERROR;
	}
    }

    /**
     * Perform a top-level one phase commit.
     *
     * @return <code>PrepareOutcome</code>
     */

public int topLevelOnePhaseCommit ()
    {
	int res = topLevelPrepare();

	switch (res)
	{
	case PrepareOutcome.PREP_OK:
	    return topLevelCommit();
	case PrepareOutcome.PREP_READONLY:
	    return PrepareOutcome.FINISH_OK;
	default:
	    return PrepareOutcome.FINISH_ERROR;
	}
    }

    /*
     * This could be done through interface/implementation
     * separation for AbstractRecords. However, since we
     * only need to dynamically create records for crash
     * recovery purposes we may not need such flexibility.
     */

public static AbstractRecord create (ClassName cName)
    {
	Object ptr = Inventory.inventory().createVoid(cName);
	AbstractRecord record = null;
		
	if (ptr instanceof AbstractRecord)
	    record = (AbstractRecord) ptr;
	else
	    record = null;

	return record;
    }

    /*
     * We would like these to be protected, but the
     * requirement for a RecoveryAbstractRecord interface
     * which is in a different package prevents this.
     */

    /**
     * Merge the current record with the one presented.
     */

public abstract void merge (AbstractRecord a);

    /**
     * Alter the current record with the one presented.
     */

public abstract void alter (AbstractRecord a);

    /**
     * Should we add the record presented to the intentions list?
     */

public abstract boolean shouldAdd (AbstractRecord a);

    /**
     * Should we alter the current record with the one presented?
     */

public abstract boolean shouldAlter (AbstractRecord a);

    /**
     * Should we merge the current record with the one presented?
     */

public abstract boolean shouldMerge (AbstractRecord a);

    /**
     * Should we replace the record presented with the current record?
     */

public abstract boolean shouldReplace (AbstractRecord a);

    /**
     * The current record is about to replace the one presented.
     * This method is invoked to give the current record a chance to
     * copy information, for example, from the record being replaced.
     */

public void replace (AbstractRecord a)
    {
    }
    
    /**
     * These few functions are link manipulation primitives used by the
     * RecordList processing software to chain instances together.
     */

protected final AbstractRecord getPrevious ()
    {
	return previous;
    }

protected final AbstractRecord getNext ()
    {
	return next;
    }

protected final void setPrevious (AbstractRecord ar)
    {
	previous = ar;
    }

protected final void setNext (AbstractRecord ar)
    {
	next = ar;
    }

protected AbstractRecord (Uid storeUid, String objType, int otype)
    {
	super(otype);
	
	next = null;
	previous = null;
	uidOfObject = new Uid(storeUid);
	typeOfObject = objType;

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS,
						 VisibilityLevel.VIS_PROTECTED,
						 FacilityCode.FAC_ABSTRACT_REC, "AbstractRecord::AbstractRecord ("+storeUid+", "+otype+")");
	}
    }

protected AbstractRecord (Uid storeUid)
    {
	super(storeUid);
	
	next = null;
	previous = null;
	uidOfObject = new Uid(storeUid);
	typeOfObject = null;

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS,
						 VisibilityLevel.VIS_PROTECTED,
						 FacilityCode.FAC_ABSTRACT_REC, "AbstractRecord::AbstractRecord ("+storeUid+")");
	}
    }

    /**
     * Creates a 'blank' abstract record. This is used during crash recovery
     * when recreating the prepared list of a server atomic action.
     */

protected AbstractRecord ()
    {
	super(Uid.nullUid());
	
	next = null;
	previous = null;
	uidOfObject = new Uid(Uid.nullUid());
	typeOfObject = null;

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS,
						 VisibilityLevel.VIS_PROTECTED,
						 FacilityCode.FAC_ABSTRACT_REC, "AbstractRecord::AbstractRecord () - crash recovery constructor");
	}
    }

    /**
     * ensure records of the same type are grouped together
     * in the list, rather than grouping them by object (i.e. uid)
     */
    
private final boolean typeEquals (AbstractRecord ar)
    {
	return ((typeIs() == ar.typeIs()) &&
		(order().equals(ar.order())));
    }

private final boolean typeLessThan (AbstractRecord ar)
    {
	return ((typeIs() < ar.typeIs()) ||
		((typeIs() == ar.typeIs()) &&
		 (order().lessThan(ar.order()))));
    }

private final boolean typeGreaterThan (AbstractRecord ar)
    {
	return ((typeIs() > ar.typeIs()) ||
		((typeIs() == ar.typeIs()) &&
		 (order().greaterThan(ar.order()))));
    }

private final boolean orderEquals (AbstractRecord ar)
    {
	return ((order().equals(ar.order())) &&
		(typeIs() == ar.typeIs()));
    }

private final boolean orderLessThan (AbstractRecord ar)
    {
	return ((order().lessThan(ar.order())) ||
		((order().equals(ar.order())) &&
		 (typeIs() < ar.typeIs())));
    }

private final boolean orderGreaterThan (AbstractRecord ar)
    {
	return ((order().greaterThan(ar.order())) ||
		((order().equals(ar.order())) &&
		 (typeIs() > ar.typeIs())));
    }

private AbstractRecord next;
private AbstractRecord previous;
private Uid            uidOfObject;
private String         typeOfObject;
    
}

