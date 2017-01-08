/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CrashRecoveryRecord.java,v 1.3 1998/07/06 10:49:56 nmcl Exp $
 */

package JavaCosRecoveryTests;

import com.arjuna.JavaGandiva.Common.*;
import com.arjuna.JavaArjunaLite.Atomic.AbstractRecord;
import com.arjuna.JavaArjunaLite.Atomic.RecordType;
import com.arjuna.JavaArjunaLite.Atomic.PrepareOutcome;
import java.io.PrintStream;

public class CrashRecoveryRecord extends AbstractRecord
{
    
public CrashRecoveryRecord ()
    {
	super();
    }

public void finalize ()
    {
    }

    /*
     * Redefintions of public virtual functions inherited from RecoveryRecord
     */

public int typeIs ()
    {
	return RecordType.USER_DEF_LAST9;
    }

public ClassName className ()
    {
	return new ClassName("CrashRecoveryRecord");
    }
    
public Object value ()
    {
	return null;
    }
    
public void setValue (Object o)
    {
    }

public int nestedAbort ()
    {
	return PrepareOutcome.FINISH_OK;
    }
    
public int nestedCommit ()
    {
	return PrepareOutcome.FINISH_OK;	
    }
    
public int nestedPrepare ()
    {
	return PrepareOutcome.PREP_OK;
    }
    
public int topLevelAbort ()
    {
	return PrepareOutcome.FINISH_OK;
    }

public int topLevelCommit ()
    {
	System.out.println("\nATOMIC ACTION "+get_uid()+" PREPARED OK -- ");
	System.exit(1);

	return PrepareOutcome.FINISH_OK;	
    }

public int topLevelPrepare ()
    {
	return PrepareOutcome.PREP_OK;
    }

public int topLevelCleanup ()
    {
	return PrepareOutcome.FINISH_OK;
    }

public boolean doSave ()
    {
	return false;
    }
    
public boolean restore_state (InputObjectState os, int ot)
    {
	return true;
    }

public boolean save_state (OutputObjectState os, int ot)
    {
	return true;
    }

public void print (PrintStream strm)
    {
	super.print(strm);	/* bypass RecoveryRecord */

	strm.println("CrashRecoveryRecord.");
    }

public String type ()
    {
	return "/StateManager/AbstractRecord/CrashRecoveryRecord";
    }

public AbstractRecord create ()
    {
	return new CrashRecoveryRecord();
    }

public void remove (AbstractRecord toDelete)
    {
	toDelete = null;
    }

public void merge (AbstractRecord a)
    {
    }
    
public void alter (AbstractRecord a)
    {
    }
    
public boolean shouldAdd (AbstractRecord a)
    {
	return false;
    }
    
public boolean shouldAlter (AbstractRecord a)
    {
	return false;
    }
    
public boolean shouldMerge (AbstractRecord a)
    {
	return false;
    }
    
public boolean shouldReplace (AbstractRecord a)
    {
	return false;
    }
    
}
