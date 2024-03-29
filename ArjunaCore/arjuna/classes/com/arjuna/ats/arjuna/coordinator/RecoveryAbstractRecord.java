/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: RecoveryAbstractRecord.java,v 1.1 2003/06/19 10:50:17 nmcl Exp $
 */

package com.arjuna.ats.arjuna.coordinator;

import com.arjuna.ats.arjuna.common.*;
import com.arjuna.ats.internal.arjuna.Implementations;
import com.arjuna.ats.arjuna.gandiva.*;
import com.arjuna.ats.arjuna.gandiva.inventory.Inventory;
import com.arjuna.ats.arjuna.state.*;
import java.io.PrintWriter;

import java.io.IOException;

/**
 * Abstract Record Interface Class.
 *
 * Used only for crash recovery. This interface allows us to use the
 * Inventory for creating abstract records during crash recovery without
 * having to make all abstract records accessible via interface/implementation
 * separation (which affects the performance.)
 *
 * If performance issues can be ironed-out, then this may become obsolete in
 * favour of a true AbstractRecord interface.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: RecoveryAbstractRecord.java,v 1.1 2003/06/19 10:50:17 nmcl Exp $
 * @since JTS 1.0.
 */

public class RecoveryAbstractRecord extends AbstractRecord
{

public RecoveryAbstractRecord (ClassName cName)
    {
	Object ptr = Inventory.inventory().createVoid(cName);
		
	if (ptr instanceof AbstractRecord)
	    _imple = (AbstractRecord) ptr;
	else
	    _imple = null;
    }

public void finalize ()
    {
	_imple = null;
    }
    
public int typeIs ()
    {
	return ((_imple == null) ? RecordType.NONE_RECORD : _imple.typeIs());
    }
    
public ClassName className ()
    {
	return ((_imple == null) ? ClassName.invalid() : _imple.className());
    }

    /**
     * @return the actual AbstractRecord instance.
     */

public AbstractRecord record ()
    {
	return _imple;
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

    /**
     * Atomic action interface - one operation per two-phase commit state.
     */

public int nestedAbort ()
    {
	return ((_imple == null) ? TwoPhaseOutcome.FINISH_ERROR : _imple.nestedAbort());
    }
    
public int nestedCommit ()
    {
	return ((_imple == null) ? TwoPhaseOutcome.FINISH_ERROR : _imple.nestedCommit());
    }
    
public int nestedPrepare ()
    {
	return ((_imple == null) ? TwoPhaseOutcome.PREPARE_NOTOK : _imple.nestedPrepare());
    }
    
public int topLevelAbort ()
    {
	return ((_imple == null) ? TwoPhaseOutcome.FINISH_ERROR : _imple.topLevelAbort());
    }
    
public int topLevelCommit ()
    {
	return ((_imple == null) ? TwoPhaseOutcome.FINISH_ERROR : _imple.topLevelCommit());
    }
	
public int topLevelPrepare ()
    {
	return ((_imple == null) ? TwoPhaseOutcome.PREPARE_NOTOK : _imple.topLevelPrepare());
    }

public Uid order ()
    {
	return ((_imple == null) ? Uid.nullUid() : _imple.order());
    }

public String getTypeOfObject ()
    {
	return ((_imple == null) ? null : _imple.getTypeOfObject());
    }

    /**
     * @return whether or not records are discarded on action abort or must be
     * propagated to parents
     */

public boolean propagateOnAbort ()
    {
	return ((_imple == null) ? false : _imple.propagateOnAbort());
    }

    /**
     * @return whether or not records are discarded on action commit or must be
     * propagated to parents
     */

public boolean propagateOnCommit ()
    {
	return ((_imple == null) ? true : _imple.propagateOnCommit());
    }
    
    /**
     * Operators for comparing and sequencing instances of classes derived
     * from AbstractRecords.
     * Records are ordered primarily based upon the value of 'order',
     * followed by 'typeIs'.
     */

    /**
     * Cleanup is called if a top-level action is detected to be an orphan.
     * NOTE nested actions are never orphans since their parents would
     * be aborted we may as well abort them as well.
     */

public int topLevelCleanup ()
    {
	return ((_imple == null) ? TwoPhaseOutcome.FINISH_ERROR : _imple.topLevelCleanup());
    }

public boolean doSave ()
    {
	return ((_imple == null) ? false : _imple.doSave());
    }

    /**
     * Re-implementation of abstract methods inherited from base class
     */

public String type ()
    {
	return ((_imple == null) ? null : _imple.type());
    }

public void print (PrintWriter strm)
    {
	if (_imple != null)
	    _imple.print(strm);
    }

    /**
     * save_state and restore_state here are used by the corresponding
     * operations of persistence records for crash recovery purposes.
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
    
public boolean forgetHeuristic ()
    {
	return ((_imple == null) ? false : _imple.forgetHeuristic());
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

    static 
    {
	if (!Implementations.added())
	    Implementations.initialise();
    }

}


