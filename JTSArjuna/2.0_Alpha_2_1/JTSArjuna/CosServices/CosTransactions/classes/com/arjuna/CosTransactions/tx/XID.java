/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: XID.java,v 1.2 2000/04/06 11:00:29 nmcl Exp $
 */

package com.arjuna.CosTransactions.tx;

import java.io.Serializable;

public class XID implements Serializable
{
    
public static final int XIDDATASIZE = 128;  /* size in bytes */
public static final int MAXGTRIDSIZE = 64;  /* maximum size in bytes of gtrid */
public static final int MAXBQUALSIZE = 64;  /* maximum size in bytes of bqual */
public static final int NULL_XID = -1;
    
public XID ()
    {
	formatID = NULL_XID;
	gtrid_length = 0;
	bqual_length = 0;
    }

    /*
     * Check for equality, then check transaction id only.
     */

public final boolean isSameTransaction (XID xid)
    {
	if (formatID == xid.formatID)
	{
	    if (gtrid_length == xid.gtrid_length)
	    {
		if (equals(xid))
		    return true;
		else
		{
		    for (int i = 0; i < gtrid_length; i++)
		    {
			if (data[i] != xid.data[i])
			    return false;
		    }
		    
		    return true;
		}
	    }
	}
	
	return false;
    }
	    
public void copy (XID toCopy)
    {
	if ((toCopy == null) || (toCopy.formatID == NULL_XID))
	{
	    formatID = NULL_XID;
	    gtrid_length = 0;
	    bqual_length = 0;
	}
	else
	{
	    formatID = toCopy.formatID;
	    gtrid_length = toCopy.gtrid_length;
	    bqual_length = toCopy.bqual_length;
	    
	    System.arraycopy(toCopy.data, 0, data, 0, toCopy.data.length);
	}
    }
	    
public boolean equals (XID other)
    {
	if (other == null)
	    return false;
	
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

public String toString ()
    {
	String dat = new String(data);  // could be garbage!
	
	return new String("< "+formatID+", "+gtrid_length+", "+bqual_length+", "+dat+" >");
    }
    
public int formatID;           /* format identifier (0 for OSI) */
public int gtrid_length;       /* value not to exceed 64 */
public int bqual_length;       /* value not to exceed 64 */
public byte[] data = new byte[XIDDATASIZE];
    
};
