/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OTS_FactoryCreator.java,v 1.2 1998/07/06 13:28:40 nmcl Exp $
 */

package com.arjuna.JavaCosTransactions;

import org.omg.CosTransactions.Control;
import org.omg.CosTransactions.PropagationContext;

public interface OTS_FactoryCreator
{

public Control recreate (PropagationContext ctx);
 
};
