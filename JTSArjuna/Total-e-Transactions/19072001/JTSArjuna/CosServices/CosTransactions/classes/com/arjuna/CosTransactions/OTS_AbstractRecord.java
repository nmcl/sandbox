/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_AbstractRecord.javatmpl,v 1.3.2.1.4.2.2.1.2.5.18.1.2.2.4.1.2.1.4.2.8.1 2001/04/18 12:47:17 nmcl Exp $
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
































































/*
 * 
 * OTS Abstract Record Class Implementation.
 *
 * (Extended resource functionality.)
 *
 */

package com.arjuna.CosTransactions;

import com.arjuna.OrbCommon.ORBInterface;
import org.omg.CosTransactions.*;
import com.arjuna.ArjunaOTS.*;
import com.arjuna.ArjunaCore.Atomic.*;
import com.arjuna.ArjunaCommon.Common.*;
import java.io.PrintWriter;

import org.omg.CORBA.SystemException;
import org.omg.CORBA.BAD_PARAM;
import org.omg.CORBA.UNKNOWN;

import org.omg.CosTransactions.NotPrepared;
import org.omg.CosTransactions.HeuristicCommit;
import org.omg.CosTransactions.HeuristicMixed;
import org.omg.CosTransactions.HeuristicHazard;
import org.omg.CosTransactions.HeuristicRollback;
import java.io.IOException;

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
 * This abstract record is used whenever resources are derived from the
 * ArjunaOTS module's AbstractRecord interface. This gives users the
 * flexibility of the original Arjuna system's AbstractRecord, and makes
 * resources behave correctly!
 *
 * We know that instances of this record will only be called for instances of
 * AbstractRecord objects.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: OTS_AbstractRecord.javatmpl,v 1.3.2.1.4.2.2.1.2.5.18.1.2.2.4.1.2.1.4.2.8.1 2001/04/18 12:47:17 nmcl Exp $
 * @since JTS 1.0.
 */

public class OTS_AbstractRecord extends com.arjuna.ArjunaCore.Atomic.AbstractRecord
{

    /**
     * @param propagate tells us whether to propagate the resource at nested
     * commit or not.
     * @param theResource is the proxy that allows us to call out to the
     * object.
     * @param myParent is the proxy for the parent coordinator needed in
     * commit_subtransaction.
     */
    
public OTS_AbstractRecord ( boolean propagate, Uid objUid, ArjunaSubtranAwareResource theResource,
			    Coordinator myParent, Uid recCoordUid, OTS_Transaction current )
    {
	super(objUid, null, ObjectType.ANDPERSISTENT);
	
	_resourceHandle = theResource;
	_stringifiedResourceHandle = null;
	_parentCoordHandle = myParent;
	_recCoordUid = (recCoordUid != null) ? recCoordUid : Uid.nullUid();
	_currentTransaction = current;
	_propagateRecord = propagate;
    }

public void finalize () throws Throwable
    {
	_resourceHandle = null;
	_stringifiedResourceHandle = null;
	_recCoordUid = null;
	_parentCoordHandle = null;
	_currentTransaction = null;

	super.finalize();
    }
    
    /**
     * Specific OTS method for getting at the value.
     */
    
public final ArjunaSubtranAwareResource resourceHandle ()
    {
	/*
	 * After recovery we may have not been able to recreate the
	 * _resourceHandle due to the fact that the Resource itself
	 * may not be alive resulting in a failure to narrow the
	 * reference returned from string_to_object. In such cases we
	 * cache the stringified reference and retry the narrow when we
	 * need to use the _resourceHandle as at this point the
	 * Resource may have recovered.
	 */ 
	
	if ( (_resourceHandle == null) && (_stringifiedResourceHandle != null) )
	{
	    try
	    {
		org.omg.CORBA.ORB theOrb = ORBInterface.orb();
		
		if (theOrb == null)
		    throw new UNKNOWN();

		if (DebugController.enabled())
		{
		    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
							 (FacilityCode.FAC_OTS | FacilityCode.FAC_ABSTRACT_REC), "OTS_AbstractRecord: About to string_to_object on "+_stringifiedResourceHandle);
		}

		org.omg.CORBA.Object optr = theOrb.string_to_object(_stringifiedResourceHandle);

		if (DebugController.enabled())
		{
		    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
							 (FacilityCode.FAC_OTS | FacilityCode.FAC_ABSTRACT_REC), "OTS_AbstractRecord: Successfully stringed to object, next try to narrow");
		}
		
		theOrb = null;
		
		_resourceHandle =  com.arjuna.ArjunaOTS.ArjunaSubtranAwareResourceHelper.narrow(optr) ;

		if (DebugController.enabled())
		{
		    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,		    
							 (FacilityCode.FAC_OTS | FacilityCode.FAC_ABSTRACT_REC), "OTS_AbstractRecord: Successfully narrowed");
		}
		
