/*
 * Copyright (C) 1996, 1997, 1998, 1999,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OTS_ClientOTSAbstractRecordWrapper.java,v 1.1 1999/09/10 15:04:08 nmcl Exp $
 */

package com.arjuna.JavaCosTransactions.Filters.ObjectWrappers.vbroker;

import org.omg.CosTransactions.*;
import com.arjuna.ArjunaOTS.*;
import com.arjuna.JavaCosTransactions.OTS;
import com.arjuna.JavaCosTransactions.OTS_Current;

public class OTS_ClientOTSAbstractRecordWrapper extends OTSAbstractRecordObjectWrapper
{

public boolean equals (OTSAbstractRecord r) throws SystemException
    {
	Control theControl = OTS.get_current().suspend();
	boolean res = super.equals(r);

	OTS.get_current().resume(theControl);

	return res;
    }

public boolean lessThan (OTSAbstractRecord r) throws SystemException
    {
	Control theControl = OTS.get_current().suspend();
	boolean res = super.lessThan(r);

	OTS.get_current().resume(theControl);

	return res;
    }

public boolean greaterThan (OTSAbstractRecord r) throws SystemException
    {
	Control theControl = OTS.get_current().suspend();
	boolean res = super.greaterThan(r);

	OTS.get_current().resume(theControl);

	return res;
    }    
    
public void merge (OTSAbstractRecord r) throws SystemException
    {
	Control theControl = OTS.get_current().suspend();
	
	super.merge(r);

	OTS.get_current().resume(theControl);
    }

public void alter (OTSAbstractRecord r) throws SystemException
    {
	Control theControl = OTS.get_current().suspend();
	
	super.alter(r);

	OTS.get_current().resume(theControl);
    }    

public boolean shouldAdd (OTSAbstractRecord r) throws SystemException
    {
	Control theControl = OTS.get_current().suspend();
	boolean res = super.shouldAdd(r);

	OTS.get_current().resume(theControl);

	return res;
    }

public boolean shouldAlter (OTSAbstractRecord r) throws SystemException
    {
	Control theControl = OTS.get_current().suspend();
	boolean res = super.shouldAlter(r);

	OTS.get_current().resume(theControl);

	return res;
    }

public boolean shouldMerge (OTSAbstractRecord r) throws SystemException
    {
	Control theControl = OTS.get_current().suspend();
	boolean res = super.shouldMerge(r);

	OTS.get_current().resume(theControl);

	return res;
    }

public boolean shouldReplace (OTSAbstractRecord r) throws SystemException
    {
	Control theControl = OTS.get_current().suspend();
	boolean res = super.shouldReplace(r);

	OTS.get_current().resume(theControl);

	return res;
    }
    
public int type () throws SystemException
    {
	Control theControl = OTS.get_current().suspend();
	int t = super.type();

	OTS.get_current().resume(theControl);

	return t;
    }
    
public Vote prepare_subtransaction () throws SystemException
    {
	Control theControl = OTS.get_current().suspend();
	Vote v = super.prepare_subtransaction();

	OTS.get_current().resume(theControl);

	return v;
    }
    
public void commit_subtransaction (Coordinator parent) throws SystemException
    {
	Control theControl = OTS.get_current().suspend();

	super.commit_subtransaction(parent);

	OTS.get_current().resume(theControl);
    }

public void rollback_subtransaction () throws SystemException
    {
	Control theControl = OTS.get_current().suspend();

	super.rollback_subtransaction();

	OTS.get_current().resume(theControl);
    }    

public Vote prepare () throws HeuristicMixed, HeuristicHazard, SystemException
    {
	Control theControl = OTS.get_current().suspend();
	Vote v = super.prepare();

	OTS.get_current().resume(theControl);

	return v;
    }

public void rollback () throws HeuristicCommit, HeuristicMixed, HeuristicHazard, SystemException
    {
	Control theControl = OTS.get_current().suspend();

	super.rollback();

	OTS.get_current().resume(theControl);
    }    

public void commit () throws NotPrepared, HeuristicRollback, HeuristicMixed, HeuristicHazard, SystemException
    {
	Control theControl = OTS.get_current().suspend();

	super.commit();

	OTS.get_current().resume(theControl);
    }    

public void commit_one_phase () throws HeuristicHazard, SystemException
    {
	Control theControl = OTS.get_current().suspend();

	super.commit_one_phase();

	OTS.get_current().resume(theControl);
    }

public void forget () throws SystemException
    {
	Control theControl = OTS.get_current().suspend();

	super.forget();

	OTS.get_current().resume(theControl);
    }    

};
