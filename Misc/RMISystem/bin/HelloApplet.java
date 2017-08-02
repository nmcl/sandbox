/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: HelloApplet.java,v 1.1 1997/05/30 18:26:42 nmcl Exp $
 */

import java.awt.*;
import java.rmi.*;

public class HelloApplet extends java.applet.Applet
{
    
public void init ()
    {
	try
	{
	    Hello obj = (Hello) Naming.lookup("//" + getCodeBase().getHost() + "/HelloServer");
	    message = obj.sayHello();
	}
	catch (Exception e)
	{
	    System.out.println("HelloApplet exception: " + e.getMessage());
	    e.printStackTrace();
	}
    }
    
public void paint (Graphics g)
    {
	g.drawString(message, 25, 50);
    }

private String message = "";
    
}
