/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_Utility.javatmpl,v 1.2.8.1.20.1.2.2.2.1.2.1.34.1.4.1 2001/07/23 11:13:08 nmcl Exp $
 */

/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OrbPortability.h,v 1.1 2000/02/25 14:09:59 nmcl Exp $
 */




/*
 * Try to get around the differences between Ansi CPP and
 * K&R cpp with concatenation.
 */


/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: OrbPortability_stdc.h,v 1.2.4.1.2.3.26.1.2.2.4.2 2001/02/02 11:51:52 nmcl Exp $
 */
































































package com.arjuna.CosTransactions;

import com.arjuna.ArjunaCommon.Common.Uid;
import com.arjuna.ArjunaCore.Atomic.BasicAction;
import com.arjuna.CosTransactions.tx.XID;
import com.arjuna.CosTransactions.ArjunaOTS.Arjuna;
import org.omg.CosTransactions.*;
import com.arjuna.ArjunaOTS.*;
import java.io.PrintWriter;

import org.omg.CosTransactions.Unavailable;
import org.omg.CORBA.SystemException;
import java.lang.IllegalStateException;
import org.omg.CORBA.BAD_PARAM;

/**
 * Some useful utility functions for the OTS. Use with care!
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: OTS_Utility.javatmpl,v 1.2.8.1.20.1.2.2.2.1.2.1.34.1.4.1 2001/07/23 11:13:08 nmcl Exp $
 * @since JTS 1.0.
 */

public class OTS_Utility
{
 
public static PrintWriter printHierarchy (PrintWriter strm, PropagationContext ctx)
    {
	int depth = ((ctx.parents != null) ? ctx.parents.length : 0);

	strm.print("PropagationContext:");
    
	for (int i = depth -1; i >= 0; i--)
	{
	    if (ctx.parents[i] != null)
		strm.print("\n"+OTS_Utility.otidToUid(ctx.parents[i].otid));
	}

	strm.print("\n"+OTS_Utility.otidToUid(ctx.currentTransaction.otid));

	return strm;
    }

    /*
     * Deprecated As of 2.1.2 use the other form of printHierarchy.
     *
     * @deprecated As of 2.1.2 use the other form of printHierarchy.
     */

public static void printHierarchy (PropagationContext ctx, PrintWriter strm)
    {
	printHierarchy(strm, ctx);
    }
    
    /*
     * Deprecated As of 2.1.2 use stringStatus.
     *
     * @deprecated As of 2.1.2 use stringStatus.
     */


public static String printStatus (org.omg.CosTransactions.Status res)

    {
	return stringStatus(res);
    }

    /*
     * Deprecated As of 2.1.2 use stringVote.
     *
     * @deprecated As of 2.1.2 use stringVote.
     */


public static String printVote (org.omg.CosTransactions.Vote v)

    {
	return stringVote(v);
    }


public static PrintWriter printVote (PrintWriter strm, org.omg.CosTransactions.Vote v)

    {
	strm.print(stringVote(v));
	
	return strm;
    }

    /**
     * @since JTS 2.1.2.
     */


public static String stringVote (org.omg.CosTransactions.Vote v)

    {
	switch ( v.value() )
	{
	case  Vote ._VoteCommit  :
	    return "CosTransactions::VoteCommit";
	case  Vote ._VoteRollback  :
	    return "CosTransactions::VoteRollback";
	case  Vote ._VoteReadOnly  :
	    return "CosTransactions::VoteReadOnly";
	default:
	    return "Unknown";
	}
    }


public static PrintWriter printStatus (PrintWriter strm, org.omg.CosTransactions.Status res)

    {
	strm.print(stringStatus(res));
	
	return strm;
    }

    /**
     * @since JTS 2.1.2.
     */


public static String stringStatus (org.omg.CosTransactions.Status res)

    {
	switch ( res.value() )
	{
	case  Status ._StatusActive  :
	    return "CosTransactions::StatusActive";
	case  Status ._StatusMarkedRollback  :
	    return "CosTransactions::StatusMarkedRollback";
	case  Status ._StatusPrepared  :
	    return "CosTransactions::StatusPrepared";
	case  Status ._StatusCommitted  :
	    return "CosTransactions::StatusCommitted";
	case  Status ._StatusRolledBack  :
	    return "CosTransactions::StatusRolledBack";
	case  Status ._StatusUnknown  :
	    return "CosTransactions::StatusUnknown";

	case  Status ._StatusPreparing  :
	    return "CosTransactions::StatusPreparing";
	case  Status ._StatusCommitting  :
	    return "CosTransactions::StatusCommitting";
	case  Status ._StatusRollingBack  :
	    return "CosTransactions::StatusRollingBack";

	case  Status ._StatusNoTransaction  :
	    return "CosTransactions::StatusNoTransaction";
	default:
	    return "Unknown";
	}
    }

