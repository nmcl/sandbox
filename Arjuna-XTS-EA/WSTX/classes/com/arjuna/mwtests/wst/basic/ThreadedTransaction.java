/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ThreadedTransaction.java,v 1.1 2003/02/19 16:13:05 nmcl Exp $
 */

package com.arjuna.mwtests.wst.basic;

import com.arjuna.mw.wst.UserTransaction;
import com.arjuna.mw.wst.UserTransactionFactory;

/**
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: ThreadedTransaction.java,v 1.1 2003/02/19 16:13:05 nmcl Exp $
 * @since 1.0.
 */

class ThreadedObject extends Thread
{
    
    public ThreadedObject ()
    {
    }

    public void run ()
    {
	try
	{
	    UserTransaction ut = UserTransactionFactory.userTransaction();
	
	    ut.begin();

	    System.out.println("Thread "+Thread.currentThread()+" started "+ut);

	    Thread.yield();
	    
	    System.out.println("\nThread "+Thread.currentThread()+" committing "+ut);
	    
	    ut.commit();
	    
	    

	    Thread.yield();
	}
	catch (Exception ex)
	{
	    ex.printStackTrace();
	}
    }

}

public class ThreadedTransaction
{

    public static void main (String[] args)
    {
	boolean passed = false;
	int size = 10;
	ThreadedObject objs[] = new ThreadedObject[size];
	
	try
	{
	    for (int i = 0; i < size; i++)
		objs[i] = new ThreadedObject();
	    
	    for (int j = 0; j < size; j++)
		objs[j].start();
	    
	    for (int k = 0; k < size; k++)
		objs[k].join();

	    passed = true;
	}
	catch (Exception ex)
	{
	    ex.printStackTrace();
	}
	
	if (passed)
	    System.out.println("\nPassed.");
	else
	    System.out.println("\nFailed.");
    }

}
