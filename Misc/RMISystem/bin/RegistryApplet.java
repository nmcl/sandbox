/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RegistryApplet.java,v 1.2 1997/05/31 13:32:00 nmcl Exp $
 */

import java.awt.*;
import java.rmi.*;
import java.rmi.registry.*;

public class RegistryApplet extends java.applet.Applet
{
    
public void init ()
    {
	try
	{
	    Registry reg = (Registry) Naming.lookup("//"+getCodeBase().getHost()+"/ArjunaRegistry");
	    String[] contents = reg.list();
	    Object obj = reg.lookup("HelloRegistryServer");
	    message = "hello";
	}
	catch (Exception e)
	{
	    System.out.println("RegistryApplet exception: " + e.getMessage());
	    e.printStackTrace();
	}
    }
    
public void paint (Graphics g)
    {
	g.drawString(message, 25, 50);
    }

private String message = "";
    
}
