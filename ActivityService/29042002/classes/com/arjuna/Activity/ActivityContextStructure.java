/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ActivityContextStructure.java,v 1.1 2001/07/23 13:44:49 nmcl Exp $
 */

package com.arjuna.Activity;

import org.omg.CosTransactions.*;
import org.omg.CosActivity.*;
import org.omg.CORBA.*;
import java.util.*;

class ActivityContextStructure
{
public ActivityContextStructure ()
    {
	_data = new Vector();
    }
    
public final ActivityContextElement elementAt (int index)
    {
	return (ActivityContextElement) _data.elementAt(index);
    }

public final void addElement (ActivityContextElement e)
    {
	_data.addElement(e);
    }
    
public final int size ()
    {
	return _data.size();
    }
    
private Vector _data;

};