    /*
     * Any need for the inverse operation?
     * Could easily do it for Arjuna transactions only.
     */

public static com.arjuna.CosTransactions.tx.XID getXid (Uid uid, boolean branch) throws IllegalStateException
    {
	if (uid == null)
	    throw new IllegalStateException();
	
	String s = uid.stringForm();
	int uidLen = s.length();
		
	if (uidLen > XID.MAXGTRIDSIZE)  // Uid is too long!!!!
	    throw new IllegalStateException();
	else
	{
	    try
	    {
		XID xid = new XID();
		byte[] b = s.getBytes();
		    
		xid.formatID = OTS_Transaction.interpositionType();
		xid.gtrid_length = uidLen;

		/*
		 * Which way round do we fill in the structure?
		 */

		System.arraycopy (b, 0, xid.data, 0, uidLen);
		    
		if (branch)
		{
		    Uid branchUid = new Uid();
		    String bs = branchUid.stringForm();
		    int bsLen = bs.length();

		    b = bs.getBytes();

		    if (bsLen > XID.MAXBQUALSIZE) // Uid is too long!!!!
			throw new IllegalStateException();
		    else
		    {
			xid.bqual_length = bsLen;
		    
			System.arraycopy (b, 0, xid.data, xid.gtrid_length, bsLen);
		    }
		
		    bs = null;
		    branchUid = null;
		}
		else
		{
		    /*
		     * Note: for some dbs we seem to be able to get
		     * away with setting the size field to the size
		     * of the actual branch. However, for Oracle,
		     * it appears as though it must always be 64.
		     * (At least for zero branches.)
		     */

		    xid.data[xid.gtrid_length] = (byte) 0;
		    xid.bqual_length = 64;
		}
		    
		b = null;
		s = null;

		return xid;
	    }
	    catch (Exception e)
	    {
		throw new IllegalStateException();
	    }
	}
    }
    
public static com.arjuna.CosTransactions.tx.XID getXid (Control cont, boolean branch) throws IllegalStateException
    {
	if (cont == null)
	    throw new IllegalStateException();

	if (cont instanceof OTS_Control)
	{
	    try
	    {
		UidCoordinator arjcoord = OTS_Helper.getUidCoordinator(cont);
		Uid u = OTS_Helper.getUid(arjcoord);

		return OTS_Utility.getXid(u, branch);
	    }
	    catch (Exception e)
	    {
		throw new IllegalStateException();
	    }
	}
	else
	    throw new IllegalStateException();
    }

public static Uid getUid (XID xid)
    {
	if ((xid.formatID == -1) || (xid.gtrid_length <= 0)) // means null XID
	    return Uid.nullUid();
	else
	{
	    byte[] buff = new byte[xid.gtrid_length+1];

	    buff[xid.gtrid_length] = (byte) '\0';

	    for (int i = 0; i < xid.gtrid_length; i++)
	    {
		buff[i] = xid.data[i];
	    }

	    Uid tx = new Uid(new String(buff), true);

	    buff = null;

	    return tx;
	}
    }

    /**
     * If this control refers to an Arjuna transaction then return its native
     * Uid representation. Otherwise return Uid.nullUid().
     */

public static final Uid getUid (Control cont)
    {
	try
	{
	    UidCoordinator coord = OTS_Helper.getUidCoordinator(cont);

	    if (coord == null)
		throw new BAD_PARAM();
	    else
		return OTS_Helper.getUid(coord);
	}
	catch (BAD_PARAM ex)
	{
	    return Uid.nullUid();
	}
    }

    /**
     * @since JTS 2.2.
     */

public static final org.omg.CosTransactions.otid_t uidToOtid (Uid theUid)
    {
	return ((theUid != null) ? uidToOtid(theUid.stringForm()) : null);
    }
    
public static final org.omg.CosTransactions.otid_t uidToOtid (String theUid)
    {
	if (theUid == null)
	    return null;
	
	otid_t otid = new otid_t();
	byte[] b = theUid.getBytes();

	otid.tid = new byte[b.length+1];
	otid.bequal_length = b.length+1;

	for (int i = 0; i < b.length; i++)
	    otid.tid[i] = b[i];

	otid.tid[b.length] = (byte) '\0';

	b = null;
	
	return otid;
    }

    /*
     * If we cannot deal with the otid then we could simply generate
     * a new Uid. We would need to keep the original otid in case
     * the transaction either retransmits it or the application asks
     * for it (e.g., via the PropagationContext).
     */
    
public static final Uid otidToUid (org.omg.CosTransactions.otid_t otid)
    {
	if (otid.bequal_length > 0)
	{
	    byte[] theUid = new byte[otid.bequal_length-1];  // don't need null terminating character

	    for (int i = 0; i < otid.bequal_length -1; i++)
		theUid[i] = otid.tid[i];

	    Uid u = new Uid(new String(theUid), true);  // errors in string give NIL_UID

	    theUid = null;

	    return u;
	}
	else
	    return Uid.nullUid();
    }

}
