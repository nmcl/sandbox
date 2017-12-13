/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * TestUtil.java
 */

package com.arjuna.wsc.tests;

public class TestUtil
{
    public static final String COORDINATION_TYPE                           = "http://wsc.example.org/coordination-type";
    public static final String UNKNOWN_COORDINATION_TYPE                   = "http://wsc.example.org/unknown-coordination-type";
    public static final String INVALID_CREATE_PARAMETERS_COORDINATION_TYPE = "http://wsc.example.org/invalid-create-parameters-coordination-type";

    public static final String PROTOCOL_IDENTIFIER                         = "http://wsc.example.org/protocol-identifier";
    public static final String UNKNOWN_PROTOCOL_IDENTIFIER                 = "http://wsc.example.org/unknown-protocol-identifier";
    public static final String ALREADY_REGISTERED_PROTOCOL_IDENTIFIER      = "http://wsc.example.org/already-registered-protocol-identifier";
    public static final String INVALID_PROTOCOL_PROTOCOL_IDENTIFIER        = "http://wsc.example.org/invalid-protocol-protocol-identifier";
    public static final String INVALID_STATE_PROTOCOL_IDENTIFIER           = "http://wsc.example.org/invalid-state-protocol-identifier";
    public static final String NO_ACTIVITY_PROTOCOL_IDENTIFIER             = "http://wsc.example.org/no-activity-protocol-identifier";

    public static final String PROTOCOL_PARTICIPANT_SERVICE                = "http://wsc.example.org/protocol-participant-service";
    public static final String PROTOCOL_COORDINATOR_SERVICE                = "http://wsc.example.org/protocol-coordinator-service";

    public static final String TEST_EXTENSION_VALUE                        = "TestExtensionValue";

    public static String ACTIVATION_REQUESTER_URL             = null;
    public static String ACTIVATION_COORDINATOR_SERVICE_URL   = null;
    public static String REGISTRATION_REQUESTER_URL           = null;
    public static String REGISTRATION_COORDINATOR_SERVICE_URL = null;
}
