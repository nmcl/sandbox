/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: MigrateStubCodeGenerator.java,v 1.1 1997/07/31 09:13:35 nmcl Exp $
 */

package JavaStubGenerator.migrate;

import JavaStubGenerator.ArjunaII.ArjunaIIStubCodeGenerator;
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


public class MigrateStubCodeGenerator extends ArjunaIIStubCodeGenerator
{

public MigrateStubCodeGenerator ()
    {
	clientCode = new MigrateStubCodeObject(this, clientExtension);
	serverCode = new MigrateStubCodeObject(this, serverExtension);
    }

public boolean errorCheck (Class c)
    {
	Class[] interfaces = c.getInterfaces();
	String migrateClass = "JavaStubGenerator.migrate.Migrate";
	
	if (interfaces.length > 0)
	{
	    for (int i = 0; i < interfaces.length; i++)
	    {
		if (interfaces[i].getName().compareTo(migrateClass) == 0)
		    return true;
	    }
	}

	System.err.println("Error "+c+" is not of type "+migrateClass);
	return false;
    }
    
public static final String clientExtension = "ClientMigrate";
public static final String serverExtension = "ServerMigrate";
    
}
