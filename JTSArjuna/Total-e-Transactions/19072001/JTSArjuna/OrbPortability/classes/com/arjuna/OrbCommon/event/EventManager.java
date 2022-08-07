/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: EventManager.java,v 1.1.2.1.2.2 2001/01/11 14:17:04 nmcl Exp $
 */

package com.arjuna.OrbCommon.event;

import com.arjuna.OrbCommon.ORBEnvironment;
import com.arjuna.ArjunaCommon.Common.ErrorStream;
import com.arjuna.ArjunaCommon.Common.PropertyManager;
import org.omg.CORBA.Object;
import java.util.*;

/**
 * The current implementation will invoke all registered handlers
 * whenever an object is connected and disconnected. These handlers
 * can then determine whether they want to do anything about it by
 * checking the type of the object (using narrow).
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: EventManager.java,v 1.1.2.1.2.2 2001/01/11 14:17:04 nmcl Exp $
 * @since JTS 2.1.
 */

/*
 * If we were to allow a registration of a handler on a per type basis
 * then we would have to do narrow ourselves (possibly for
 * every type in a classes hierarchy). A user would also then have to
 * register one instance for each type the object may have, or we would
 * have a more complicated registration process, possibly requiring a
 * "hierarchy" string to be passed in.
 *
 * Until we know how (and if) this class is used, it's better to start
 * off simple and work our way up to extremely complicated!
 */

public class EventManager
{

    /**
     * The object has been connected to the ORB.
     */

public void connected (org.omg.CORBA.Object obj)
    {
	Enumeration e = _handlers.elements();
	
	while (e.hasMoreElements())
	{
	    EventHandler h = (EventHandler) e.nextElement();

	    try
	    {
		h.connected(obj);
	    }
	    catch (Throwable ex)
	    {
		ErrorStream.warning().println("EventHandler.connected for: "+h.name()+" threw: "+ex);
	    }
	}
    }

    /**
     * The object has been disconnected from the ORB.
     */

public void disconnected (org.omg.CORBA.Object obj)
    {
	Enumeration e = _handlers.elements();
	
	while (e.hasMoreElements())
	{
	    EventHandler h = (EventHandler) e.nextElement();

	    try
	    {
		h.disconnected(obj);
	    }
	    catch (Throwable ex)
	    {
		ErrorStream.warning().println("EventHandler.disconnected for: "+h.name()+" threw: "+ex);
	    }
	}
    }

    /**
     * Add the specified handler. If the handler has already been
     * registered then this operation will fail.
     */

public boolean addHandler (EventHandler h)
    {
	if (_handlers.get(h) == null)
	{
	    _handlers.put(h, h);

	    return true;
	}
	else
	    return false;
    }

    /**
     * Remove the specified handler. If the handler has not been
     * registered then this operation will fail.
     */

public boolean removeHandler (EventHandler h)
    {
	if (_handlers.remove(h) != null)
	    return true;
	else
	    return false;
    }

    /**
     * @return the EventManager instance.
     */

public static synchronized EventManager getManager ()
    {
	if (_theManager == null)
	    _theManager = new EventManager();
	
	return _theManager;
    }

protected EventManager ()
    {
	_handlers = new Hashtable();

	/*
	 * Now scan through the list of properties and see if there
	 * are any classes for us.
	 */

	Enumeration e = PropertyManager.propertyNames();
	
	while (e.hasMoreElements())
	{
	    String name = (String) e.nextElement();
	    
	    if (name.startsWith(ORBEnvironment.EVENT_HANDLER))
	    {
		String val = (String) PropertyManager.getProperty(name);
		
		if (val != null)
		{
		    try
		    {
			Class c = Class.forName(val);
			EventHandler h = (EventHandler) c.newInstance();

			addHandler(h);
		    }
		    catch (Exception ex)
		    {
			ErrorStream.warning().println("EventManager - caught exception: "+ex+" for "+val);
		    }
		}
		else
		    ErrorStream.warning().println("EventManager - no value for: "+name);
	    }
	}
    }
    
private Hashtable _handlers;
    
private static EventManager _theManager = null;
        
}
