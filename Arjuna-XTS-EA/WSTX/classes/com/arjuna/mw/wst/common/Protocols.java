/*
 * Copyright (C) 2003,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: Protocols.java,v 1.2 2003/02/13 11:45:54 nmcl Exp $
 */

package com.arjuna.mw.wst.common;

/**
 */

public interface Protocols
{

    public static final String AtomicTransaction = "http://schemas.xmlsoap.org/ws/2002/08/wstx";
    public static final String BusinessActivity = "http://schemas.xmlsoap.org/ws/2002/08/wsba";

    public static final String Completion = "http://schemas.xmlsoap.org/ws/2002/08/wstx/Completion";
    public static final String CompletionWithAck = "http://schemas.xmlsoap.org/ws/2002/08/wstx/CompletionWithAck";
    public static final String PhaseZero = "http://schemas.xmlsoap.org/ws/2002/08/wstx/PhaseZero";
    public static final String TwoPhaseCommit = "http://schemas.xmlsoap.org/ws/2002/08/wstx/2PC";
    public static final String OutcomeNotification = "http://schemas.xmlsoap.org/ws/2002/08/wstx/OutcomeNotification";
    
}
