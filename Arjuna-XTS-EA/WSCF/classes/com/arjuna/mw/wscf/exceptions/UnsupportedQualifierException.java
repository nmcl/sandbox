/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: UnsupportedQualifierException.java,v 1.1 2002/11/25 10:56:34 nmcl Exp $
 */

package com.arjuna.mw.wscf.exceptions;

import com.arjuna.mw.wsas.exceptions.WSASException;

/**
 * Exception reported if a given qualifier is not supported. This includes
 * those cases where the <i>format</i> of a qualifier is not in canonical form.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: UnsupportedQualifierException.java,v 1.1 2002/11/25 10:56:34 nmcl Exp $
 * @since 1.0.
 */

public class UnsupportedQualifierException extends WSASException
{

    public UnsupportedQualifierException ()
    {
        super();
    }

    public UnsupportedQualifierException (String msg)
    {
        super(msg);
    }

    public UnsupportedQualifierException (String s, int errorcode)
    {
	super(s, errorcode);
    }
    
}

