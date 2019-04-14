/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: SimpleObject.java,v 1.1 2002/05/29 10:37:42 rbegg Exp $
 */

package com.hp.mwtests.ts.arjuna.resources;

public class SimpleObject
{

public SimpleObject ()
    {
	state = 0;

	System.out.println("Created simple object.");
    }
    
public void incr (int value)
    {
	state += value;
    }
	
public void set (int value)
    {
	state = value;
    }

public int get ()
    {
	return state;
    }

private int state;
    
};

