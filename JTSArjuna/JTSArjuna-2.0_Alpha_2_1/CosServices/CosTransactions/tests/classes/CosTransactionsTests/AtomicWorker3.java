/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: AtomicWorker3.java,v 1.1 2000/02/25 14:08:45 nmcl Exp $
 */

package CosTransactionsTests;

import TestModule.*;
import com.arjuna.OrbCommon.ORBInterface;
import com.arjuna.CosTransactions.*;
import java.util.Random;

import CosTransactionsTests.TestException;
import java.lang.InterruptedException;

public class AtomicWorker3
{

public static void randomOperation (int thr, int level)
    {
	switch (Util.rand.nextInt() % 23)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	    incr12(thr, level);
	    break;
	case 4:
	case 5:
	case 6:
	case 7:
	    incr21(thr, level);
	    break;
	case 8:
	case 9:
	case 10:
	case 11:
	    get12(thr, level);
	    break;
	case 12:
	case 13:
	case 14:
	case 15:
            get21(thr, level);
            break;
        case 16:
        case 17:
            {
		try
		{
		    current.begin();

		    Util.indent(thr, level);
		    System.out.println("begin");

		    randomOperation(thr, level + 1);
		    randomOperation(thr, level + 1);

		    current.commit(false);

		    Util.indent(thr, level);
		    System.out.println("end");
		}
		catch (Exception e)
		{
		    System.err.println(e);
		}
	    }
	break;
        case 18:
        case 19:
            {
		try
		{
		    current.begin();
		
		    Util.indent(thr, level);
		    System.out.println("begin");

		    randomOperation(thr, level + 1);
		    randomOperation(thr, level + 1);

		    current.rollback();

		    Util.indent(thr, level);
		    System.out.print("abort");
		}
		catch (Exception e)
		{
		    System.err.println(e);
		}
	    }
	break;
	case 20:
            {
                Thread thr1 = null;
                Thread thr2 = null;

                Util.indent(thr, level);
                System.out.println("fork");

		thr1 = new ThreadObject3a(false);
		thr2 = new ThreadObject3a(false);

		thr1.start();
		thr2.start();

		try
		{
		    thr1.join();
		    thr2.join();
		}
		catch (InterruptedException e)
		{
		    System.err.println(e);
		}

                Util.indent(thr, level);
                System.out.println("join");
	    }
	break;
	case 21:
            {
                Thread thr1 = null;
                Thread thr2 = null;

                Util.indent(thr, level);
                System.out.println("fork");

		thr1 = new ThreadObject3a(true);
		thr2 = new ThreadObject3a(false);

		thr1.start();
		thr2.start();

		try
		{
		    thr1.join();
		    thr2.join();
		}
		catch (InterruptedException e)
		{
		    System.err.println(e);
		}

                Util.indent(thr, level);
                System.out.println("join");
	    }
            break;
	case 22:
            {
                Thread thr1 = null;
                Thread thr2 = null;

                Util.indent(thr, level);
                System.out.println("fork");

		thr1 = new ThreadObject3a(true);
		thr2 = new ThreadObject3a(true);

		thr1.start();
		thr2.start();

		try
		{
		    thr1.join();
		    thr2.join();
		}
		catch (InterruptedException e)
		{
		    System.err.println(e);
		}
                    
                Util.indent(thr, level);
                System.out.println("join");
	    }
	break;
	}
    }
    
public static void incr12 (int thr, int level)
    {
	boolean res  = false;
	boolean res1 = false;
	boolean res2 = false;

	int ran;

	try
	{
	    current.begin();

	    Util.indent(thr, level);
	    System.out.println("begin   incr12");

	    ran = Util.rand.nextInt() % 16;

	    res1 = atomicObject_1.incr(ran);
	    res  = res1;

	    Util.indent(thr, level);
	    System.out.println("part1   incr12 : " + res1);

	    Util.lowProbYield();

	    if (res)
	    {
		res2 = atomicObject_2.incr(-ran);
		res  = res2;

		Util.indent(thr, level);
		System.out.println("part2   incr12 : " + res2);
	    }

	    Util.lowProbYield();

	    Util.indent(thr, level);
	    if (res)
	    {
		System.out.print("end ");
		current.commit(false);
	    }
	    else
	    {
		System.out.print("abort  ");
		current.rollback();
	    }

	    System.out.println(" incr12 : " + res1 + " : " + res2 + " : " + res
			       + " : " + ran);
	}
	catch (Exception e)
	{
	    System.err.println(e);
	}
    }

