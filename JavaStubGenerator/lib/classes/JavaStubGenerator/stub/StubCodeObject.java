/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: StubCodeObject.java,v 1.2 1997/08/05 14:43:02 nmcl Exp $
 */

package JavaStubGenerator.stub;

import java.lang.Class;
import java.lang.reflect.Method;
import java.lang.reflect.Constructor;
import java.io.PrintWriter;

import java.io.IOException;

public interface StubCodeObject
{

public void setInterfaceName (String interfaceName);

public void exceptionType (boolean uniform);

public void produceStubCode (Method[] methods) throws IOException;
public void produceStubCode (Constructor[] constructors, Method[] methods) throws IOException;

public void addPackages (PrintWriter stream);

public void addClassDeclaration (String stubClassName, PrintWriter stream);

public void addConstructor (String stubClassName, PrintWriter stream);
public void addConstructor (Constructor constructor, PrintWriter stream);

public void addMethod (Method method, PrintWriter stream);

public void addMethodBody (Method method, PrintWriter stream);
    
public void addVariables (PrintWriter stream);

public void addSpecials (String stubClassName, PrintWriter stream);
    
}
