/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: Message.java,v 1.1 2003/01/07 10:33:41 nmcl Exp $
 */

package com.arjuna.mw.wscf.model.as.coordinator;

import com.arjuna.mw.wscf.common.Qualifier;

import com.arjuna.mw.wsas.activity.Outcome;

import com.arjuna.mw.wsas.exceptions.SystemException;

/**
 * Whenever a coordinator must contact a participant, it sends a protocol
 * specific notification message.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: Message.java,v 1.1 2003/01/07 10:33:41 nmcl Exp $
 * @since 1.0.
 */

public interface Message
{

    /**
     * @exception SystemException Thrown if any error occurs.
     *
     * @return The unique identification of this notification. The participant
     * may use this as the only information necessary to process the request
     * from the coordinator.
     */

    public String messageName () throws SystemException;

    /**
     * Many notifications may be processed simply on the name of the message.
     * In those situations where this is not the case, additional protocol
     * specific data may be send and obtained via this method.
     *
     * @exception SystemException Thrown if any error occurs.
     *
     * @return The coordination specific data, or null if there is none.
     */

    public Object coordinationSpecificData () throws SystemException;

    /**
     * @exception SystemException Thrown if any error occurs.
     *
     * @return Any qualifiers associated with the notification, or null.
     * For example, "only vote to commit if you can remain in this state for
     * 24 hours".
     */

    public Qualifier[] qualifiers () throws SystemException;
    
}

