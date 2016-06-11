/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: HelloArjunaApplet.java,v 1.1 1997/07/31 09:13:04 nmcl Exp $
 */

import java.awt.*;
import java.rmi.*;

import java.io.IOException;

public class HelloArjunaApplet extends java.applet.Applet
{
    
public void init ()
    {
	System.runFinalizersOnExit(true);
	
	try
	{
	    obj = new HelloClient("JavaStubGenerator.rmi.RMIClientObjectBinder", "tarry-f.ncl.ac.uk:testobject");
	    message = obj.sayHello();
	}
	catch (Exception e)
	{
	    System.out.println("HelloArjunaApplet exception: " + e.getMessage());
	    e.printStackTrace();
	}
    }
    
public void paint (Graphics g)
    {
	g.drawString(message, 25, 50);
    }

private String message = "";
private Hello obj = null;
    
}