		if (_resourceHandle == null)
		    throw new BAD_PARAM();
		else
		{
		    optr = null;
		}
	    }
	    catch (SystemException e)
	    {
		// Failed to narrow to a ArjunaSubtranAwareResource
		if (DebugController.enabled())
		{
		    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
							 (FacilityCode.FAC_OTS | FacilityCode.FAC_ABSTRACT_REC), "OTS_AbstractRecord: Failed to narrow to ArjunaSubtranAwareResource");
		}
	    }
	}

	return _resourceHandle;
    }
    
public boolean propagateOnCommit ()
    {
	return _propagateRecord;
    }

public boolean propagateOnAbort ()
    {
	return false;
    }

public int typeIs ()
    {
	OTSAbstractRecord resHandle = otsRecord();
	int r = RecordType.OTS_ABSTRACTRECORD;

	try
	{
	    if (resHandle != null)
		r = resHandle.typeId();
	}
	catch (Exception e)
	{
	    r = RecordType.OTS_ABSTRACTRECORD;

	    ErrorStream.stream(ErrorStream.WARNING).println("OTS_AbstractRecord::typeIs failed. Returning default "+r);
	}

	resHandle = null;

	return r;
    }

public ClassName className ()
    {
	return CosTransactionsNames.AbstractRecord_OTS_AbstractRecord();
    }
    
public Object value ()
    {
	return _resourceHandle;
    }

public void setValue (Object o)
    {
	ErrorStream.warning().println("OTS_AbstractRecord::set_value() called illegally");
    }

    /**
     * General nesting rules:
     *
     * Only SubtransactionAware resources get registered with nested actions. 
     * The OTS_AbstractRecord creator is assumed to ensure that plain Resources
     * are only registered with the appropriate top level action.
     *
     * That said the _propagateRecord flag ensures that resources registered
     * via register_subtran only take part in the action they where registered
     * in after which they are dropped.
     */

public int nestedAbort ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 (FacilityCode.FAC_OTS | FacilityCode.FAC_ABSTRACT_REC), "OTS_AbstractRecord::nestedAbort() for "+order());
	}

	/*
	 * Must be an staResource to get here.
	 */

	try
	{
	    resourceHandle().rollback_subtransaction();
	}
	catch (SystemException ex)
	{
	    return PrepareOutcome.FINISH_ERROR;
	}
    
	return PrepareOutcome.FINISH_OK;
    }

public int nestedCommit ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 (FacilityCode.FAC_OTS | FacilityCode.FAC_ABSTRACT_REC), "OTS_AbstractRecord::nestedCommit() for "+order());
	}

	int o = PrepareOutcome.FINISH_ERROR;

	try
	{
	    resourceHandle().commit_subtransaction(_parentCoordHandle);

	    /*
	     * Now release the parent as it is about to be destroyed
	     * anyway.
	     */

	    _parentCoordHandle = null;
    
	    if (_currentTransaction != null)
	    {
		/*
		 * Now change our notion of our parent for subsequent nested
		 * transaction commits. We were passed a reference to the
		 * current transaction, so we can just ask it for it's parent.
		 * If it doesn't have one, then generate an error.
		 */
		
		_currentTransaction = (OTS_Transaction) _currentTransaction.parent();

		OTS_Control control = _currentTransaction.getControlHandle();

		if (control != null)
		{
		    _parentCoordHandle = control.get_coordinator();

		    control = null;

		    o = PrepareOutcome.FINISH_OK;		    
		}
		else
		{
		    ErrorStream.stream(ErrorStream.WARNING).println("OTS_AbstractRecord::nestedCommit - no parent!");
		    o = PrepareOutcome.FINISH_ERROR;
		}
	    }
	    else
		o = PrepareOutcome.FINISH_ERROR;
	}
	catch (Exception e)
	{
	    o = PrepareOutcome.FINISH_ERROR;
	}

	return o;
    }

    /**
     * Because resource is an Arjuna AbstractRecord we can do proper nesting!
     */

