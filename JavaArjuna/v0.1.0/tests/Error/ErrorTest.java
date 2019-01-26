/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ErrorTest.java,v 1.1.2.1.24.1 2000/11/21 11:47:54 nmcl Exp $
 */


public class ErrorTest
{
    
public static void main (String[] args)
    {
	ErrorStream.stream().println("Basic test of error stream.");
	ErrorStream.stream().flush();
	
	ErrorStream.stream("\n", ErrorStream.WARNING).println("Another example");
	ErrorStream.stream().flush();

	ErrorStream.stream("\n", ErrorStream.FATAL).println("And another!");
	ErrorStream.stream().flush();

	for (int i = 0; i < 10; i++)
	{
	    try
	    {
		Thread.currentThread().sleep(1000);
	    }
	    catch (Exception e)
	    {
	    }
	    
	    if (i % 2 == 0)
		ErrorStream.fatal().println(i);
	    else
		ErrorStream.warning().println(i);
	}
    }
    
}
