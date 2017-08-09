/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OTS_ClientTransactionFactoryWrapper.java,v 1.2 1998/07/06 13:29:10 nmcl Exp $
 */

package com.arjuna.JavaCosTransactions.Filters.ObjectWrappers;

import org.omg.CosTransactions.*;
import com.arjuna.ArjunaOTS.*;
import com.arjuna.JavaCosTransactions.OTS;
import com.arjuna.JavaCosTransactions.OTS_Current;

public class OTS_ClientTransactionFactoryWrapper extends TransactionFactoryObjectWrapper
{
    
public Control create (int time_out) throws SystemException
    {
	Control theControl = OTS.get_current().suspend();
	Control c = super.create(time_out);

	OTS.get_current().resume(theControl);

	return c;
    }

public Control recreate (PropagationContext ctx) throws SystemException
    {
	Control theControl = OTS.get_current().suspend();
	Control c = super.recreate(ctx);

	OTS.get_current().resume(theControl);

	return c;
    }    

};
