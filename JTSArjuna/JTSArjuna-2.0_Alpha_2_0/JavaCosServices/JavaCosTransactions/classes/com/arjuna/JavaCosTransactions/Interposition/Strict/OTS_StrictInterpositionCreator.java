/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OTS_StrictInterpositionCreator.java,v 1.2 1998/07/06 13:29:19 nmcl Exp $
 */

package com.arjuna.JavaCosTransactions.Interposition.Strict;

import com.arjuna.JavaCosTransactions.OTS_FactoryCreator;
import org.omg.CosTransactions.PropagationContext;
import org.omg.CosTransactions.Control;

public class OTS_StrictInterpositionCreator implements OTS_FactoryCreator
{

public Control recreate (PropagationContext ctx)
    {
	return OTS_StrictInterposition.create(ctx);
    }

};
