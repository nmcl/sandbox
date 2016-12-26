/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_FactoryCreator.java,v 1.1.30.1.2.1.2.1 2000/12/21 11:21:20 nmcl Exp $
 */

package com.arjuna.CosTransactions.Interposition;

import com.arjuna.CosTransactions.OTS_Control;
import org.omg.CosTransactions.Control;
import org.omg.CosTransactions.PropagationContext;

import org.omg.CORBA.SystemException;

/**
 * Different transaction factories can be added dynamically
 * to the system to deal with specific interposition types.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: OTS_FactoryCreator.java,v 1.1.30.1.2.1.2.1 2000/12/21 11:21:20 nmcl Exp $
 * @since JTS 1.0.
 */

public interface OTS_FactoryCreator
{

public OTS_Control recreateLocal (PropagationContext ctx) throws SystemException;

public Control recreate (PropagationContext ctx) throws SystemException;
 
};
