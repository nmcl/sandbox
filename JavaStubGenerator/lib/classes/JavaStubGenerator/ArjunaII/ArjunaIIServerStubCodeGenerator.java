/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ArjunaIIServerStubCodeGenerator.java,v 1.1 1997/07/31 09:13:30 nmcl Exp $
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

public class ArjunaIIServerStubCodeGenerator extends ArjunaIIStubCodeObject
{

public ArjunaIIServerStubCodeGenerator (ArjunaIIStubCodeGenerator parent)
    {
	super(serverExtension);

	super._parent = parent;
    }
    
public void addSpecials (String interfaceName, String stubClassName, PrintWriter stream)
    {
	stream.println("public static void main (String args[])");
	stream.println("{");
	stream.println("\tString objectName = null;");
	stream.println("\tString binderImpl = null;");
	stream.println("\tString theRealClass = null;\n");

	stream.println("\tfor (int i = 0; i < args.length; i++)");
	stream.println("\t{");
	stream.println("\t\tif (args[i].compareTo(\"-name\") == 0)");
	stream.println("\t\t\tobjectName = args[i+1];");
	stream.println("\t\tif (args[i].compareTo(\"-class\") == 0)");
	stream.println("\t\t\ttheRealClass = args[i+1];");
	stream.println("\t\tif (args[i].compareTo(\"-binder\") == 0)");
	stream.println("\t\t\tbinderImpl = args[i+1];");
	stream.println("\t}\n");

	stream.println("\tif (objectName == null)");
	stream.println("\t{");
	stream.println("\t\tSystem.err.println(\"Error - no object name specified.\");");
	stream.println("\t\tSystem.exit(-1);");
	stream.println("\t}\n");

	stream.println("\t// Create and install a security manager\n");
	
	stream.println("\tSystem.setSecurityManager(new RMISecurityManager());\n");

	stream.println("\ttry");
	stream.println("\t{");
	stream.println("\t\t"+stubClassName+" obj = new "+stubClassName+"(binderImpl, theRealClass);");
	stream.println("\t\tNaming.rebind(\"//tarry-f.ncl.ac.uk/\"+objectName, obj);");
	stream.println("\t\tSystem.out.println(objectName+\" bound in registry\");");
	stream.println("\t}");
	stream.println("\tcatch (Exception e)");
	stream.println("\t{");
	stream.println("\t\tSystem.out.println(objectName+\" err: \" + e.getMessage());");
	stream.println("\t\te.printStackTrace();");
	stream.println("\t}");
	stream.println("}\n");

	stream.flush();
    }

public static final String serverExtension = "Server";
    
}
