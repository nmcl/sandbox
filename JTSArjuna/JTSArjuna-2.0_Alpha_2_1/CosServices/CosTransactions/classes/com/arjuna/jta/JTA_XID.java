
/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: JTA_XID.java,v 1.1 2000/04/06 11:00:30 nmcl Exp $
 */

package com.arjuna.jta;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.CosTransactions.*;
import com.arjuna.CosTransactions.tx.*;
import org.omg.CosTransactions.Control;
import javax.transaction.xa.Xid;
import java.io.*;

import java.io.IOException;

public class JTA_XID implements Xid, Serializable
{

public JTA_XID ()
    {
	_theXid = null;
    }
    
public JTA_XID (Control c)
    {
	this(c, false);
    }
    
public JTA_XID (Control c, boolean branch)
    {
	try
	{
	    _theXid = OTS_Utility.getXid(c, branch);
	    
	    c = null;
	}
	catch (Exception e)
	{
	    _theXid = null;

	    // abort or throw exception?
	}
    }

public JTA_XID (XID x)
    {
	_theXid = x;
    }    

public final boolean isSameTransaction (Xid xid)
    {
	if (xid == null)
	    return false;
	
	if (xid instanceof XID)
	{
	    return _theXid.isSameTransaction(((JTA_XID)xid)._theXid);
	}
	
	if (getFormatId() == xid.getFormatId())
	{
	    byte[] gtx = xid.getGlobalTransactionId();
	    
	    if (_theXid.gtrid_length == gtx.length)
	    {
		if (equals(xid))
		    return true;
		else
		{
		    for (int i = 0; i < _theXid.gtrid_length; i++)
		    {
			if (_theXid.data[i] != gtx[i])
			    return false;
		    }
		    
		    return true;
		}
	    }
	}
	
	return false;

    }
    
public int getFormatId ()
    {
	if (_theXid != null)
	{
	    return _theXid.formatID;
	}
	else
	    return -1;
    }

    /*
     * These operations critically rely on the fact that we
     * unpack the array in the order we packed it!
     */
    
public byte[] getGlobalTransactionId ()
    {
	if (_theXid != null)
	{
	    byte b[] = new byte[_theXid.gtrid_length];

	    for (int i = 0; i < b.length; i++)
	    {
		b[i] = _theXid.data[i];
	    }
	    
	    return b;
	}
	else
	    return null;
    }
    
public byte[] getBranchQualifier ()
    {
	if (_theXid != null)
	{
	    byte b[] = new byte[_theXid.bqual_length];

	    for (int i = _theXid.gtrid_length; i < b.length; i++)
	    {
		b[i] = _theXid.data[i];
	    }
	    
	    return b;
	}
	else
	    return null;
    }

public XID getXID ()
    {
	return _theXid;
    }

public void copy (Xid xid)
    {
	_theXid = new XID();

	if (xid != null)
	{
	    if (xid instanceof JTA_XID)
		_theXid.copy(((JTA_XID)xid)._theXid);
	    else
	    {
		_theXid.formatID = xid.getFormatId();
		
		byte[] gtx = xid.getGlobalTransactionId();
		byte[] bql = xid.getBranchQualifier();
		
		_theXid.gtrid_length = gtx.length;
		_theXid.bqual_length = bql.length;
		
		int i;
		
		for (i = 0; i < _theXid.gtrid_length; i++)
		    _theXid.data[i] = gtx[i];
			
		for (i = _theXid.gtrid_length; i < _theXid.gtrid_length + _theXid.bqual_length; i++)
		    _theXid.data[i] = bql[i];
	    }
	}
    }
	
public boolean equals (Xid xid)
    {
	if (xid == null)
	    return false;
	
	if (xid == this)
	    return true;
	else
	{
	    if (xid instanceof JTA_XID)
		return ((JTA_XID)xid)._theXid.equals(_theXid);
	    else
	    {
		if (xid.getFormatId() == _theXid.formatID)
		{
		    byte[] gtx = xid.getGlobalTransactionId();
		    byte[] bql = xid.getBranchQualifier();
		    
		    if ((_theXid.gtrid_length == gtx.length) &&
			(_theXid.bqual_length == bql.length))
		    {
			int i;
			
			for (i = 0; i < _theXid.gtrid_length; i++)
			{
			    if (_theXid.data[i] != gtx[i])
				return false;
			}
			
			for (i = _theXid.gtrid_length; i < _theXid.gtrid_length + _theXid.bqual_length; i++)
			{
			    if (_theXid.data[i] != bql[i])
				return false;
			}
			
			return true;
		    }
		}
	    }
	}
	
	return false;
    }
    
public boolean packInto (OutputObjectState os)
    {
	boolean result = false;

	try
	{
	    os.packInt(_theXid.formatID);
	    os.packInt(_theXid.gtrid_length);
	    os.packInt(_theXid.bqual_length);
	    os.packBytes(_theXid.data);

	    result = true;
	}
	catch (Exception e)
	{
	    result = false;
	}

	return result;
    }

public boolean unpackFrom (InputObjectState os)
    {
	boolean result = false;

	try
	{
	    if (_theXid == null)
		_theXid = new XID();
	    
	    _theXid.formatID = os.unpackInt();
	    _theXid.gtrid_length = os.unpackInt();
	    _theXid.bqual_length = os.unpackInt();
	    _theXid.data = os.unpackBytes();

	    result = true;
	}
	catch (Exception e)
	{
	    result = false;
	}

	return result;
    }

public static void pack (OutputObjectState os, Xid xid) throws IOException
    {
	if (xid instanceof JTA_XID)
	{
	    JTA_XID x = (JTA_XID) xid;

	    os.packBoolean(true);
	    
	    if (!x.packInto(os))
		throw new IOException("Could not pack JTA_XID.");
	}
	else
	{
	    os.packBoolean(false);
		
	    ByteArrayOutputStream s = new ByteArrayOutputStream();
	    ObjectOutputStream o = new ObjectOutputStream(s);

	    o.writeObject(xid);
	    o.close();

	    os.packBytes(s.toByteArray());
	}
    }

public static Xid unpack (InputObjectState os) throws IOException
    {
	if (os.unpackBoolean())
	{
	    JTA_XID x = new JTA_XID();
	    
	    x.unpackFrom(os);

	    return x;
	}
	else
	{
	    try
	    {
		byte[] b = os.unpackBytes();
	    
		ByteArrayInputStream s = new ByteArrayInputStream(b);
		ObjectInputStream o = new ObjectInputStream(s);
	    
		Xid x = (Xid) o.readObject();

		return x;
	    }
	    catch (Exception e)
	    {
		throw new IOException(e.toString());
	    }
	}
    }

public String toString ()
    {
	if (_theXid != null)
	    return _theXid.toString();
	else
	    return "Xid unset";
    }
    
private XID _theXid;
    
};
