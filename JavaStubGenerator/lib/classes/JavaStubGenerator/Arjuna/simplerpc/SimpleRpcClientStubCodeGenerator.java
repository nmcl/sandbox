/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: SimpleRpcClientStubCodeGenerator.java,v 1.1 1997/08/05 14:23:11 nmcl Exp $
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

public class SimpleRpcClientStubCodeObject implements StubCodeObject
{

public SimpleRpcStubCodeObject ()
    {
	_extension = "_Server";
    }

public void setInterfaceName (String ifName)
    {
	_interfaceName = ifName;
    }
    
public void exceptionType (boolean uniform)
    {
	_uniformException  = uniform;
    }
    
public void produceStubCode (Constructor[] constructors, Method[] methods) throws IOException
    {
	FileOutputStream stubFile = new FileOutputStream(interfaceName+_extension+StubCodeGenerator.javaExtension);
	PrintWriter stream = new PrintWriter(stubFile);
	String stubClassName = interfaceName+_extension;
	
	_parent.producePreamble(stream);

	/*
	 * Include any packages and exceptions we may need.
	 */

	addPackages(stream);

	addClassDeclaration(stubClassName, stream);
	
	/*
	 * Now add the default constructor, which will do the remote binding for us.
	 */
	
	addConstructor(stubClassName, stream);
	    
	stream.flush();

	if ((constructors != null) && (constructors.length > 0))
	{
	    for (int j = 0; j < constructors.length; j++)
		addConstructor(constructor[i], stream);
	}
	
	if (methods.length > 0)
	{
	    for (int i = 0; i < methods.length; i++)
		addMethod(methods[i], stream);
	}

	/*
	 * Now any specials.
	 */

	addSpecials(stubClassName, stream);
	
	/*
	 * Now any variables we need.
	 */

	addVariables(stream);
	
	stream.println("}");

	stream.flush();
    }    

public void produceStubCode (String interfaceName, Constructor[] constructors, Method[] methods) throws IOException
    {
	produceStubCode(interfaceName, null, methods);
    }

public void addPackages (String interfaceName, PrintWriter stream)
    {
	stream.println("import "+interfaceName+";");
	stream.println("import JavaStubGenerator.net.ObjectBinder;");
	stream.println("import JavaArjuna.StubGen.*;");
	stream.println("import java.lang.Class;");

	stream.println("import JavaStubGenerator.stub.StubException;");
	stream.println("import java.lang.SecurityException;");
	stream.println("import java.lang.ClassNotFoundException;");
	stream.println("import java.lang.InstantiationException;");
	stream.println("import java.lang.IllegalAccessException;");
	stream.println("import java.io.IOException;\n");

	stream.flush();
    }

public void addClassDeclaration (String stubClassName, PrintWriter stream)
    {
	stream.println("public class "+stubClassName+" implements "+_interfaceName);
	stream.println("{");
    }
    
public void addConstructor (String stubClassName, PrintWriter stream)
    {
	stream.println("\npublic "+stubClassName+" (ClientRpcManager _cRpcMan_) throws IOException");

	stream.println("{");

	setupCall(stream);

	makeCall(stream);

	stream.println("\tif (_rpcStatus == RPC_Status.OPER_DONE)");
	stream.println("\t{");
	stream.println("\t\tswitch (_serverStatus.get())");
	stream.println("\t\t{");
	stream.println("\t\t\tcase ServerErrCode.OPER_INVOKED_OK:");
	stream.println("\t\t\t\tbreak;");
	stream.println("\t\t\tdefault:");
	stream.println("\t\t\t\t_clientManager.rpcAbort(_rpcStatus_, _serverStatus_.get());");
	stream.println("\t\t}");
	stream.println("\t}");
	stream.println("\telse");
	stream.println("\t\t_clientManager.rpcAbort(_rpcStatus_, _serverStatus.get());");
	stream.println("\t}");

	stream.flush();
    }

public void addConstructor (Constructor constructor, PrintWriter stream)
    {
	Class[] params = constructor.getParameterTypes();
	
	stream.print("\npublic "+constructor.getName()+" ( ");

	if (params.length > 0)
	{
	    printVariableList(params, stream);

	    stream.print(", ");
	}

	stream.println("ClientRpcManager _cRpcMan_ ) throws IOException");

	stream.println("{");

	setupCall(stream);

	makeCall(stream);

	stream.println("\tif (_rpcStatus == RPC_Status.OPER_DONE)");
	stream.println("\t{");
	stream.println("\t\tswitch (_serverStatus.get())");
	stream.println("\t\t{");
	stream.println("\t\t\tcase ServerErrCode.OPER_INVOKED_OK:");
	stream.println("\t\t\t\tbreak;");
	stream.println("\t\t\tdefault:");
	stream.println("\t\t\t\t_clientManager.rpcAbort(_rpcStatus_, _serverStatus_.get());");
	stream.println("\t\t}");
	stream.println("\t}");
	stream.println("\telse");
	stream.println("\t\t_clientManager.rpcAbort(_rpcStatus_, _serverStatus.get());");
	stream.println("\t}");

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
	out.println(" * $Id: SimpleRpcClientStubCodeGenerator.java,v 1.1 1997/08/05 14:23:11 nmcl Exp $");
	out.println(" */\n");
	
	out.println("// Automatically generated by ArjunaII Java Stub Compiler.");
	out.println("// WARNING: do not edit by hand.\n");

	out.flush();
    }

protected void setupCall (PrintWriter stream)
    {
	stream.println("\t_clientManager = _cRpcMan_;");
	stream.println("\t_myHashVal = new OutputBuffer();");
	stream.println("\t_myHashVal.packInt(-1);\n");

	stream.println("\tOutputBuffer _callBuffer_ = new OutputBuffer(_myHashVal);");
	stream.println("\tInputBuffer _replyBuffer_ = null;");
	stream.println("\tint _rpcStatus_ = RPC_Status.OPER_UNKNOWN;");
	stream.println("\tintWrapper _serverStatus_ = new intWrapper(ServerErrCode.OPER_INVOKED_OK);\n");
    }

protected void makeCall (PrintWriter stream)
    {
	stream.println("\t_rpcStatus_ = RPC_Status.OPER_DONE;");
	stream.println("\ttry");
	stream.println("\t{");
	stream.println("\t\t_replyBuffer_ = clientManager.call("+opcode()+", _callBuffer_, _serverStatus_);");
	stream.println("\tcatch (IOException e)");
	stream.println("\t{");
	stream.println("\t\t_rpcStatus_ = RPC_Status.OPER_NOTDONE;");
	stream.println("\t}\n");
    }

protected printVariableList (Class[] params, PrintWriter stream)
    {
	for (int j = 0; j < params.length; j++)
	{
	    stream.print(params[j].getName() + " " + variableName+"_"+j);

	    if (j+1 < params.length)
		stream.print(", ");
	}
    }
    
protected boolean _uniformException = false;
protected String  _extension = "";
protected ArjunaIIStubCodeGenerator _parent = null;
protected String _interfaceName = "";
    
protected static final String variableName = "rmivar";

private int opcode ()
    {
	return _opcode++;
    }

private boolean isPackable (Class c)
    {
    }
    
private int _opcode = 0;
	
}
