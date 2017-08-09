/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OTS_InterpositionCreator.java,v 1.2 1998/07/06 13:29:14 nmcl Exp $
 */

package com.arjuna.JavaCosTransactions.Interposition.Arjuna;

import com.arjuna.JavaCosTransactions.OTS_FactoryCreator;
import org.omg.CosTransactions.PropagationContext;
import org.omg.CosTransactions.Control;

public class OTS_InterpositionCreator implements OTS_FactoryCreator
{

public Control recreate (PropagationContext ctx)
    {
	return OTS_Interposition.create(ctx);
    }

};
