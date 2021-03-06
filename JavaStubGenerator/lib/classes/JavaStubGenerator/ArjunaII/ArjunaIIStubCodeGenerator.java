/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ArjunaIIStubCodeGenerator.java,v 1.2 1997/08/05 14:35:45 nmcl Exp $
 */

package JavaStubGenerator.ArjunaII;

import JavaStubGenerator.stub.StubCodeGenerator;
import JavaStubGenerator.stub.StubCodeObject;
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

public class ArjunaIIStubCodeGenerator implements StubCodeGenerator
{

public ArjunaIIStubCodeGenerator ()
    {
	clientCode = new ArjunaIIClientStubCodeGenerator(this);
	serverCode = new ArjunaIIServerStubCodeGenerator(this);

	clientCode.setInterfaceName(_interfaceName);
	serverCode.setInterfaceName(_interfaceName);
    }
    
public boolean hasConstructors ()
    {
	System.err.println("Error - interfaces cannot have constructors!");
	
	return false;
    }

public boolean errorCheck (Class c)
    {
	return true;
    }
    
public void exceptionType (boolean uniform)
    {
	_uniformException = uniform;

	clientCode.exceptionType(uniform);
	serverCode.exceptionType(uniform);
    }
    
public void produceClientStubCode (Method[] methods) throws IOException
    {
	clientCode.produceStubCode(methods);
    }    

public void produceServerStubCode (Method[] methods) throws IOException
    {
	serverCode.produceStubCode(methods);
    }

public void produceClientStubCode (Constructor[] constructors, Method[] methods) throws IOException
    {
	System.out.println("Warning - produceClientStubCode ignoring constructors.");

	produceClientStubCode(methods);
    }

public void produceServerStubCode (Constructor[] constructor, Method[] methods) throws IOException
    {
	System.out.println("Warning - produceServerStubCode ignoring constructors.");

	produceServerStubCode(methods);
    }

public void printClass (Constructor[] constructors, Method[] methods, PrintWriter out) throws IOException
    {
	out.println("interface "+_interfaceName);
	out.println("{");

	if (methods.length > 0)
	{
	    for (int i = 0; i < methods.length; i++)
		out.println(methods[i]);
	}

	out.println("}");

	out.flush();
    }

public void producePreamble (PrintWriter out) throws IOException
    {
	out.println("/*");
	out.println(" * Copyright (C) 1997,");
	out.println(" *");
	out.println(" * Department of Computing Science,");
	out.println(" * University of Newcastle upon Tyne,");
	out.println(" * Newcastle upon Tyne,");
	out.println(" * UK.");
	out.println(" *");
	out.println(" * $Id: ArjunaIIStubCodeGenerator.java,v 1.2 1997/08/05 14:35:45 nmcl Exp $");
	out.println(" */\n");
	
	out.println("// Automatically generated by ArjunaII Java Stub Compiler.");
	out.println("// WARNING: do not edit by hand.\n");

	out.flush();
    }

public void setInterfaceName (String interfaceName)
    {
	_interfaceName = interfaceName;
    }
    
protected boolean _uniformException = false;
protected StubCodeObject clientCode = null;
protected StubCodeObject serverCode = null;
protected String _interfaceName = "";
    
}
