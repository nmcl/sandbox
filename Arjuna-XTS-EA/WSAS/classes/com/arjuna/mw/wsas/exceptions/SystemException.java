/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: SystemException.java,v 1.1 2002/11/25 10:51:43 nmcl Exp $
 */

package com.arjuna.mw.wsas.exceptions;

/**
 * Thrown if an error occurs which is not met by another specific
 * exception.
 * 
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: SystemException.java,v 1.1 2002/11/25 10:51:43 nmcl Exp $
 * @since 1.0.
 */

public class SystemException extends WSASException
{

    public SystemException ()
    {
	super();
    }

    public SystemException (String s)
    {
	super(s);
    }

    public SystemException (String s, int errorcode)
    {
	super(s, errorcode);
    }

    public SystemException (String reason, Object obj)
    {
	super(reason, obj);
    }
    
    public SystemException (Object ex)
    {
	super(ex);
    }
    
}
