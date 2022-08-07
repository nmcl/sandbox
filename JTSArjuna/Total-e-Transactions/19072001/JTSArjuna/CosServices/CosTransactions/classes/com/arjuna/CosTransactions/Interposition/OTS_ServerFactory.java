/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_ServerFactory.java,v 1.3.2.1.4.1.2.2.8.1.10.1.4.1.8.1.14.1.14.1 2001/06/28 14:09:11 nmcl Exp $
 */

package com.arjuna.CosTransactions.Interposition;

import com.arjuna.OrbCommon.ORBInterface;
import com.arjuna.ArjunaCommon.Common.Uid;
import com.arjuna.ArjunaCommon.Common.ErrorStream;
import com.arjuna.ArjunaCore.Interface.ObjectStore;
import com.arjuna.CosTransactions.*;
import org.omg.CosTransactions.*;
import java.util.Enumeration;

import org.omg.CORBA.BAD_PARAM;
import org.omg.CORBA.SystemException;

/**
 * This is a server-side factory used for creating server transactions.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: OTS_ServerFactory.java,v 1.3.2.1.4.1.2.2.8.1.10.1.4.1.8.1.14.1.14.1 2001/06/28 14:09:11 nmcl Exp $
 * @since JTS 1.2.4.
 */

public class OTS_ServerFactory
{

    /**
     * @return the server transaction status.
     * @since JTS 2.1.1.
     */

public static org.omg.CosTransactions.Status getCurrentStatus (Uid uid) throws SystemException
    {
	if (!uid.valid())
	    throw new BAD_PARAM();
	else
	{
	    try
	    {
		OTS_Control ctx = null;

		synchronized (OTS_ServerControl.allServerControls)
		    {
			ctx = (OTS_ServerControl) OTS_ServerControl.allServerControls.get(uid);
		    }
		    
		/*
		 * If it's not present then check each element's
		 * savingUid just in case that is being used
		 * instead of the transaction id. This is because
		 * a server transaction actually has two names:
		 *
		 * (i) the tid it pretends to be
		 * (ii) the tid it actually is and saves its
		 *      intentions list in.
		 *
		 * Don't bother synchronizing since the hash table is
		 * synchronized anyway, and we're not bothered if new
		 * items go in while we're looking. If the element
		 * we're looking for isn't there now it won't be there
		 * at all.
		 */

		if (ctx == null)
		{
		    Enumeration e = OTS_ServerControl.allServerControls.elements();
		    
		    while (e.hasMoreElements())
		    {
			ctx = (OTS_ServerControl) e.nextElement();
			
			if (ctx.getImplHandle().getSavingUid().equals(uid))
			{
			    break;
			}
		    }
		}
		
		if (ctx != null)
		    return ctx.getImplHandle().get_status();
		else
		    throw new NoTransaction();
	    }
	    catch (NoTransaction ex)
	    {
		return org.omg.CosTransactions.Status.StatusNoTransaction;
	    }
	    catch (Exception e)
	    {
		ErrorStream.warning().println("OTS_ServerFactory.getCurrentStatus for "+uid+" caught: "+e);
		
		return org.omg.CosTransactions.Status.StatusUnknown;
	    }
	}
    }

    /*
     * @return the status of the transaction, even if it is not active.
     * @since JTS 2.1.1.
     */

public static org.omg.CosTransactions.Status getStatus (Uid u) throws NoTransaction, SystemException
    {
	org.omg.CosTransactions.Status s = org.omg.CosTransactions.Status.StatusUnknown;
	
	try
	{
	    s = getCurrentStatus(u);
	}
	catch (SystemException e2)
	{
	    throw e2;
	}
	catch (Exception e3)
	{
	    ErrorStream.warning().println("OTS_ServerFactory.getStatus for "+u+" caught: "+e3);

	    return org.omg.CosTransactions.Status.StatusUnknown;
	}	    

	if ((s == org.omg.CosTransactions.Status.StatusUnknown) ||
	    (s == org.omg.CosTransactions.Status.StatusNoTransaction))
	{
	    return getOSStatus(u);
	}
	else
	    return s;
    }

/**
 * @return the status of the server transaction as recorded in the object store.
 * @since JTS 2.1.1.
 */

public static org.omg.CosTransactions.Status getOSStatus (Uid u) throws NoTransaction, SystemException
    {
	org.omg.CosTransactions.Status s = org.omg.CosTransactions.Status.StatusUnknown;

	if (!u.valid())
	    throw new BAD_PARAM();
	else
	{
	    // if here then it is not active, so look in the object store

	    ObjectStore store = com.arjuna.ArjunaCore.Atomic.InitAction.getStore();

	    try
	    {
		/*
		 * Do we need to search server transactions too?
		 * Possibly not, since an interposed coordinator can never
		 * always say with certainty what the status is of the root
		 * coordinator.
		 */

		int status = store.currentState(u, OTS_ServerTransaction.typeName());

		switch (status)
		{
		case ObjectStore.OS_UNKNOWN:  // means no state present
		    return org.omg.CosTransactions.Status.StatusNoTransaction;
		case ObjectStore.OS_COMMITTED:
		    return org.omg.CosTransactions.Status.StatusCommitted;
		case ObjectStore.OS_UNCOMMITTED:
		    return org.omg.CosTransactions.Status.StatusPrepared;
		case ObjectStore.OS_HIDDEN:
		case ObjectStore.OS_COMMITTED_HIDDEN:
		case ObjectStore.OS_UNCOMMITTED_HIDDEN:
		    return org.omg.CosTransactions.Status.StatusPrepared;
		default:
		    return org.omg.CosTransactions.Status.StatusUnknown;
		}
	    }
	    catch (Exception e)
	    {
		ErrorStream.warning().println("OTS_ServerFactory.getStatus for "+u+" caught: "+e);
		
		return org.omg.CosTransactions.Status.StatusUnknown;
	    }
	}
    }

public static OTS_ServerControl create_transaction (Uid u,
						    Control parentControl,
						    OTS_Transaction parentImpl,
						    Coordinator realCoord,
						    Terminator realTerm,
						    int time_out)
    {
	OTS_ServerControl tranControl = new OTS_ServerControl(u, parentControl, parentImpl, realCoord, realTerm);
	
	if ((time_out != 0) && (parentImpl == null))
	{
	    /*
	     * Currently we do not remove controls from the list once they
	     * have terminated. We should to save time and space!
	     */
	
	    OTS_TransactionReaper reaper = OTS_TransactionReaper.transactionReaper();
	
	    if (reaper == null)
		reaper = OTS_TransactionReaper.create();

	    reaper.insert(new OTS_ControlWrapper((OTS_Control) tranControl), time_out);
	}

	return tranControl;
    }

public static OTS_ServerControl create_subtransaction (Uid actUid, Coordinator realCoord,
						       Terminator realTerm, OTS_ServerControl parent)
    {
	if (parent == null)
	{
	    ErrorStream.stream(ErrorStream.WARNING).println("OTS_ServerFactory::create_subtransaction - no parent given!");
	    return null;
	}
    
	OTS_ServerControl toReturn = null;

	try
	{
	    Control handle = parent.getControl();
	    OTS_Transaction tranHandle = parent.getImplHandle ();
	
	    toReturn = new OTS_ServerControl(actUid, handle, tranHandle, realCoord, realTerm);

	    handle = null;
	    tranHandle = null;
	}
	catch (Exception e)
	{
	    if (toReturn != null)
	    {
		try
		{
		    toReturn.destroy();  // will delete itself
		}
		catch (Exception ex)
		{
		}
	    }
	}
    
	return toReturn;
    }
 
};
