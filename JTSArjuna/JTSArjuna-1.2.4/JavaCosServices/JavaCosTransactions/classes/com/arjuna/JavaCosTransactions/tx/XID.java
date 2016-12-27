/*
 * Copyright (C) 1996, 1997, 1998, 1999,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: XID.java,v 1.1.2.1 1999/02/16 14:56:18 nmcl Exp $
 */

package com.arjuna.JavaCosTransactions.tx;

public class XID
{
    
public static final int XIDDATASIZE = 128;  /* size in bytes */
public static final int MAXGTRIDSIZE = 64;  /* maximum size in bytes of gtrid */
public static final int MAXBQUALSIZE = 64;  /* maximum size in bytes of bqual */

public int formatID;           /* format identifier (0 for OSI) */
public int gtrid_length;       /* value not to exceed 64 */
public int bqual_length;       /* value not to exceed 64 */
public byte[] data = new byte[XIDDATASIZE];

public boolean equals (XID other)
    {
	if (other == this)
	    return true;
	else
	{
	    if ((formatID == other.formatID) &&
		(gtrid_length == other.gtrid_length) &&
		(bqual_length == other.bqual_length))
	    {
		for (int i = 0; i < (gtrid_length + bqual_length); i++)
		{
		    if (data[i] != other.data[i])
			return false;
		}

		return true;
	    }
	    else
		return false;
	}
    }
    
};
