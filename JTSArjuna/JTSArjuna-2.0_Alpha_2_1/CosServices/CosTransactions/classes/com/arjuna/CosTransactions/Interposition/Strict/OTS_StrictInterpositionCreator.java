/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_StrictInterpositionCreator.java,v 1.1 2000/02/25 14:06:32 nmcl Exp $
 */

package com.arjuna.CosTransactions.Interposition.Strict;

import com.arjuna.CosTransactions.Interposition.OTS_FactoryCreator;
import org.omg.CosTransactions.PropagationContext;
import org.omg.CosTransactions.Control;

public class OTS_StrictInterpositionCreator implements OTS_FactoryCreator
{

public Control recreate (PropagationContext ctx)
    {
	return OTS_StrictInterposition.create(ctx);
    }

};
