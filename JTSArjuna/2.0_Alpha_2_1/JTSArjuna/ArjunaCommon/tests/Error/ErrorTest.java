/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ErrorTest.java,v 1.1.2.1 2000/07/17 09:12:55 nmcl Exp $
 */

import com.arjuna.ArjunaCommon.Common.*;

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
    }
    
};
