/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: SimpleRpcServerStubCodeGenerator.java,v 1.1 1997/08/05 14:23:12 nmcl Exp $
 */

package JavaStubGenerator.Arjuna.simplerpc;

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

public class SimpleRpcStubCodeObject implements StubCodeObject
{

public SimpleRpcStubCodeObject (String extension)
    {
	_extension = extension;
    }
    
public void exceptionType (boolean uniform)
    {
	_uniformException  = uniform;
    }
    
public void produceStubCode (String interfaceName, Method[] methods) throws IOException
    {
	FileOutputStream stubFile = new FileOutputStream(interfaceName+_extension+StubCodeGenerator.javaExtension);
	PrintWriter stream = new PrintWriter(stubFile);
	String stubClassName = interfaceName+_extension;
	
	_parent.producePreamble(stream);

	/*
	 * Include any packages and exceptions we may need.
	 */

	addPackages(interfaceName, stream);

	/*
	 * Now add the constructor, which will do the remote binding for us.
	 */
	
	addConstructor(interfaceName, stubClassName, stream);

	stream.flush();
	
	if (methods.length > 0)
	{
	    for (int i = 0; i < methods.length; i++)
		addMethod(methods[i], stream);
	}

	/*
	 * Now any specials.
	 */

	addSpecials(interfaceName, stubClassName, stream);
	
	/*
	 * Now any variables we need.
	 */

	addVariables(interfaceName, stream);
	
	stream.println("}");

	stream.flush();
    }    

public void produceStubCode (String interfaceName, Constructor[] constructors, Method[] methods) throws IOException
    {
	System.out.println("Warning - produceStubCode ignoring constructors.");

	produceStubCode(interfaceName, methods);
    }

public void addPackages (String interfaceName, PrintWriter stream)
    {
	stream.println("import "+interfaceName+";");
	stream.println("import JavaStubGenerator.net.ObjectBinder;");
	stream.println("import java.lang.Class;");
	stream.println("import java.lang.reflect.Method;");
	stream.println("import java.lang.reflect.Constructor;\n");
	stream.println("import java.rmi.*;");
	stream.println("import java.rmi.server.*;");

	stream.println("import JavaStubGenerator.stub.StubException;");
	stream.println("import java.rmi.RemoteException;");
	stream.println("import java.lang.SecurityException;");
	stream.println("import java.lang.ClassNotFoundException;");
	stream.println("import java.lang.InstantiationException;");
	stream.println("import java.lang.IllegalAccessException;");
	stream.println("import java.io.IOException;\n");

	stream.flush();
    }

public void addConstructor (String interfaceName, String stubClassName, PrintWriter stream)
    {
	stream.println("public class "+stubClassName+" implements "+interfaceName);
	stream.println("{");

	if (!_uniformException)
	    stream.println("\npublic "+stubClassName+" (String binderImpl, String binderInfo) throws RemoteException, InstantiationException");
	else
	    stream.println("\npublic "+stubClassName+" (String binderImpl, String binderInfo) throws StubException, InstantiationException");
	
	stream.println("{");
	stream.println("\ttry");
	stream.println("\t{");
	stream.println("\t\tClass binderClass = Class.forName(binderImpl);");
	stream.println("\t\tObject o = binderClass.newInstance();\n");
	
	stream.println("\t\tif (!(o instanceof ObjectBinder))");
	stream.println("\t\t\tthrow new InstantiationException(\"Class \"+binderImpl+\" is not an ObjectBinder!\");\n");

	stream.println("\t\t_binder = (ObjectBinder) o;\n");
	
	stream.println("\t\t_obj = ("+interfaceName+") _binder.bind(binderInfo);");
	    
	stream.println("\t}");
	stream.println("\tcatch (Exception e)");
	stream.println("\t{");

	if (!_uniformException)
	    stream.println("\t\tthrow new RemoteException(e.toString());");
	else
	    stream.println("\t\tthrow new StubException(e.toString());");
	
	stream.println("\t}");
	stream.println("}");
	stream.println("");

	stream.flush();
    }

public void addMethod (Method method, PrintWriter stream)
    {
	Class[] params = method.getParameterTypes();

	/*
	 * First add the method signature. This should be the same for all
	 * implementations. The body of the method may change.
	 */
	
	stream.print("public synchronized "+method.getReturnType().getName()+" "+method.getName());
	stream.print(" ( ");

	if (params.length > 0)
	{
	    for (int j = 0; j < params.length; j++)
	    {
		stream.print(params[j].getName() + " " + variableName+"_"+j);

		if (j+1 < params.length)
		    stream.print(", ");
	    }
	}

	stream.print(" ) ");

	if (method.getExceptionTypes().length > 0)
	{
	    Class[] excepts = method.getExceptionTypes();

	    stream.print("throws ");
		    
	    for (int k = 0; k < excepts.length; k++)
	    {
		stream.print(excepts[k].getName());

		if (k+1 < excepts.length)
		    stream.print(", ");
	    }
	}

	stream.println();
	stream.println("{");

	/*
	 * Now add the body of the method. Can be overridden.
	 */

	addMethodBody(method, stream);

	stream.println("}\n");

	stream.flush();
    }

public void addMethodBody (Method method, PrintWriter stream)
    {
	Class[] params = method.getParameterTypes();
	
	if (method.getReturnType().getName().compareTo(StubCodeGenerator.voidType) != 0)
	    stream.print("\treturn ");
	else
	    stream.print("\t");
		
	stream.print("_obj."+method.getName()+"( ");

	for (int l = 0; l < params.length; l++)
	{
	    stream.print(variableName+"_"+l);

	    if (l+1 < params.length)
		stream.print(", ");			 
	}

	stream.println(" );");
    }
    
public void addVariables (String interfaceName, PrintWriter stream)
    {
	stream.println("private "+interfaceName+" _obj;");
	stream.println("private ObjectBinder _binder;\n");
    }

public void addSpecials (String interfaceName, String stubClassName, PrintWriter stream)
    {
    }

protected boolean _uniformException = false;
protected String  _extension = "";
protected ArjunaIIStubCodeGenerator _parent = null;
    
protected static final String variableName = "rmivar";
    
}
