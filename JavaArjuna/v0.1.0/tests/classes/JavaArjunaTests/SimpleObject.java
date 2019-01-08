/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: SimpleObject.java,v 1.1 1998/06/24 15:07:56 nmcl Exp $
 */

package JavaArjunaLiteTests;

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

