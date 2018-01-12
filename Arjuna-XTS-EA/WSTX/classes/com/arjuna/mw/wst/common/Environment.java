/*
 * Copyright (C) 2003,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: Environment.java,v 1.2 2003/04/11 11:20:59 nmcl Exp $
 */

package com.arjuna.mw.wst.common;

/**
 */

public interface Environment
{

    public static final String COORDINATOR_URL = "com.arjuna.mw.wst.at.coordinatorURL";

    public static final String ACTIVATION_REQUESTER = "com.arjuna.mw.wst.at.activationrequester";
    public static final String ACTIVATION_COORDINATOR = "com.arjuna.mw.wst.at.activationcoordinator";
    public static final String COMPLETION_PARTICIPANT = "com.arjuna.mw.wst.at.completionparticipant";
    public static final String COMPLETION_WITH_ACK_PARTICIPANT = "com.arjuna.mw.wst.at.completionwithackparticipant";

    public static final String REGISTRATION_REQUESTER = "com.arjuna.mw.wst.at.registrationrequester";
    public static final String TWOPC_DISPATCHER = "com.arjuna.mw.wst.at.twopcdispatcher";
    public static final String TWOPC_PARTICIPANT = "com.arjuna.mw.wst.at.twopcparticipant";
    public static final String PHASEZERO_DISPATCHER = "com.arjuna.mw.wst.at.phasezerodispatcher";
    public static final String PHASEZERO_PARTICIPANT = "com.arjuna.mw.wst.at.phasezeroparticipant";
    public static final String OUTCOMENOTIFICATION_DISPATCHER = "com.arjuna.mw.wst.at.outcomenotificationdispatcher";
    public static final String OUTCOMENOTIFICATION_PARTICIPANT = "com.arjuna.mw.wst.at.outcomenotificationparticipant";
    
}
