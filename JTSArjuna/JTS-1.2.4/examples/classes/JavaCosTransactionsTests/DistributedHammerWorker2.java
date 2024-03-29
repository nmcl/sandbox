/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DistributedHammerWorker2.javatmpl,v 1.2 1998/07/06 13:29:47 nmcl Exp $
 */

/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OrbPortability.h,v 1.4 1998/06/30 16:34:10 nmcl Exp $
 */




/*
 * Try to get around the differences between Ansi CPP and
 * K&R cpp with concatenation.
 */


/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OrbPortability_stdc.h,v 1.2 1998/11/12 08:52:40 nmcl Exp $
 */













































package JavaCosTransactionsTests;

import TestModule.*;
import com.arjuna.OrbCommon.ORBInterface;
import com.arjuna.JavaCosTransactions.*;
import org.omg.CosTransactions.*;
import java.util.Random;
import org.omg.CORBA  .IntHolder;

import JavaCosTransactionsTests.TestException;
import java.lang.InterruptedException;

public class DistributedHammerWorker2
{
    
public static void incr12 (char thr, int level)
    {
	boolean res  = false;
	boolean res1 = false;
	boolean res2 = false;
    
	int ran = 0;

	try
	{
	    OTS.current().begin();
	    Control control = OTS.current().get_control();
    
	    Util.indent(thr, level);
	    System.out.println("begin   incr12");

	    ran = Util.rand.nextInt() % 16;

	    res1 = hammerObject_1.incr(ran, control);
	    res  = res1;

	    Util.indent(thr, level);
	    System.out.println("part1   incr12 : "+res1);

	    Util.lowProbYield();

	    if (res)
	    {
		res2 = hammerObject_2.incr(-ran, control);
		res  = res2;

		Util.indent(thr, level);
		System.out.println("part2   incr12 : "+res2);
	    }

	    Util.lowProbYield();

	    control = null;
	
	    Util.indent(thr, level);
	    if (res)
	    {
		System.out.print("end ");
		OTS.current().commit(true);
	    }
	    else
	    {
		System.out.print("abort  ");
		OTS.current().rollback();
	    }
	}
	catch (Exception e)
	{
	    System.err.println("DistributedHammerWorker2.incr12: "+e);
	    res1 = res2 = res = false;
	}

	System.out.println(" incr12 : "+res1+" : "+res2+" : "+res
			   +" : "+ran);
    }

public static void incr21 (char thr, int level)
    {
	boolean res  = false;
	boolean res1 = false;
	boolean res2 = false;
    
	int ran = 0;

	try
	{
	    OTS.current().begin();
	    Control control = OTS.current().get_control();
    
	    Util.indent(thr, level);
	    System.out.println("begin   incr21");

	    ran = Util.rand.nextInt() % 16;

	    res1 = hammerObject_2.incr(ran, control);
	    res  = res1;

	    Util.indent(thr, level);
	    System.out.println("part1   incr21 : "+res1);

	    Util.lowProbYield();

	    if (res)
	    {
		res2 = hammerObject_1.incr(-ran, control);
		res  = res2;

		Util.indent(thr, level);
		System.out.println("part2   incr21 : "+res2);
	    }

	    Util.lowProbYield();

	    control = null;
    
	    Util.indent(thr, level);
	    if (res)
	    {
		System.out.print("end ");
		OTS.current().commit(true);
	    }
	    else
	    {
		System.out.print("abort  ");
		OTS.current().rollback();
	    }
	}
	catch (Exception e)
	{
	    System.err.println("DistributedHammerWorker2.incr21: "+e);
	    res1 = res2 = res = false;
	}

	System.out.println(" incr21 : "+res1+" : "+res2+" : "+res
			   +" : "+ran);
    }

public static void get12 (char thr, int level)
    {
	boolean res  = false;
	boolean res1 = false;
	boolean res2 = false;
    
	IntHolder value1 = new IntHolder(0);
	IntHolder value2 = new IntHolder(0);

	try
	{
	    OTS.current().begin();
	    Control control = OTS.current().get_control();
    
	    Util.indent(thr, level);
	    System.out.println("begin   get12");

	    res1 = hammerObject_1.get(value1, control);
	    res  = res1;

	    Util.indent(thr, level);
	    System.out.println("part1   get12  : "+res1);

	    Util.lowProbYield();

	    if (res)
	    {
		res2 = hammerObject_2.get(value2, control);
		res  = res2;

		Util.indent(thr, level);
		System.out.println("part2   get12  : "+res2);
	    }

	    Util.lowProbYield();

	    control = null;
    
	    Util.indent(thr, level);
	    if (res)
	    {
		System.out.print("end ");
		OTS.current().commit(true);
	    }
	    else
	    {
		System.out.print("abort  ");
		OTS.current().rollback();
	    }
	}
	catch (Exception e)
	{
	    System.err.println("DistributedHammerWorker2.get12: "+e);
	    res1 = res2 = res = false;
	}

	System.out.println(" get12  : "+res1+" : "+res2+" : "+res
			   +" : "+value1.value+" : "+value2.value);
    }

public static void get21 (char thr, int level)
    {
	boolean res  = false;
	boolean res1 = false;
	boolean res2 = false;
    
	IntHolder value1 = new IntHolder(0);
	IntHolder value2 = new IntHolder(0);

	try
	{
	    OTS.current().begin();
	    Control control = OTS.current().get_control();
    
	    Util.indent(thr, level);
	    System.out.println("begin   get21");

	    res1 = hammerObject_2.get(value1, control);
	    res  = res1;

	    Util.indent(thr, level);
	    System.out.println("part1   get21  : "+res1);

	    Util.lowProbYield();

	    if (res)
	    {
		res2 = hammerObject_1.get(value2, control);
		res  = res2;
	
		Util.indent(thr, level);
		System.out.println("part2   get21  : "+res2);
	    }

	    Util.lowProbYield();

	    control = null;
	    
	    Util.indent(thr, level);
	    if (res)
	    {
		System.out.print("end ");
		OTS.current().commit(true);
	    }
	    else
	    {
		System.out.print("abort  ");
		OTS.current().rollback();
	    }
	}
	catch (Exception e)
	{
	    System.err.println("DistributedHammerWorker2.get21: "+e);
	    res1 = res2 = res = false;
	}

	System.out.println(" get21  : "+res1+" : "+res2+" : "+res
			   +" : "+value1.value+" : "+value2.value);
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
		    OTS.current().begin();

		    Util.indent(thr, level);
		    System.out.println("begin");

		    randomOperation(thr, level + 1);
		    randomOperation(thr, level + 1);

		    OTS.current().commit(true);

		    Util.indent(thr, level);
		    System.out.println("end");
		}
		catch (Exception e)
		{
		    System.err.println("DistributedHammerWorker2.randomOperation: "+e);
		}
	    }
	break;
        case 5:
            {
		try
		{
		    OTS.current().begin();

		    Util.indent(thr, level);
		    System.out.println("begin");

		    randomOperation(thr, level + 1);
		    randomOperation(thr, level + 1);

		    OTS.current().rollback();

		    Util.indent(thr, level);
		    System.out.println("abort");
		}
		catch (Exception e)
		{
		    System.err.println("DistributedHammerWorker2.randomOperation: "+e);
		}		
	    }
	break;
	}
    }

public static Hammer hammerObject_1 = null;
public static Hammer hammerObject_2 = null;

};
