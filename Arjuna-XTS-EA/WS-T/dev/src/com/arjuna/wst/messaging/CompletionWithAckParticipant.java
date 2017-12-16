/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * CompletionWithAckParticipant.java
 */

package com.arjuna.wst.messaging;

import com.arjuna.wst.messaging.MarshalException;

public interface CompletionWithAckParticipant
{
    public String getServiceURI();

    public void sendCommit(CommitMessage commitMessage)
        throws MarshalException;

    public void sendRollback(RollbackMessage rollbackMessage)
        throws MarshalException;

    public void sendNotified(NotifiedMessage notifiedMessage)
        throws MarshalException;

    public void sendUnknown(UnknownMessage unknownMessage)
        throws MarshalException;

    public void sendError(ErrorMessage errorMessage)
        throws MarshalException;

    public Object recv()
        throws MarshalException;

    public Object recv(String dialogIdentifier)
        throws MarshalException;
}
