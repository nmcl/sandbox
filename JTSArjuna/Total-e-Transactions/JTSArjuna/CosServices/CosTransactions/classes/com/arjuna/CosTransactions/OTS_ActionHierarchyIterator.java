/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_ActionHierarchyIterator.java,v 1.1.4.1.30.1.4.1 2000/12/21 11:21:14 nmcl Exp $
 */

package com.arjuna.CosTransactions;

import com.arjuna.ArjunaCommon.Template.*;
import org.omg.CosTransactions.*;

/**
 * An iterator for the OTS_ActionHierarchy.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: OTS_ActionHierarchyIterator.java,v 1.1.4.1.30.1.4.1 2000/12/21 11:21:14 nmcl Exp $
 * @since JTS 1.0.
 */

public class OTS_ActionHierarchyIterator
{

    /**
     * Create a new instance to iterate across the specified hierarchy.
     */

public OTS_ActionHierarchyIterator (OTS_ActionHierarchy toIter)
    {
	_theIter = new SimpleListIterator(toIter._hier);
    }

    /**
     * Return the next transaction in the hierarchy.
     */

public final synchronized Control iterate ()
    {
	return (Control) _theIter.iterate();
    }
    
private SimpleListIterator _theIter;
 
}
