/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: AtomicObjectTest2.java,v 1.1 1998/11/12 09:39:15 nmcl Exp $
 */

import JavaArjunaLiteTests.AtomicObject;
import com.arjuna.JavaArjunaLite.Atomic.*;
import java.lang.Thread;
import java.lang.Math;
import java.util.Random;

import JavaArjunaLiteTests.TestException;
import java.lang.InterruptedException;

class ThreadObject1 extends Thread
{

public ThreadObject1 (char c)
    {
	chr = c;
    }

public void run ()
    {
	for (int i = 0; i < 100; i++)
	{
	    AtomicObjectTest2.randomOperation(chr, 0);
	    AtomicObjectTest2.highProbYield();
	}
    }

private char chr;
    
};

public class AtomicObjectTest2
{

public static void main (String[] args)
    {
	rand = new Random();	
	atomicObject1 = new AtomicObject();
	atomicObject2 = new AtomicObject();

	System.out.println(atomicObject1.get_uid());
	System.out.println(atomicObject2.get_uid());

	try
	{
	    atomicObject1.set(10);
	}
	catch (TestException e)
	{
	    System.out.println("m set1 : failed");
	}

	try
	{
	    atomicObject2.set(101);
	}
	catch (TestException e)
	{
	    System.out.println("m set2 : failed");
	}

	ThreadObject1 thr1 = new ThreadObject1('1');
	ThreadObject1 thr2 = new ThreadObject1('2');

	thr1.start();
	thr2.start();

	try
	{
	    thr1.join();
	    thr2.join();
	}
	catch (InterruptedException e)
	{
	}

	get12('m', 0);
	get21('m', 0);
    }

public static void randomOperation (char thr, int level)
    {
	switch (Math.abs(rand.nextInt()) % 6)
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
                AtomicAction a = new AtomicAction();

                a.begin();

                indent(thr, level);
                System.out.println("begin");

	        randomOperation(thr, level + 1);
	        randomOperation(thr, level + 1);

                a.commit();

                indent(thr, level);
                System.out.println("commit");
	    }
            break;
        case 5:
            {
                AtomicAction a = new AtomicAction();

                a.begin();

                indent(thr, level);
                System.out.println("begin");

	        randomOperation(thr, level + 1);
	        randomOperation(thr, level + 1);

                a.abort();

                indent(thr, level);
                System.out.println("abort");
	    }
	break;
	}
    }

public static void lowProbYield()
    {
	while ((Math.abs(rand.nextInt()) % 2) != 0)
	    Thread.yield();
    }

public static void highProbYield()
    {
	while ((Math.abs(rand.nextInt()) % 4) != 0)
	    Thread.yield();
    }
    
private static void indent (char thr, int level)
    {
	System.out.print(thr+" ");
	for (int i = 0; i < level; i++)
	    System.out.print(" ");
    }

private static void incr12 (char thr, int level)
    {
	boolean res  = true;
	boolean res1 = true;
	boolean res2 = true;

	int ran;

	AtomicAction a = new AtomicAction();

	a.begin();

	indent(thr, level);
	System.out.println("begin   incr12");

	ran = Math.abs(rand.nextInt()) % 16;

	try
	{
	    atomicObject1.incr(ran);
	}
	catch (TestException e)
	{
	    res  = res1 = false;
	}

	indent(thr, level);
	System.out.println("part1   incr12 : "+res1);

	lowProbYield();

	if (res)
	{
	    try
	    {
		atomicObject2.incr(-ran);
	    }
	    catch (TestException e)
	    {
		res  = res2 = false;
	    }

	    indent(thr, level);
	    System.out.println("part2   incr12 : "+res2);
	}
	else
	    res2 = false;
	
	lowProbYield();

	indent(thr, level);
	if (res)
	{
	    System.out.print("commit ");
	    res = (boolean) (a.commit() == ActionStatus.COMMITTED);
	}
	else
	{
	    System.out.print("abort  ");
	    a.abort();
	}

	System.out.println(" incr12 : "+res1+" : "+res2+" : "+res+" : "+ran);
    }

