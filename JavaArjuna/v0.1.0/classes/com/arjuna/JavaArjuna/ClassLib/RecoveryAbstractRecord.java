/*
 * Copyright (C) 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RecoveryAbstractRecord.java,v 1.3 1998/07/06 13:26:53 nmcl Exp $
 */

package com.arjuna.JavaArjuna.ClassLib;

import com.arjuna.JavaArjuna.Common.LockStoreException;
import com.arjuna.JavaArjuna.Common.*;
import java.io.PrintStream;

import java.io.IOException;

/*
 * Abstract Record Interface Class.
 *
 * Used only for crash recovery. This interface allows us to use the
 * Inventory for creating abstract records during crash recovery without
 * having to make all abstract records accessible via interface/implementation
 * separation (which affects the performance.)
 *
 * If performance issues can be ironed-out, then this may become obsolete in
 * favour of a true AbstractRecord interface.
 */

public class RecoveryAbstractRecord extends AbstractRecord
{

public RecoveryAbstractRecord (int type)
    {
	_imple = RecordType.create(type);
    }
    
public void finalize ()
    {
	_imple = null;
    }
    
public int typeIs ()
    {
	return ((_imple == null) ? RecordType.NONE_RECORD : _imple.typeIs());
    }
    
public String className ()
    {
	return ((_imple == null) ? null : _imple.className());
    }
    
public Object value ()
    {
	return ((_imple == null) ? null : _imple.value());
    }
    
public void setValue (Object o)
    {
	if (_imple != null)
	    _imple.setValue(o);
    }

/* atomic action interface - one operation per two-phase commit state */

public int nestedAbort ()
    {
	return ((_imple == null) ? PrepareOutcome.FINISH_ERROR : _imple.nestedAbort());
    }
    
public int nestedCommit ()
    {
	return ((_imple == null) ? PrepareOutcome.FINISH_ERROR : _imple.nestedCommit());
    }
    
public int nestedPrepare ()
    {
	return ((_imple == null) ? PrepareOutcome.PREP_NOTOK : _imple.nestedPrepare());
    }
    
public int topLevelAbort ()
    {
	return ((_imple == null) ? PrepareOutcome.FINISH_ERROR : _imple.topLevelAbort());
    }
    
public int topLevelCommit ()
    {
	return ((_imple == null) ? PrepareOutcome.FINISH_ERROR : _imple.topLevelCommit());
    }
	
public int topLevelPrepare ()
    {
	return ((_imple == null) ? PrepareOutcome.PREP_NOTOK : _imple.topLevelPrepare());
    }

public Uid order ()
    {
	return ((_imple == null) ? Uid.nullUid() : _imple.order());
    }

public String getTypeOfObject ()
    {
	return ((_imple == null) ? null : _imple.getTypeOfObject());
    }

    /*
     * Determine if records are discarded on action abort or must be
     * propagated to parents
     */

public boolean propagateOnAbort ()
    {
	return ((_imple == null) ? false : _imple.propagateOnAbort());
    }

    /*
     * Determine if records are discarded on action commit or must be
     * propagated to parents
     */

public boolean propagateOnCommit ()
    {
	return ((_imple == null) ? true : _imple.propagateOnCommit());
    }
    
    /*
     * Operators for comparing and sequencing instances of classes derived
     * from AbstractRecords.
     * Records are ordered primarily based upon the value of 'order',
     * followed by 'typeIs'.
     */

    /*
     * Determine if two records are equal in that both are the same type
     * and have the same order value (determined via 'order()').
     */
    
public boolean equals (AbstractRecord ar)
    {
	return ((_imple == null) ? false : _imple.equals(ar));
    }

public boolean lessThan (AbstractRecord ar)
    {
	return ((_imple == null) ? false : _imple.lessThan(ar));	
    }

public boolean greaterThan (AbstractRecord ar)
    {
	return ((_imple == null) ? false : _imple.greaterThan(ar));	
    }

    /*
     * Cleanup is called if a top-level action is detected to be an orphan
     * NOTE nested actions are never orphans since their parents would
     * be aborted we may as well abort them as well
     */

public int topLevelCleanup ()
    {
	return ((_imple == null) ? PrepareOutcome.FINISH_ERROR : _imple.topLevelCleanup());
    }

public boolean doSave ()
    {
	return ((_imple == null) ? false : _imple.doSave());
    }

    /*
     * Re-Implementation of virtual functions inherited from base class
     */

public String type ()
    {
	return ((_imple == null) ? null : _imple.type());
    }

public void print (PrintStream strm)
    {
	if (_imple != null)
	    _imple.print(strm);
    }

    /*
     * save_state and restore_state here are used by the corresponding operations
     * of persistence records for crash recovery purposes.
     */

public boolean restore_state (InputObjectState os, int i)
    {
	return ((_imple == null) ? false : _imple.restore_state(os, i));
    }

public boolean save_state (OutputObjectState os, int i)
    {
	return ((_imple == null) ? false : _imple.save_state(os, i));
    }
    
    /*
     * OTS specific methods which we cannot hide!
     */
    
public void forgetHeuristic ()
    {
	if (_imple != null)
	    _imple.forgetHeuristic();
    }

public void merge (AbstractRecord a)
    {
	if (_imple != null)
	    _imple.merge(a);
    }
    
public void alter (AbstractRecord a)
    {
	if (_imple != null)
	    _imple.alter(a);
    }

public boolean shouldAdd (AbstractRecord a)
    {
	return ((_imple == null) ? false : _imple.shouldAdd(a));
    }
    
public boolean shouldAlter (AbstractRecord a)
    {
	return ((_imple == null) ? false : _imple.shouldAlter(a));
    }
    
public boolean shouldMerge (AbstractRecord a)
    {
	return ((_imple == null) ? false : _imple.shouldMerge(a));
    }
    
public boolean shouldReplace (AbstractRecord a)
    {
	return ((_imple == null) ? false : _imple.shouldReplace(a));
    }

private AbstractRecord _imple;

};


