/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: XAStatus.java,v 1.1 2003/01/07 10:33:47 nmcl Exp $
 */

package com.arjuna.mw.wscf.model.xa.status;

import com.arjuna.mw.wsas.status.Status;

/**
 * The status of the coordination is such that it will eventually cancel.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: XAStatus.java,v 1.1 2003/01/07 10:33:47 nmcl Exp $
 * @since 1.0.
 */

public class XAStatus implements Status
{

    public XAStatus (int status)
    {
	_status = status;
    }
    
    public static XAStatus instance ()
    {
	return _instance;
    }
    
    public String toString ()
    {
	return "org.w3c.wscf.xa.status.XAStatus";
    }

    public int getStatus ()
    {
	return _status;
    }
    
    private XAStatus ()
    {
    }

    private int _status;
    
    private static final XAStatus _instance = new XAStatus();
    
}
