/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: StubGenerator.java,v 1.2 1997/08/05 14:21:03 nmcl Exp $
 */

import JavaStubGenerator.stub.*;
import java.lang.Class;
import java.lang.reflect.Method;
import java.lang.reflect.Constructor;
import java.io.PrintWriter;
import java.io.FileOutputStream;

import java.io.IOException;
import java.lang.SecurityException;
import java.lang.ClassNotFoundException;
import java.lang.InstantiationException;
import java.lang.IllegalAccessException;

public class StubGenerator
{

public static void main (String args[])
    {
	String interfaceName = null;
	String stubObject = "JavaStubGenerator.ArjunaII.ArjunaIIStubCodeGenerator";
	boolean uniformException = false;
	
	for (int i = 0; i < args.length; i++)
	{
	    if (args[i].compareTo("-debug") == 0)
		debugging = true;
	    if (args[i].compareTo("-interface") == 0)
		interfaceName = args[i+1];
	    if (args[i].compareTo("-stubcode") == 0)
		stubObject = args[i+1];
	    if (args[i].compareTo("-uniformexception") == 0)
		uniformException = true;
	    if (args[i].compareTo("-help") == 0)
	    {
		System.out.println("Usage: -interface <name> [-stubcode <stubcode object>] [-uniformexception] [-debug] [-help]");
		System.exit(0);
	    }
	}

	if (interfaceName == null)
	{
	    System.err.println("Error - no interface name specified.");
	    System.exit(0);
	}

	try
	{
	    /*
	     * This is a protocol engine for the stub generator. So, first
	     * we create the real stub generator.
	     */
	    
	    Class stubGeneratorClass = Class.forName(stubObject);
	    Object obj = stubGeneratorClass.newInstance();

	    if (!(obj instanceof StubCodeGenerator))
	    {
		System.err.println("Error - "+stubObject+" is not a StubCodeGenerator!");
		System.exit(-1);
	    }

	    StubCodeGenerator stubGen = (StubCodeGenerator) obj;

	    stubGen.exceptionType(uniformException);
	    stubGen.setInterfaceName(interfaceName);
	    
	    /*
	     * Now instantiate the interface class.
	     */
	    
	    Class interfaceClass = Class.forName(interfaceName);

	    Constructor[] constructors = interfaceClass.getDeclaredConstructors(); // should be 0 for an interface!

	    if (constructors.length != 0)
	    {
		if (!stubGen.hasConstructors())
		    System.exit(-1);
	    }

	    if (!stubGen.errorCheck(interfaceClass))
		System.exit(-1);
	    
	    Method[] methods = interfaceClass.getMethods();

	    if (debugging)
		stubGen.printClass(constructors, methods, new PrintWriter(System.out));

	    if (constructors.length != 0)
	    {
		stubGen.produceClientStubCode(constructors, methods);
		stubGen.produceServerStubCode(constructors, methods);
	    }
	    else
	    {
		stubGen.produceClientStubCode(methods);
		stubGen.produceServerStubCode(methods);		
	    }
	}
	catch (SecurityException e1)
	{
	    System.err.println(e1);
	}
	catch (ClassNotFoundException e2)
	{
	    System.err.println(e2);
	}
	catch (IOException e3)
	{
	    System.err.println(e3);
	}
	catch (InstantiationException e4)
	{
	    System.err.println(e4);
	}
	catch (IllegalAccessException e5)
	{
	    System.err.println(e5);
	}
    }

private static boolean debugging = false;
    
}
