/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_RestrictedInterpositionCreator.java,v 1.1.26.1 2000/12/04 12:12:09 nmcl Exp $
 */

package com.arjuna.CosTransactions.Interposition.Restricted;

import com.arjuna.CosTransactions.OTS_Control;
import com.arjuna.CosTransactions.Interposition.OTS_FactoryCreator;
import org.omg.CosTransactions.PropagationContext;
import org.omg.CosTransactions.Control;

import org.omg.CORBA.SystemException;

public class OTS_RestrictedInterpositionCreator implements OTS_FactoryCreator
{

public OTS_Control recreateLocal (PropagationContext ctx) throws SystemException
    {
	return OTS_RestrictedInterposition.create(ctx);
    }

public Control recreate (PropagationContext ctx) throws SystemException
    {
	return recreateLocal(ctx).getControl();
    }

};
