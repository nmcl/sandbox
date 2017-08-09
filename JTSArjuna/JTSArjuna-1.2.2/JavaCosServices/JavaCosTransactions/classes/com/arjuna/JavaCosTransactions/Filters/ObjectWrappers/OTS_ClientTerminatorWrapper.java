/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OTS_ClientTerminatorWrapper.java,v 1.2 1998/07/06 13:29:10 nmcl Exp $
 */

package com.arjuna.JavaCosTransactions.Filters.ObjectWrappers;

import org.omg.CosTransactions.*;
import com.arjuna.ArjunaOTS.*;
import com.arjuna.JavaCosTransactions.OTS;
import com.arjuna.JavaCosTransactions.OTS_Current;

public class OTS_ClientTerminatorWrapper extends TerminatorObjectWrapper
{
    
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
