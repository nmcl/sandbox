/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: AtomicWorker1.java,v 1.2 1998/07/06 13:29:41 nmcl Exp $
 */

package JavaCosTransactionsTests;

import JavaCosTransactionsTests.*;
import TestModule.*;
import com.arjuna.OrbCommon.ORBInterface;
import com.arjuna.JavaCosTransactions.*;
import java.util.Random;

public class AtomicWorker1
{
    
public static void incr12 (char thr, int level)
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
	    System.out.println("part1   incr12 : "+res1);

	    if (res)
	    {
		res2 = atomicObject_2.incr(-ran);
		res  = res2;

		Util.indent(thr, level);
		System.out.println("part2   incr12 : "+res2);
	    }
		 
	    Util.indent(thr, level);
    
	    if (res)
	    {
		System.out.print("end ");

		current.commit(false);
		res = true;
	    }
	    else
	    {
		System.out.print("abort  ");
		current.rollback();
	    }

	    System.out.println(" incr12 : "+res1+" : "+res2+" : "+res
			       +" : "+ran);
	}
	catch (Exception e1)
	{
	    System.err.println(e1);
	}
    }

public static void incr21 (char thr, int level)
    {
	boolean res  = false;
	boolean res1 = false;
	boolean res2 = false;

	int ran;

	try
	{
	    current.begin();

	    Util.indent(thr, level);
	    System.out.println("begin   incr21");

	    ran = Util.rand.nextInt() % 16;

	    res1 = atomicObject_2.incr(ran);
	    res  = res1;

	    Util.indent(thr, level);
	    System.out.println("part1   incr21 : "+res1);
	
	    if (res)
	    {
		res2 = atomicObject_1.incr(-ran);
		res  = res2;

		Util.indent(thr, level);
		System.out.println("part2   incr21 : "+res2);
	    }

	    Util.indent(thr, level);
	
	    if (res)
	    {
		System.out.print("end ");
		current.commit(false);
		res = true;
	    }
	    else
	    {
		System.out.print("abort  ");
		current.rollback();
	    }

	    System.out.println(" incr21 : "+res1+" : "+res2+" : "+res
			       +" : "+ran);
	}
	catch (Exception e)
	{
	    System.err.println(e);
	}
    }

public static void get12 (char thr, int level)
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
	    System.out.println("part1   get12  : "+res1);

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
		System.out.println("part2   get12  : "+res2);
	    }

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

	    System.out.println(" get12  : "+res1+" : "+res2+" : "+res
			       +" : "+value1+" : "+value2);
	}
	catch (Exception e)
	{
	    System.err.println(e);
	}
    }

public static void get21 (char thr, int level)
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
	    System.out.println("begin   get21");

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
	    System.out.println("part1   get21  : "+res1);

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
		System.out.println("part2   get21  : "+res2);
	    }

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

	    System.out.println(" get21  : "+res1+" : "+res2+" : "+res
			       +" : "+value1+" : "+value2);
	}
	catch (Exception e)
	{
	    System.err.println(e);
	}
    }

public static void randomOperation (char thr, int level)
    {
	switch (Util.rand.nextInt() % 6)
	{
        case 0:
            incr12(thr, level);
            break;
        case 1:
            incr21(thr, level);
            break;
        case 2:
            get12(thr, level);
            break;
        case 3:
            get21(thr, level);
            break;
        case 4:
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
        case 5:
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
		    System.out.println("abort");
		}
		catch (Exception e)
		{
		    System.err.println(e);
		}
	    }
	break;
	}
    }
    
public static AtomicObject atomicObject_1 = null;
public static AtomicObject atomicObject_2 = null;
public static OTS_Current current = null;

};