public int nestedPrepare ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 (FacilityCode.FAC_OTS | FacilityCode.FAC_ABSTRACT_REC), "OTS_AbstractRecord::nestedPrepare() for "+order());
	}

	int o = PrepareOutcome.PREP_NOTOK;
    
	try
	{
 	    switch ( resourceHandle().prepare_subtransaction().value() )
	    {
	    case  Vote ._VoteCommit  :
		o = PrepareOutcome.PREP_OK;
		break;
	    case  Vote ._VoteRollback  :
		o = PrepareOutcome.PREP_NOTOK;
		break;
	    case  Vote ._VoteReadOnly  :
		o = PrepareOutcome.PREP_READONLY;
		break;
	    }
	}
	catch (Exception e)
	{
	    o = PrepareOutcome.PREP_NOTOK;
	}

	return o;
    }

public int topLevelAbort ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 (FacilityCode.FAC_OTS | FacilityCode.FAC_ABSTRACT_REC), "OTS_AbstractRecord::topLevelAbort() for "+order());
	}

	try
	{
	    if (resourceHandle() != null)
	    {
		_resourceHandle.rollback();
	    }
	    else
		return PrepareOutcome.FINISH_ERROR;
	}
	catch (HeuristicCommit e1)
	{
	    return PrepareOutcome.HEURISTIC_COMMIT;
	}
	catch (HeuristicMixed e2)
	{
	    return PrepareOutcome.HEURISTIC_MIXED;
	}
	catch (HeuristicHazard e3)
	{
	    return PrepareOutcome.HEURISTIC_HAZARD;
	}
	catch (SystemException e4)
	{
	    ErrorStream.warning().println("OTS_ResourceRecord.topLevelAbort caught: "+e4);

	    return PrepareOutcome.FINISH_ERROR;
	}
	
	return PrepareOutcome.FINISH_OK;
    }

public int topLevelCommit ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 (FacilityCode.FAC_OTS | FacilityCode.FAC_ABSTRACT_REC), "OTS_AbstractRecord::topLevelCommit() for "+order());
	}

	try
	{
	    if (resourceHandle() != null)
	    {
		_resourceHandle.commit();
	    }
	    else
		return PrepareOutcome.FINISH_ERROR;
	}
	catch (NotPrepared e1)
	{
	    return PrepareOutcome.NOT_PREPARED;
	}
	catch (HeuristicRollback e2)
	{
	    return PrepareOutcome.HEURISTIC_ROLLBACK;
	}
	catch (HeuristicMixed e3)
	{
	    return PrepareOutcome.HEURISTIC_MIXED;
	}
	catch(HeuristicHazard e4)
	{
	    return PrepareOutcome.HEURISTIC_HAZARD;
	}
	catch (SystemException e5)
	{
	    return PrepareOutcome.FINISH_ERROR;
	}

	return PrepareOutcome.FINISH_OK;
    }

public int topLevelPrepare ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 (FacilityCode.FAC_OTS | FacilityCode.FAC_ABSTRACT_REC), "OTS_AbstractRecord::topLevelPrepare() for "+order());
	}

	try
	{
	    if (resourceHandle() != null)
	    {
		switch ( _resourceHandle.prepare().value() )
		{
		case  Vote ._VoteCommit  :
		    return PrepareOutcome.PREP_OK;
		case  Vote ._VoteRollback  :
		    return PrepareOutcome.PREP_NOTOK;
		case  Vote ._VoteReadOnly  :
		    return PrepareOutcome.PREP_READONLY;
		}
	    }
	    else
		return PrepareOutcome.PREP_NOTOK;
	}
	catch (HeuristicMixed e1)
	{
	    return PrepareOutcome.HEURISTIC_MIXED;
	}
	catch (HeuristicHazard e2)
	{
	    return PrepareOutcome.HEURISTIC_HAZARD;
	}
	catch (Exception e)
	{
	    return PrepareOutcome.PREP_NOTOK;
	}

        return PrepareOutcome.PREP_NOTOK;
    }

