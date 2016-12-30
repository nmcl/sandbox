/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_ActionHierarchyIterator.java,v 1.1.4.1 2000/04/25 08:36:03 nmcl Exp $
 */

package com.arjuna.CosTransactions;

import com.arjuna.ArjunaCommon.Template.*;
import org.omg.CosTransactions.*;

public class OTS_ActionHierarchyIterator
{
    
public OTS_ActionHierarchyIterator (OTS_ActionHierarchy toIter)
    {
	_theIter = new SimpleListIterator(toIter._hier);
    }

public final synchronized Control iterate ()
    {
	return (Control) _theIter.iterate();
    }
    
private SimpleListIterator _theIter;
 
};
