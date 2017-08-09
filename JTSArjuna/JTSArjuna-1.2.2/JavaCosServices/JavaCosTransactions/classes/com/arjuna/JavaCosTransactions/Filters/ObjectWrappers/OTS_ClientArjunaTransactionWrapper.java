/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OTS_ClientArjunaTransactionWrapper.java,v 1.2 1998/07/06 13:29:05 nmcl Exp $
 */

package com.arjuna.JavaCosTransactions.Filters.ObjectWrappers;

import org.omg.CosTransactions.*;
import com.arjuna.ArjunaOTS.*;
import com.arjuna.JavaCosTransactions.OTS;
import com.arjuna.JavaCosTransactions.OTS_Current;

public class OTS_ClientArjunaTransactionWrapper extends ArjunaTransactionObjectWrapper
{

public String uid () throws SystemException
    {
	Control theControl = OTS.get_current().suspend();
	String s = super.uid();

	OTS.get_current().resume(theControl);

	return s;
    }

public String topLevelUid () throws SystemException
    {
	Control theControl = OTS.get_current().suspend();
	String s = super.topLevelUid();

	OTS.get_current().resume(theControl);

	return s;
    }    
	
public Status get_status () throws SystemException
    {
	Control theControl = OTS.get_current().suspend();
	Status status = super.get_status();

	OTS.get_current().resume(theControl);

	return status;
    }
    
public Status get_parent_status () throws SystemException
    {
	Control theControl = OTS.get_current().suspend();
	Status status = super.get_parent_status();

	OTS.get_current().resume(theControl);

	return status;
    }

public Status get_top_level_status () throws SystemException
    {
	Control theControl = OTS.get_current().suspend();
	Status status = super.get_top_level_status();

	OTS.get_current().resume(theControl);

	return status;
    }

public boolean is_same_transaction (Coordinator tc) throws SystemException
    {
	Control theControl = OTS.get_current().suspend();
	boolean res = super.is_same_transaction(tc);

	OTS.get_current().resume(theControl);

	return res;
    }

public boolean is_related_transaction (Coordinator tc) throws SystemException
    {
	Control theControl = OTS.get_current().suspend();
	boolean res = super.is_related_transaction(tc);

	OTS.get_current().resume(theControl);

	return res;
    }    

public boolean is_ancestor_transaction (Coordinator tc) throws SystemException
    {
	Control theControl = OTS.get_current().suspend();
	boolean res = super.is_ancestor_transaction(tc);

	OTS.get_current().resume(theControl);

	return res;
    }

public boolean is_descendant_transaction (Coordinator tc) throws SystemException
    {
	Control theControl = OTS.get_current().suspend();
	boolean res = super.is_descendant_transaction(tc);

	OTS.get_current().resume(theControl);

	return res;
    }

public boolean is_top_level_transaction () throws SystemException
    {
	Control theControl = OTS.get_current().suspend();
	boolean res = super.is_top_level_transaction();

	OTS.get_current().resume(theControl);

	return res;
    }

public int hash_transaction () throws SystemException
    {
	Control theControl = OTS.get_current().suspend();
	int h = super.hash_transaction();

	OTS.get_current().resume(theControl);

	return h;
    }    

public int hash_top_level_tran () throws SystemException
    {
	Control theControl = OTS.get_current().suspend();
	int h = super.hash_top_level_tran();

	OTS.get_current().resume(theControl);

	return h;
    }

public RecoveryCoordinator register_resource (Resource r) throws Inactive, SystemException
    {
	Control theControl = OTS.get_current().suspend();
	RecoveryCoordinator rc = super.register_resource(r);

	OTS.get_current().resume(theControl);

	return rc;
    }

public void register_synchronization (Synchronization sync) throws Inactive, SynchronizationUnavailable, SystemException
    {
	Control theControl = OTS.get_current().suspend();
	
	super.register_synchronization(sync);

	OTS.get_current().resume(theControl);
    }

public void register_subtran_aware (SuntransactionAwareResource r) throws Inactive, NotSubtransaction, SystemException
    {
	Control theControl = OTS.get_current().suspend();
	
	super.register_subtran_aware(r);

	OTS.get_current().resume(theControl);
    }

public void rollback_only () throws Inactive, SystemException
    {
	Control theControl = OTS.get_current().suspend();
	
	super.rollback_only();

	OTS.get_current().resume(theControl);
    }

public String get_transaction_name () throws SystemException
    {
	Control theControl = OTS.get_current().suspend();
	String s = super.get_transaction_name();

	OTS.get_current().resume(theControl);

	return s;
    }

public Control create_subtransaction () throws SubtransactionsUnavailable, Inactive, SystemException
    {
	Control theControl = OTS.get_current().suspend();
	Control c = super.create_subtransaction();

	OTS.get_current().resume(theControl);

	return c;
    }

public PropagationContext get_txcontext () throws Inactive, SystemException
    {
	Control theControl = OTS.get_current().suspend();
	PropagationContext ctx = super.get_txcontext();

	OTS.get_current().resume(theControl);

	return ctx;
    }
    
public void commit (boolean report_heuristics) throws HeuristicMixed, HeuristicHazard, SystemException
    {
	Control theControl = OTS.get_current().suspend();

	super.commit(report_heuristics);

	OTS.get_current().resume(theControl);
    }

public void rollback () throws SystemException
    {
	Control theControl = OTS.get_current().suspend();

	super.rollback();

	OTS.get_current().resume(theControl);
    }

};
