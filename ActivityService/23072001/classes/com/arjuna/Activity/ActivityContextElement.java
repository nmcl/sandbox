/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ActivityContextElement.java,v 1.1 2001/07/23 13:44:49 nmcl Exp $
 */

package com.arjuna.Activity;

import org.omg.CosTransactions.*;
import org.omg.CosActivity.*;
import org.omg.CORBA.*;
import java.util.*;

class ActivityContextElement
{

public final static int TRANSACTION = 1;
public final static int ACTIVITY_COORDINATOR = 2;
    
    /*
     * Work in terms of Objects so that we can add other types
     * later. The specification allows this.
     */

public ActivityContextElement (java.lang.Object o, int type)
    {
	_entity = o;
	_type = type;
    }
    
public final java.lang.Object getEntity ()
    {
	return _entity;
    }

public final int getType ()
    {
	return _type;
    }
    
public final boolean isActivity ()
    {
	if (_type == 2)
	    return true;
	else
	    return false;
    }
    
private java.lang.Object _entity;
private int              _type;
    
};
