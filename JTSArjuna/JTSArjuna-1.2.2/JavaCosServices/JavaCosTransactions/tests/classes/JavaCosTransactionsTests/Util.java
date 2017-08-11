/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Util.java,v 1.1 1998/07/06 08:58:36 nmcl Exp $
 */

package JavaCosTransactionsTests;

import java.lang.Thread;
import java.util.Random;

public class Util
{

public static void indent (char thr, int level)
    {
	System.out.print(thr+" ");
	
	for (int i = 0; i < level; i++)
	    System.out.print(" ");
    }

public static void indent (int thr, int level)
    {
	System.out.print(thr+" ");
	
	for (int i = 0; i < level; i++)
	    System.out.print(" ");
    }    
    
public static void lowProbYield ()
    {
	while ((rand.nextInt() % 2) != 0)
	    Thread.yield();
    }
    
public static void highProbYield ()
    {
	while ((rand.nextInt() % 4) != 0)
	    Thread.yield();
    }
    
public static Random rand = new Random();
    
};
