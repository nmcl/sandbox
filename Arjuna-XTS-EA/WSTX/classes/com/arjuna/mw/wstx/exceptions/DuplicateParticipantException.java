/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: DuplicateParticipantException.java,v 1.1 2002/11/25 11:00:51 nmcl Exp $
 */

package com.arjuna.mw.wstx.exceptions;

import com.arjuna.mw.wsas.exceptions.WSASException;

/**
 * Thrown if an enrol attempt is made for a participant with an identifier
 * that has already been used to enrol.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: DuplicateParticipantException.java,v 1.1 2002/11/25 11:00:51 nmcl Exp $
 * @since 1.0.
 */

public class DuplicateParticipantException extends WSASException
{
    
    public DuplicateParticipantException ()
    {
        super();
    }

    public DuplicateParticipantException (String s)
    {
        super(s);
    }
    
    public DuplicateParticipantException (String s, int errorcode)
    {
	super(s, errorcode);
    }

}