public int nestedOnePhaseCommit ()
    {
	switch (nestedPrepare())
	{
	case PrepareOutcome.PREP_OK:
	    return nestedCommit();
	case PrepareOutcome.PREP_READONLY:
	    return PrepareOutcome.FINISH_OK;
	default:
	    return PrepareOutcome.FINISH_ERROR;
	}
    }

public int topLevelOnePhaseCommit ()
    {
	try
	{
	    if (resourceHandle() != null)
		_resourceHandle.commit_one_phase();
	}
	catch (HeuristicHazard e1)
	{
	    return PrepareOutcome.HEURISTIC_HAZARD;
	}

	catch (TRANSACTION_ROLLEDBACK  e4)
	{
	    return PrepareOutcome.FINISH_ERROR;
	}
	catch (INVALID_TRANSACTION  e5)
	{
	    return PrepareOutcome.FINISH_ERROR;
	}
	catch (Exception e5)
	{
	    /*
	     * Unknown error - better assume heuristic!
	     */
	    
	    return PrepareOutcome.HEURISTIC_HAZARD;
	}	

	return PrepareOutcome.FINISH_OK;    
    }

public boolean forgetHeuristic ()
    {
	try
	{
	    if (resourceHandle() != null)
	    {
		_resourceHandle.forget();
		return true;
	    }
	    else
		ErrorStream.stream(ErrorStream.WARNING).println("OTS_AbstractRecord::forgetHeuristic called without a resource!");
	}
	catch (Exception e)
	{
	    ErrorStream.stream(ErrorStream.WARNING).println("OTS_AbstractRecord::forgetHeuristic caught exception: "+e);
	}

	return false;
    }
    
public static AbstractRecord create ()
    {
	return new OTS_AbstractRecord();
    }

public void remove (AbstractRecord toDelete)
    {
	toDelete = null;
    }
    
public void print (PrintWriter strm)
    {
	super.print(strm);

	strm.println("OTS_AbstractRecord");
	strm.println(_resourceHandle+"\t"+_parentCoordHandle+"\t"+_propagateRecord);
    }

    /**
     * restore_state and save_state for OTS_AbstractRecords doesn't generally
     * apply due to object pointers. However, we need to save something so we
     * can recover failed transactions. So, rather than insist that all
     * Resources derive from a class which we can guarantee will give us some
     * unique id, we simply rely on string_to_object and object_to_string to
     * be meaningful.
     */

public boolean restore_state (InputObjectState os, int t)
    {
	int isString = 0;
	boolean result = super.restore_state(os, t);

	if (!result)
	    return false;
	
	try
	{
	    isString = os.unpackInt();

	    /*
	     * Do we need to restore the parent coordinator handle?
	     */

	    _parentCoordHandle = null;
	    
	    if (isString == 1)
	    {
		_stringifiedResourceHandle = os.unpackString();

		/*
		 * Could call resourceHandle() here to restore the
		 * _resourceHandle reference but no loss in doing it
		 * lazily.  
		 */

		// Unpack recovery coordinator Uid

		_recCoordUid.unpack(os);

		if (DebugController.enabled())
		{
		    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
							 (FacilityCode.FAC_OTS | FacilityCode.FAC_ABSTRACT_REC), 
							 "OTS_AbstractRecord.restore_state: unpacked record with uid="+_recCoordUid);
		}
	    }
	    else
		_stringifiedResourceHandle = null;
	}
	catch (IOException e)
	{
	    result = false;
	}

	return result;
    }

    /**
     * restore_state and save_state for OTS_AbstractRecords doesn't generally
     * apply due to object pointers. However, we need to save something so we
     * can recover failed transactions. So, rather than insist that all
     * Resources derive from a class which we can guarantee will give us some
     * unique id, we simply rely on string_to_object and object_to_string to
     * be meaningful.
     */

