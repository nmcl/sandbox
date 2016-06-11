/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: MigrateStubCodeObject.java,v 1.1 1997/07/31 09:13:35 nmcl Exp $
 */

package JavaStubGenerator.migrate;

import JavaStubGenerator.ArjunaII.ArjunaIIStubCodeGenerator;
import JavaStubGenerator.ArjunaII.ArjunaIIStubCodeObject;
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

public class MigrateStubCodeObject extends ArjunaIIStubCodeObject
{

public MigrateStubCodeObject (ArjunaIIStubCodeGenerator parent, String extension)
    {
	super(extension);

	super._parent = parent;
    }

public void addMethodBody (Method method, PrintWriter stream)
    {
	/*
	 * Determine whether local or remote instance is set.
	 */

	stream.println("\tif (_localObject != null)");

	addMethodCode(method, MigrateStubCodeObject.localObject, stream);

	stream.println("\telse");

	addMethodCode(method, MigrateStubCodeObject.remoteObject, stream);

	stream.println(" );");
    }
    
public void addVariables (String interfaceName, PrintWriter stream)
    {
	stream.println("private "+interfaceName+" "+MigrateStubCodeObject.localObject+";");
	stream.println("private "+interfaceName+" "+MigrateStubCodeObject.remoteObject+";");
	stream.println("private boolean _migrating;");	
	stream.println("private ObjectBinder _binder;\n");
    }
    
public void addPackages (String interfaceName, PrintWriter stream)
    {
	stream.println("import "+interfaceName+";");
	stream.println("import JavaStubGenerator.net.ObjectBinder;");
	stream.println("import java.io.*;");
	stream.println("import java.lang.Class;");
	stream.println("import java.lang.reflect.Method;");
	stream.println("import java.lang.reflect.Constructor;\n");

	stream.println("import JavaStubGenerator.stub.StubException;");
	stream.println("import java.rmi.RemoteException;");
	stream.println("import java.lang.SecurityException;");
	stream.println("import java.lang.ClassNotFoundException;");
	stream.println("import java.lang.InstantiationException;");
	stream.println("import java.lang.IllegalAccessException;");
	stream.println("import java.io.IOException;\n");

	stream.flush();
    }
    
public void addSpecials (String interfaceName, String stubClassName, PrintWriter stream)
    {
	/*
	 * Migration specific methods.
	 */

	stream.println("public boolean migrateLocal () throws StubException");
	stream.println("{");
	stream.println("\tboolean result = false;\n");
	
	stream.println("\tif (_remoteObject != null)");
	stream.println("\t{");
	stream.println("\t\tif (_remoteObject.beginMigrate())");
	stream.println("\t\t{");
	stream.println("\t\t\tObjectOutputBuffer buff = new ObjectOutputBuffer(new ByteArrayOutputStream());\n");

	stream.println("\t\t\tif (_remoteObject.pack(buff))");
	stream.println("\t\t\t{");
	stream.println("\t\t\t\t_localObject = _binder.bind(myAddress);  // if local addr then return local object\n");

	stream.println("\t\t\t\tif (_localObject && _localObject.unpack(buff))");
	stream.println("\t\t\t\t\tresult = true;");
	stream.println("\t\t\t}\n");

	stream.println("\t\t\tresult = _remoteObject.endMigrate(myAddress);");
	stream.println("\t\t}");
	stream.println("\t}\n");

	stream.println("\treturn result;");
	stream.println("}\n");

	stream.flush();

	stream.println("public boolean beginMigrate () throws RemoteException, StubException");
	stream.println("{");
	stream.println("\tboolean result = false;\n");

	stream.println("\tif (!migrating)");
	stream.println("\t{");
	stream.println("\t\tif (_localObject != null)");
	stream.println("\t\t{");
	stream.println("\t\t\tmigrating = true;");
	stream.println("\t\t\tresult = true;");
	stream.println("\t\t}");
	stream.println("\t}\n");

	stream.println("\treturn result;");
	stream.println("}\n");

	stream.flush();

	stream.println("public boolean endMigrate (String newAddress) throws RemoteException, StubException");
	stream.println("{");
	stream.println("\tboolean result = false;\n");
	
	stream.println("\tif (migrating)");
	stream.println("\t{");
	stream.println("\t\t_remoteObject = _binder.bind(newAddress);\n");

	stream.println("\t\tif (_remoteObject != null)");
	stream.println("\t\t{");
	stream.println("\t\t\t_localObject = null;");
	stream.println("\t\t\tresult = true;");
	stream.println("\t\t}\n");

	stream.println("\t\tmigrating = false;");
	stream.println("\t}\n");

	stream.println("\treturn result;");
	stream.println("}\n");

	stream.flush();
    }

protected void addMethodCode (Method method, String varName, PrintWriter stream)
    {
	Class[] params = method.getParameterTypes();

	if (method.getReturnType().getName().compareTo(StubCodeGenerator.voidType) != 0)
	    stream.print("\t\treturn ");
	else
	    stream.print("\t\t");
		
	stream.print(varName+"."+method.getName()+"( ");

	for (int l = 0; l < params.length; l++)
	{
	    stream.print(ArjunaIIStubCodeObject.variableName+"_"+l);

	    if (l+1 < params.length)
		stream.print(", ");			 
	}

	stream.println(" );");
    }
    
protected boolean _uniformException = false;
    
private ArjunaIIStubCodeGenerator _parent = null;

private static final String localObject = "_localObject";
private static final String remoteObject = "_remoteObject";
private static final String variableName = "rmivar";
    
}
