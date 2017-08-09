/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OTS_ClientResourceWrapper.java,v 1.2 1998/07/06 13:29:08 nmcl Exp $
 */

package com.arjuna.JavaCosTransactions.Filters.ObjectWrappers;

import org.omg.CosTransactions.*;
import com.arjuna.ArjunaOTS.*;
import com.arjuna.JavaCosTransactions.OTS;
import com.arjuna.JavaCosTransactions.OTS_Current;

public class OTS_ClientResourceWrapper extends ResourceObjectWrapper
{

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
