/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * XMLUtil.java
 */

package com.arjuna.wsc.impl.messaging;

public class XMLUtil
{
    // WS-U

    public static final String WSU_PREFIX        = "wsu";
    public static final String WSU_NAMESPACE_URI = "http://schemas.xmlsoap.org/ws/2002/07/utility";

    public static final String ADDRESS_TAG    = "Address";
    public static final String EXPIRES_TAG    = "Expires";
    public static final String IDENTIFIER_TAG = "Identifier";

    public static final String VALUETYPE_ATTR = "ValueType";

    // WS-C

    public static final String WSCOOR_PREFIX        = "wscoor";
    public static final String WSCOOR_NAMESPACE_URI = "http://schemas.xmlsoap.org/ws/2002/08/wscoor";

    public static final String CREATE_COORDINATION_CONTEXT_TAG          = "CreateCoordinationContext";
    public static final String CREATE_COORDINATION_CONTEXT_RESPONSE_TAG = "CreateCoordinationContextResponse";
    public static final String REGISTER_TAG                             = "Register";
    public static final String REGISTER_RESPONSE_TAG                    = "RegisterResponse";
    public static final String ERROR_TAG                                = "Error";

    public static final String COORDINATION_CONTEXT_TYPE_TAG    = "CoordinationContextType";
    public static final String COORDINATION_TYPE_TAG            = "CoordinationType";
    public static final String REGISTRATION_SERVICE_TAG         = "RegistrationService";
    public static final String COORDINATOR_PROTOCOL_SERVICE_TAG = "CoordinatorProtocolService";
    public static final String PARTICIPANT_PROTOCOL_SERVICE_TAG = "ParticipantProtocolService";
    public static final String PROTOCOL_IDENTIFIER_TAG          = "ProtocolIdentifier";
    public static final String ACTIVATION_SERVICE_TAG           = "ActivationService";
    public static final String REQUESTER_REFERENCE_TAG          = "RequesterReference";
    public static final String TARGET_PROTOCOL_SERVICE_TAG      = "TargetProtocolService";
    public static final String COORDINATION_CONTEXT_TAG         = "CoordinationContext";
    public static final String CURRENT_CONTEXT_TAG              = "CurrentContext";
    public static final String ERROR_CODE_TAG                   = "Errorcode";

    public static final String ALREADYREGISTERED_ERROR_CODE       = "AlreadyRegistered";
    public static final String INVALIDSTATE_ERROR_CODE            = "InvalidState";
    public static final String INVALIDPROTOCOL_ERROR_CODE         = "InvalidProtocol";
    public static final String NOACTIVITY_ERROR_CODE              = "NoActivity";
    public static final String INVALIDCREATEPARAMETERS_ERROR_CODE = "InvalidCreateParameters";
    public static final String ADDRESSTOOLONG_ERROR_CODE          = "AddressTooLong";
    public static final String INVALIDADDRESS_ERROR_CODE          = "InvalidAddress";

    // WS-ARJCOOR

    public static final String WSARJCOOR_PREFIX        = "wsarjcoor";
    public static final String WSARJCOOR_NAMESPACE_URI = "http://schemas.arjuna.com/ws/2003/01/wsarjcoor";

    public static final String DIALOG_IDENTIFIER_TAG   = "DialogIdentifier";
    public static final String INSTANCE_IDENTIFIER_TAG = "InstanceIdentifier";

    public static final String UNKNOWNERROR_ERROR_CODE = "UnknownError";
}
