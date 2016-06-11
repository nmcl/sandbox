/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ArjunaIIClientStubCodeGenerator.java,v 1.1 1997/07/31 09:13:30 nmcl Exp $
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

public class ArjunaIIClientStubCodeGenerator extends ArjunaIIStubCodeObject
{

public ArjunaIIClientStubCodeGenerator (ArjunaIIStubCodeGenerator parent)
    {
	super(clientExtension);

	super._parent = parent;
    }

public static final String clientExtension = "Client";
    
}
