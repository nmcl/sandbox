/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: HelloImpl.java,v 1.1 1997/07/31 09:13:04 nmcl Exp $
 */

import java.rmi.RemoteException;
import java.io.IOException;

public class HelloImpl implements Hello
{

public HelloImpl () throws RemoteException
    {
    }
    
public HelloImpl (String s) throws RemoteException
    {
	super();
	
	name = s;
    }

public int foobar (long a) throws RemoteException
    {
	return 0;
    }
    
public String sayHello () throws RemoteException
    {
	return "Hello World!";
    }

private String name;

}
