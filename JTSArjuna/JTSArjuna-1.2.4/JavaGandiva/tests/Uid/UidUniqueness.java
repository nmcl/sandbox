/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: UidUniqueness.java,v 1.1 1998/11/18 09:01:38 nmcl Exp $
 */

import com.arjuna.JavaGandiva.Common.*;

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
    
};
