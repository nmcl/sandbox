/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_FactoryCreator.java,v 1.1 2000/02/25 14:06:17 nmcl Exp $
 */

package com.arjuna.CosTransactions.Interposition;

import org.omg.CosTransactions.Control;
import org.omg.CosTransactions.PropagationContext;

public interface OTS_FactoryCreator
{

public Control recreate (PropagationContext ctx);
 
};