private static void incr21 (char thr, int level)
    {
	boolean res  = true;
	boolean res1 = true;
	boolean res2 = true;

	int ran;

	AtomicAction a = new AtomicAction();

	a.begin();

	indent(thr, level);
	System.out.println("begin   incr21");

	ran = Math.abs(rand.nextInt()) % 16;

	try
	{
	    atomicObject2.incr(ran);
	}
	catch (TestException e)
	{
	    res  = res1 = false;
	}

	indent(thr, level);
	System.out.println("part1   incr21 : "+res1);

	lowProbYield();

	if (res)
	{
	    try
	    {
		atomicObject1.incr(-ran);
	    }
	    catch (TestException e)
	    {
		res  = res2 = false;
	    }

	    indent(thr, level);
	    System.out.println("part2   incr21 : "+res2);
	}
	else
	    res2 = false;
	
	lowProbYield();

	indent(thr, level);
	if (res)
	{
	    System.out.print("commit ");
	    res = (boolean) (a.commit() == ActionStatus.COMMITTED);
	}
	else
	{
	    System.out.print("abort  ");
	    a.abort();
	}

	System.out.println(" incr21 : "+res1+" : "+res2+" : "+res+" : "+ran);
    }

private static void get12 (char thr, int level)
    {
	boolean res  = true;
	boolean res1 = true;
	boolean res2 = true;

	int value1 = 0;
	int value2 = 0;

	AtomicAction a = new AtomicAction();

	a.begin();

	indent(thr, level);
	System.out.println("begin   get12");

	try
	{
	    value1 = atomicObject1.get();
	}
	catch (TestException e)
	{
	    res  = res1 = false;
	}

	indent(thr, level);
	System.out.println("part1   get12  : "+res1);

	lowProbYield();

	if (res)
	{
	    try
	    {
		value2 = atomicObject2.get();
	    }
	    catch (TestException e)
	    {
		res  = res2 = false;
	    }

	    indent(thr, level);
	    System.out.println("part2   get12  : "+res2);
	}
	else
	    res2 = false;
	
	lowProbYield();

	indent(thr, level);
	if (res)
	{
	    System.out.print("commit ");
	    res = (boolean) (a.commit() == ActionStatus.COMMITTED);
	}
	else
	{
	    System.out.print("abort  ");
	    a.abort();
	}

	System.out.println(" get12  : "+res1+" : "+res2+" : "+res+" : "+value1+" : "+value2);
    }

private static void get21 (char thr, int level)
    {
	boolean res  = true;
	boolean res1 = true;
	boolean res2 = true;

	int value1 = 0;
	int value2 = 0;

	AtomicAction a = new AtomicAction();

	a.begin();

	indent(thr, level);
	System.out.println("begin   get21");

	try
	{
	    value1 = atomicObject2.get();
	}
	catch (TestException e)
	{
	    res  = res1 = false;
	}

	indent(thr, level);
	System.out.println("part1   get21  : "+res1);

	lowProbYield();

	if (res)
	{
	    try
	    {
		value2 = atomicObject1.get();
	    }
	    catch (TestException e)
	    {
		res  = res2 = false;
	    }

	    indent(thr, level);
	    System.out.println("part2   get21  : "+res2);
	}
	else
	    res2 = false;
	
	lowProbYield();

	indent(thr, level);
	if (res)
	{
	    System.out.print("commit ");
	    res = (boolean) (a.commit() == ActionStatus.COMMITTED);
	}
	else
	{
	    System.out.print("abort  ");
	    a.abort();
	}

	System.out.println(" get21  : "+res1+" : "+res2+" : "+res+" : "+value1+" : "+value2);
    }

private static AtomicObject atomicObject1 = null;
private static AtomicObject atomicObject2 = null;
private static Random rand = null;

}