public boolean save_state (OutputObjectState os, int t)
    {
	boolean result = super.save_state(os, t);

	if (!result)
	    return false;
	
	try
	{	
	    /*
	     * Do we need to save the parent coordinator handle?
	     */

	    /*
	     * If we have a _resourceHandle then we stringify it and
	     * pack the string.  Failing that if we have a cached
	     * stringified version (in _stringifiedResourceHandle)
	     * then we pack that. If we have neither then we're
	     * doomed.  
	     */

	    if ( (_resourceHandle == null) && (_stringifiedResourceHandle == null) )
	    {
		os.packInt(-1);
	    }
	    else
	    {
		os.packInt(1);
		String stringRef = null;

		if (_resourceHandle != null)
		{
		    org.omg.CORBA.ORB theOrb = ORBInterface.orb();

		    if (theOrb == null)
			throw new UNKNOWN();
		
		    stringRef = theOrb.object_to_string(_resourceHandle);

		    theOrb = null;
		}
		else
		{
		    stringRef = _stringifiedResourceHandle;
		}

		if (stringRef != null)
		{
		    os.packString(stringRef);

		    if (DebugController.enabled())
		    {
			DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
							     (FacilityCode.FAC_OTS | FacilityCode.FAC_ABSTRACT_REC), 
							     "OTS_AbstractRecord: packed obj ref "+stringRef);
		    }
		}
		else
		{
		    result = false;
		}
		
		stringRef = null;
		
		if (result)
		{
		    // Pack recovery coordinator Uid
		    _recCoordUid.pack(os);
		    
		    if (DebugController.enabled())
		    {
			DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
							     (FacilityCode.FAC_OTS | FacilityCode.FAC_ABSTRACT_REC), 
							     "Packed rec co uid of "+_recCoordUid);
		    }
		}
	    }
	}
	catch (IOException e)
	{
	    result = false;
	}
	catch (SystemException e)
	{
	    result = false;
	}
	
	return result;
    }

public String type ()
    {
	return "/StateManager/AbstractRecord/OTS_AbstractRecord";
    }

public boolean doSave ()
    {
	OTSAbstractRecord resHandle = otsRecord();
	boolean save = true;

	try
	{
	    if (resHandle != null)
		save = resHandle.saveRecord();
	}
	catch (Exception e)
	{
	    save = true;  // just to be on the safe side!

	    ErrorStream.warning().println("OTS_AbstractRecord::doSave failed. Returning default "+save);
	}

	resHandle = null;

	return save;
    }

public final Uid getRCUid ()
    {
	return _recCoordUid;
    }

public void merge ( AbstractRecord absRec )
    {
	OTSAbstractRecord resHandle = otsRecord();

	if (resHandle != null)
	{
	    OTSAbstractRecord rec = otsRecord(absRec);

	    if (rec != null)
	    {
		try
		{
		    resHandle.merge(rec);
		}
		catch (Exception e)
		{
		    ErrorStream.stream(ErrorStream.WARNING).println("OTS_AbstractRecord::merge caught exception");
		}

		rec = null;
	    }
	}

	resHandle = null;
    }

public void alter (AbstractRecord absRec)
    {
	OTSAbstractRecord resHandle = otsRecord();

	if (resHandle != null)
	{
	    OTSAbstractRecord rec = otsRecord(absRec);

	    if (rec != null)
	    {
		try
		{
		    resHandle.alter(rec);
		}
		catch (Exception e)
		{
		    ErrorStream.stream(ErrorStream.WARNING).println("OTS_AbstractRecord::alter caught exception");
		}

		rec = null;
	    }
	}

	resHandle = null;
    }

public boolean shouldAdd ( AbstractRecord absRec )
    {
	boolean result = false;
	OTSAbstractRecord resHandle = otsRecord();

	if (resHandle != null)
	{
	    OTSAbstractRecord rec = otsRecord(absRec);

	    if (rec != null)
	    {
		try
		{
		    result = resHandle.shouldAdd(rec);
		}
		catch (Exception e)
		{
		    ErrorStream.stream(ErrorStream.WARNING).println("OTS_AbstractRecord::shouldAdd caught exception");
		}

		rec = null;
	    }
	}

	resHandle = null;

	return result;
    }

