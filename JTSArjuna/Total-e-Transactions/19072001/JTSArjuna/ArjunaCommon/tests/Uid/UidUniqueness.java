/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: UidUniqueness.java,v 1.1.36.1 2001/01/24 13:39:35 nmcl Exp $
 */

import com.arjuna.ArjunaCommon.Common.*;

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
