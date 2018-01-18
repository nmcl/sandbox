/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: DemoOutcomeNotification.java,v 1.6 2003/03/04 16:43:23 nmcl Exp $
 */

package com.arjuna.mwtests.wst.common;

import com.hp.mw.ts.arjuna.common.Uid;

import com.arjuna.wst.*;

//import com.arjuna.mw.wst.exceptions.*;

/**
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: DemoOutcomeNotification.java,v 1.6 2003/03/04 16:43:23 nmcl Exp $
 * @since 1.0.
 */

public class DemoOutcomeNotification implements OutcomeNotificationParticipant
{

    public DemoOutcomeNotification ()
    {
    }
    
    public void committed () throws SystemException
    {
	System.out.println("DemoOutcomeNotification.committed");
    }
    
    public void aborted () throws SystemException
    {
	System.out.println("DemoOutcomeNotification.aborted");
    }	

    public void unknown () throws SystemException
    {
    }

    public void error () throws SystemException
    {
    }

    public String identifier ()
    {
	return _id.stringForm();
    }

    private Uid _id = new Uid();
    
}

