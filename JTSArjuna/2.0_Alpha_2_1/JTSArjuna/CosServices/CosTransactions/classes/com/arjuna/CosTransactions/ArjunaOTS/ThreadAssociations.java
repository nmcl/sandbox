/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ThreadAssociations.java,v 1.5.6.1 2000/05/22 10:23:54 nmcl Exp $
 */

package com.arjuna.CosTransactions.ArjunaOTS;

import org.omg.CosTransactions.*;
import com.arjuna.CosTransactions.OTS_Current;
import com.arjuna.CosTransactions.OTS_Control;
import java.util.*;

/*
 * Instances of TxAssociation can be added on
 * a per thread basis, or for all threads.
 */

public class ThreadAssociations
{

public final static boolean add (TxAssociation tx)
    {
	if (tx == null)
	    return false;
	
	Vector v;
	Thread ct = Thread.currentThread();
	    
	synchronized (txAssociations)
	    {
		v = (Vector) txAssociations.get(tx);

		if (v == null)
		{
		    v = new Vector();
		    txAssociations.put(ct, v);
		}
	    }

	v.addElement(tx);

	return true;
    }

public final static boolean addGlobal (TxAssociation tx)
    {
	if (tx == null)
	    return false;

	synchronized (globalTxAssociations)
	    {
		globalTxAssociations.addElement(tx);
	    }

	return true;
    }

public final static boolean remove (TxAssociation tx)
    {
	if (tx == null)
	    return false;

	synchronized (txAssociations)
	    {
		Thread ct = Thread.currentThread();
		Vector v = (Vector) txAssociations.get(ct);
		
		v.removeElement(tx);

		if (v.size() == 0)
		    txAssociations.remove(ct);
	    }

	return true;
    }

public final static boolean removeGlobal (TxAssociation tx)
    {
	if (tx == null)
	    return false;

	synchronized (globalTxAssociations)
	    {
		globalTxAssociations.removeElement(tx);
	    }

	return true;
    }

public final static boolean removeAll (boolean global)
    {
	if (global)
	{
	    synchronized (globalTxAssociations)
		{
		    globalTxAssociations.removeAllElements();
		    globalTxAssociations = null;
		}
	}
	else
	{
	    synchronized (txAssociations)
		{
		    txAssociations.clear();
		    txAssociations = null;
		}
	}

	return true;
    }
	    
final static void updateAssociation (OTS_Control tx, int reason)
    {
	
	/*
	 * Do thread specific first.
	 */
	
	try
	{
	    synchronized (txAssociations)
		{
		    Vector v = (Vector) txAssociations.get(Thread.currentThread());

		    if (v != null)
		    {
			for (int i = 0; i < v.size(); i++)
			{
			    TxAssociation ta = (TxAssociation) v.elementAt(i);

			    if (ta != null)
				update(ta, tx, reason);
			}
		    }
		}
	    
	    /*
	     * Now do globals.
	     */

	    synchronized (globalTxAssociations)
		{
		    for (int i = 0; i < globalTxAssociations.size(); i++)
		    {
			TxAssociation ta = (TxAssociation) globalTxAssociations.elementAt(i);
		
			if (ta != null)
			    update(ta, tx, reason);
		    }
		}
	}
	catch (Throwable t)
	{
	    // ignore any exceptions or errors!
	}
    }
    
final static void updateAssociation (Control tx, int reason)
    {
	/*
	 * Do thread specific first.
	 */
	
	try
	{
	    synchronized (txAssociations)
		{
		    Vector v = (Vector) txAssociations.get(Thread.currentThread());

		    if (v != null)
		    {
			for (int i = 0; i < v.size(); i++)
			{
			    TxAssociation ta = (TxAssociation) v.elementAt(i);

			    if (ta != null)
				update(ta, tx, reason);
			}
		    }
		}
	    
	    /*
	     * Now do globals.
	     */

	    synchronized (globalTxAssociations)
		{
		    for (int i = 0; i < globalTxAssociations.size(); i++)
		    {
			TxAssociation ta = (TxAssociation) globalTxAssociations.elementAt(i);
		
			if (ta != null)
			    update(ta, tx, reason);
		    }
		}
	}
	catch (Throwable t)
	{
	    // ignore any exceptions or errors!
	}
    }
 
private static void update (TxAssociation ta, Control tx, int reason)
    {
	switch (reason)
	{
	case OTS_Current.TX_BEGUN:
	    {
		ta.begin(tx);
	    }
	    break;
	case OTS_Current.TX_COMMITTED:
	    {
		ta.commit(tx);
	    }
	    break;
	case OTS_Current.TX_ABORTED:
	    {
		ta.rollback(tx);
	    }
	    break;
	case OTS_Current.TX_SUSPENDED:
	    {
		ta.suspend(tx);
	    }
	    break;
	case OTS_Current.TX_RESUMED:
	    {
		ta.resume(tx);
	    }
	    break;
	default:
	    break;
	}
    }

private static void update (TxAssociation ta, OTS_Control tx, int reason)
    {
	switch (reason)
	{
	case OTS_Current.TX_BEGUN:
	    {
		ta.begin(tx);
	    }
	    break;
	case OTS_Current.TX_COMMITTED:
	    {
		ta.commit(tx);
	    }
	    break;
	case OTS_Current.TX_ABORTED:
	    {
		ta.rollback(tx);
	    }
	    break;
	case OTS_Current.TX_SUSPENDED:
	    {
		ta.suspend(tx);
	    }
	    break;
	case OTS_Current.TX_RESUMED:
	    {
		ta.resume(tx);
	    }
	    break;
	default:
	    break;
	}
    }

private static Hashtable txAssociations = new Hashtable();
private static Vector globalTxAssociations = new Vector();

};