public boolean shouldAlter (AbstractRecord absRec)
    {
	boolean result = false;
	OTSAbstractRecord resHandle = otsRecord();

	if (resHandle != null)
	{
	    OTSAbstractRecord rec = otsRecord(absRec);

	    if (rec != null)
	    {
		try
		{
		    result = resHandle.shouldAlter(rec);
		}
		catch (Exception e)
		{
		    ErrorStream.stream(ErrorStream.WARNING).println("OTS_AbstractRecord::shouldAlter caught exception");
		}

		rec = null;
	    }
	}

	resHandle = null;

	return result;
    }

public boolean shouldMerge ( AbstractRecord absRec )
    {
	boolean result = false;
	OTSAbstractRecord resHandle = otsRecord();

	if (resHandle != null)
	{
	    OTSAbstractRecord rec = otsRecord(absRec);

	    if (rec != null)
	    {
		try
		{
		    result = resHandle.shouldMerge(rec);
		}
		catch (Exception e)
		{
		    ErrorStream.stream(ErrorStream.WARNING).println("OTS_AbstractRecord::shouldMerge caught exception");
		}

		rec = null;
	    }
	}

	resHandle = null;

	return result;
    }

public boolean shouldReplace ( AbstractRecord absRec )
    {
	boolean result = false;
	OTSAbstractRecord resHandle = otsRecord();

	if (resHandle != null)
	{
	    OTSAbstractRecord rec = otsRecord(absRec);

	    if (rec != null)
	    {
		try
		{
		    result = resHandle.shouldReplace(rec);
		}
		catch (Exception e)
		{
		    ErrorStream.stream(ErrorStream.WARNING).println("OTS_AbstractRecord::shouldReplace caught exception");
		}

		rec = null;
	    }
	}

	resHandle = null;

	return result;
    }

    /**
     * Protected constructor used by crash recovery.
     */

protected OTS_AbstractRecord ()
    {
	super();

	_resourceHandle = null;
	_stringifiedResourceHandle = null;
	_recCoordUid = Uid.nullUid();
	_parentCoordHandle = null;
	_currentTransaction = null;
	_propagateRecord = false;
    }
    
private final OTSAbstractRecord otsRecord (AbstractRecord absRec)
    {
	/*
	 * Is the abstract record an OTS_AbstractRecord ?
	 */
    
	OTSAbstractRecord resHandle = null;

	if ((absRec != null) && (absRec instanceof OTS_AbstractRecord))
	{
	    if (_resourceHandle != null)
	    {
		try
		{
		    OTS_AbstractRecord theRecord = (OTS_AbstractRecord) absRec;
		    ArjunaSubtranAwareResource theResource = theRecord.resourceHandle();

		    resHandle =  com.arjuna.ArjunaOTS.OTSAbstractRecordHelper.narrow(theResource) ;

		    theResource = null;
		}
		catch (Exception e)
		{
		    ErrorStream.stream(ErrorStream.WARNING).println("OTS_AbstractRecord::otsRecord caught exception");
		}
	    }
	}

	return resHandle;
    }

private final OTSAbstractRecord otsRecord ()
    {
	OTSAbstractRecord resHandle = null;

	try
	{
	    resHandle =  com.arjuna.ArjunaOTS.OTSAbstractRecordHelper.narrow(_resourceHandle) ;

	    if (resHandle == null)
		throw new BAD_PARAM();
	}
	catch (Exception e)
	{
	    // we are not an OTSAbstractRecord

	    resHandle = null;
	}
    
	return resHandle;  // already duplicated
    }

private Coordinator                _parentCoordHandle;
private ArjunaSubtranAwareResource _resourceHandle;
private String                     _stringifiedResourceHandle;
private Uid                        _recCoordUid;
private OTS_Transaction            _currentTransaction;
private boolean                    _propagateRecord;
 
};
