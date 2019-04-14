package com.hp.mwtests.ts.arjuna.uid;

/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: UidUniqueness.java,v 1.2 2003/06/19 10:51:22 nmcl Exp $
 */

import com.arjuna.ats.arjuna.common.*;

public class UidUniqueness
{
    
public static void main (String[] args)
    {
	for (int i = 0; i < 100; i++)
	{
	    Uid u = new Uid();
	    System.out.println(u + " " + u.hashCode());
	}
    }
    
}
