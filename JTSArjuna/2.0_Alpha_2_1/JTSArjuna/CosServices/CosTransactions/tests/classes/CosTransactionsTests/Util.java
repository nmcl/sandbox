/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: Util.java,v 1.1 2000/02/25 14:08:55 nmcl Exp $
 */

package CosTransactionsTests;

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
