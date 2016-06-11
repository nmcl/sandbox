/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: StubCodeGenerator.java,v 1.2 1997/08/05 14:43:01 nmcl Exp $
 */

package JavaStubGenerator.stub;

import java.lang.reflect.Method;
import java.lang.reflect.Constructor;
import java.io.PrintWriter;
import java.io.FileOutputStream;

import java.io.IOException;

public interface StubCodeGenerator
{

public void setInterfaceName (String interfaceName);

public void produceClientStubCode (Method[] methods) throws IOException;

public void produceClientStubCode (Constructor[] constructors, Method[] methods) throws IOException;

public void produceServerStubCode (Method[] methods) throws IOException;

public void produceServerStubCode (Constructor[] constructor, Method[] methods) throws IOException;

public void producePreamble (PrintWriter out) throws IOException;

public void printClass (Constructor[] constructors, Method[] methods, PrintWriter out) throws IOException;
    
public boolean hasConstructors ();

public boolean errorCheck (Class c);  // check things like type, exceptions etc.

public void exceptionType (boolean e);

public static final String javaExtension = ".java";
public static final String voidType = "void";

}
