/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: UidUniqueness.java,v 1.1 2000/02/25 14:03:25 nmcl Exp $
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
    
};
