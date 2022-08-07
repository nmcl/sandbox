/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: JTA_Utility.javatmpl,v 1.1.4.1.4.1.6.1.2.1.6.1.8.1.6.1 2001/02/09 10:22:19 pfurniss Exp $
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
































































package com.arjuna.jta;

import org.omg.CosTransactions.*;
import javax.transaction.*;
import javax.transaction.xa.*;

/**
 * Some useful utility routines.
 */

public class JTA_Utility
{

    /**
     * Print a human-readable version of the XAException.
     */

public static String printXAErrorCode (XAException e)
    {
	if (e == null)
	    return "Null exception!";
	else
	{
	    switch (e.errorCode)
	    {
	    case XAException.XA_RBROLLBACK:
		return "XAException.XA_RBROLLBACK";
	    case XAException.XA_RBCOMMFAIL:
		return "XAException.XA_RBCOMMFAIL";
	    case XAException.XA_RBDEADLOCK:
		return "XAException.XA_RBDEADLOCK";
	    case XAException.XA_RBINTEGRITY:
		return "XAException.XA_RBINTEGRITY";
	    case XAException.XA_RBOTHER:
		return "XAException.XA_RBOTHER";
	    case XAException.XA_RBPROTO:
		return "XAException.XA_RBPROTO";
	    case XAException.XA_RBTIMEOUT:
		return "XAException.XA_RBTIMEOUT";
	    case XAException.XA_RBTRANSIENT:
		return "XAException.XA_RBTRANSIENT";
	    case XAException.XA_NOMIGRATE:
		return "XAException.XA_NOMIGRATE";
	    case XAException.XA_HEURHAZ:
		return "XAException.XA_HEURHAZ";
	    case XAException.XA_HEURCOM:
		return "XAException.XA_HEURCOM";
	    case XAException.XA_HEURRB:
		return "XAException.XA_HEURRB";
	    case XAException.XA_HEURMIX:
		return "XAException.XA_HEURMIX";
	    case XAException.XA_RDONLY:
		return "XAException.XA_RDONLY";
	    case XAException.XAER_RMERR:
		return "XAException.XAER_RMERR";
	    case XAException.XAER_NOTA:
		return "XAException.XAER_NOTA";
	    case XAException.XAER_INVAL:
		return "XAException.XAER_INVAL";
	    case XAException.XAER_PROTO:
		return "XAException.XAER_PROTO";
	    case XAException.XAER_RMFAIL:
		return "XAException.XAER_RMFAIL";
	    case XAException.XAER_DUPID:
		return "XAException.XAER_DUPID";
	    case XAException.XAER_OUTSIDE:
		return "XAException.XAER_OUTSIDE";
	    default:
		return "Unknown error code: "+e.errorCode;
	    }
	}
    }
    

public static int convertStatus (org.omg.CosTransactions.Status status)

    {
	switch ( status.value() )
	{
	case  org.omg.CosTransactions.Status ._StatusActive  :
	    return javax.transaction.Status.STATUS_ACTIVE;
	case  org.omg.CosTransactions.Status ._StatusCommitted  :
	    return javax.transaction.Status.STATUS_COMMITTED;

	case  org.omg.CosTransactions.Status ._StatusMarkedRollback  :
	    return javax.transaction.Status.STATUS_MARKED_ROLLBACK;
	case  org.omg.CosTransactions.Status ._StatusNoTransaction  :
	    return javax.transaction.Status.STATUS_NO_TRANSACTION;
	case  org.omg.CosTransactions.Status ._StatusPrepared  :
	    return javax.transaction.Status.STATUS_PREPARED;
	case  org.omg.CosTransactions.Status ._StatusRolledBack  :
	    return javax.transaction.Status.STATUS_ROLLEDBACK;
	case  org.omg.CosTransactions.Status ._StatusUnknown  :
	    return javax.transaction.Status.STATUS_UNKNOWN;
	default:
	    return javax.transaction.Status.STATUS_UNKNOWN;
	}
    }

public static boolean sameXID (Xid x1, Xid x2)
    {
	if (x1 == x2)
	    return true;
	else
	{
	    if (x1.getFormatId() == x2.getFormatId())
	    {
		byte[] gtrid1 = x1.getGlobalTransactionId();
		byte[] gtrid2 = x2.getGlobalTransactionId();
		
		if (gtrid1.length == gtrid2.length)
		{
		    for (int i = 0; i < gtrid1.length; i++)
		    {
			if (gtrid1[i] != gtrid2[i])
			    return false;
		    }
		}
		else
		    return false;

		byte[] bqual1 = x1.getBranchQualifier();
		byte[] bqual2 = x2.getBranchQualifier();

		if (bqual1.length == bqual2.length)
		{
		    for (int i = 0; i < bqual1.length; i++)
		    {
			if (bqual1[i] != bqual2[i])
			    return false;
		    }
		}
		else
		    return false;

		return true;
	    }
	    else
		return false;
	}
    }

/**
 * get a string representing anyones Xid 
 * similar too, but not the same as OTS_Transaction/xa/XID.toString()
 */
public static String xidToString(Xid xid)
{
    String globalTranId = new String(xid.getGlobalTransactionId());  // could be garbage!
    String branchQualifier = new String(xid.getBranchQualifier());   // and this
	
    return new String("< "+xid.getFormatId()+", "+globalTranId+", "+branchQualifier+" >");

}

};