public static void incr21 (int thr, int level)
    {
	boolean res  = false;
	boolean res1 = false;
	boolean res2 = false;

	int ran;

	try
	{
	    current.begin();

	    Util.indent(thr, level);
	    System.out.print("begin   incr21");

	    ran = Util.rand.nextInt() % 16;
	
	    res1 = atomicObject_2.incr(ran);
	    res  = res1;

	    Util.indent(thr, level);
	    System.out.print("part1   incr21 : " + res1);
	
	    Util.lowProbYield();

	    if (res)
	    {
		res2 = atomicObject_1.incr(-ran);
		res  = res2;
	    
		Util.indent(thr, level);
		System.out.println("part2   incr21 : " + res2);
	    }

	    Util.lowProbYield();

	    Util.indent(thr, level);
	    if (res)
	    {
		System.out.print("end ");
		current.commit(false);
	    }
	    else
	    {
		System.out.print("abort  ");
		current.rollback();
	    }

	    System.out.println(" incr21 : " + res1 + " : " + res2 + " : " + res
			       + " : " + ran);
	}
	catch (Exception e)
	{
	    System.err.println(e);
	}
    }

public static void get12 (int thr, int level)
    {
	boolean res  = false;
	boolean res1 = false;
	boolean res2 = false;

	int value1 = 0;
	int value2 = 0;

	try
	{
	    current.begin();
	
	    Util.indent(thr, level);
	    System.out.println("begin   get12");

	    res1 = true;
	    
	    try
	    {
		value1 = atomicObject_1.get();
	    }
	    catch (TestException e)
	    {
		res1 = false;
	    }
	    
	    res  = res1;

	    Util.indent(thr, level);
	    System.out.println("part1   get12  : " + res1);

	    Util.lowProbYield();

	    if (res)
	    {
		res2 = true;

		try
		{
		    value2 = atomicObject_2.get();
		}
		catch (TestException e)
		{
		    res2 = false;
		}
		
		res  = res2;

		Util.indent(thr, level);
		System.out.println("part2   get12  : " + res2);
	    }
		 
	    Util.lowProbYield();

	    Util.indent(thr, level);
	    if (res)
	    {
		System.out.print("end ");
		current.commit(false);
	    }
	    else
	    {
		System.out.print("abort  ");
		current.rollback();
	    }

	    System.out.println(" get12  : " + res1 + " : " + res2 + " : " + res
			       + " : " + value1 + " : " + value2);
	}
	catch (Exception e)
	{
	    System.err.println(e);
	}
    }

public static void get21 (int thr, int level)
    {
	boolean res  = false;
	boolean res1 = false;
	boolean res2 = false;

	int value1 = 0;
	int value2 = 0;

	try
	{
	    current.begin();

	    Util.indent(thr, level);
	    System.out.print("begin   get21");

	    res1 = true;

	    try
	    {
		value1 = atomicObject_2.get();
	    }
	    catch (TestException e)
	    {
		res1 = false;
	    }
	    
	    res  = res1;

	    Util.indent(thr, level);
	    System.out.print("part1   get21  : " + res1);

	    Util.lowProbYield();

	    if (res)
	    {
		res2 = true;

		try
		{
		    value2 = atomicObject_1.get();
		}
		catch (TestException e)
		{
		    res2 = false;
		}
		
		res  = res2;

		Util.indent(thr, level);
		System.out.println("part2   get21  : " + res2);
	    }

	    Util.lowProbYield();

	    Util.indent(thr, level);
	    if (res)
	    {
		System.out.print("end ");
		current.commit(false);
	    }
	    else
	    {
		System.out.print("abort  ");
		current.rollback();
	    }

	    System.out.println(" get21  : " + res1 + " : " + res2 + " : " + res
			       + " : " + value1 + " : " + value2);
	}
	catch (Exception e)
	{
	    System.err.println(e);
	}
    }
    
public static AtomicObject atomicObject_1 = null;
public static AtomicObject atomicObject_2 = null;
public static OTS_Current current = null;

};
