/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: CheckedTransactions.java,v 1.1.28.1 2000/11/08 14:25:55 nmcl Exp $
 */

import com.arjuna.OrbCommon.ORBInterface;
import com.arjuna.OrbCommon.OAInterface;
import com.arjuna.CosTransactions.OTS;
import com.arjuna.CosTransactions.OTS_Current;
import com.arjuna.ArjunaCore.Atomic.CheckedAction;
import com.arjuna.ArjunaCommon.Common.Uid;
import org.omg.CosTransactions.*;
import java.util.*;

class MyCheckedAction extends CheckedAction
{
    
public synchronized void check (boolean isCommit, Uid actUid, Hashtable list)
    {
	// don't do anything so that no warning message is printed!
    }
    
}

class TXThread extends Thread
{

public TXThread (Control c)
    {
        cont = c;
    }

public void run ()
    {
	try
	{
	    System.out.println("Thread "+Thread.currentThread()+" attempting to rollback transaction.");
	    
	    cont.get_terminator().rollback();

	    System.out.println("Transaction rolled back. Checked transactions disabled.");
	}
	catch (Exception e)
	{
	    System.out.println("Caught exception: "+e);
	    System.out.println("Checked transactions enabled!");
	}
    }
    
private Control cont;
    
};

public class CheckedTransactions
{
    
public static void main (String[] args)
    {
	try
	{
	    ORBInterface.initORB(args, null);
	    OAInterface.initOA();

	    for (int i = 0; i < args.length; i++)
	    {
		if (args[i].compareTo("-check") == 0)
		{
		    Properties p = System.getProperties();

		    p.put("OTS_CHECKED_TRANSACTIONS", "YES");
		    
		    System.setProperties(p);
		}
		if (args[i].compareTo("-help") == 0)
		{
		    System.out.println("Usage: CheckedTransactions [-check] [-help]");
		    System.exit(0);
		}
	    }
	    
	    Control tx = null;

	    System.out.println("Thread "+Thread.currentThread()+" starting transaction.");

	    OTS.current().setCheckedAction(new MyCheckedAction());
	    
	    OTS.current().begin();

	    tx = OTS.current().get_control();

	    TXThread txThread = new TXThread(tx);

	    txThread.start();
	    txThread.join();

	    System.out.println("Thread "+Thread.currentThread()+" committing transaction.");

	    OTS.current().commit(false);

	    System.out.println("Transaction committed. Checked transactions enabled.");
	}
	catch (Exception e)
	{
	    System.out.println("Caught exception: "+e);
	    System.out.println("Checked transactions disabled!");
	}

	OAInterface.shutdownOA();
	ORBInterface.shutdownORB();
    }

};
