/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * XMLUtil.java
 */

package com.arjuna.wst.impl.messaging;

public class XMLUtil
{
    // WS-U

    public static final String WSU_PREFIX        = "wsu";
    public static final String WSU_NAMESPACE_URI = "http://schemas.xmlsoap.org/ws/2002/07/utility";

    public static final String ADDRESS_TAG    = "Address";
    public static final String EXPIRES_TAG    = "Expires";
    public static final String IDENTIFIER_TAG = "Identifier";

    // WS-TX

    public static final String WSTX_PREFIX        = "wstx";
    public static final String WSTX_NAMESPACE_URI = "http://schemas.xmlsoap.org/ws/2002/08/wstx";

    public static final String PREPARE_TAG              = "Prepare";
    public static final String PREPARED_TAG             = "Prepared";
    public static final String ABORTED_TAG              = "Aborted";
    public static final String READ_ONLY_TAG            = "ReadOnly";
    public static final String COMMIT_TAG               = "Commit";
    public static final String ROLLBACK_TAG             = "Rollback";
    public static final String COMMITTED_TAG            = "Committed";
    public static final String NOTIFIED_TAG             = "Notified";
    public static final String ONE_PHASE_COMMIT_TAG     = "OnePhaseCommit";
    public static final String PHASE_ZERO_TAG           = "PhaseZero";
    public static final String PHASE_ZERO_COMPLETED_TAG = "PhaseZeroCompleted";
    public static final String UNKNOWN_TAG              = "Unknown";
    public static final String REPLAY_TAG               = "Replay";
    public static final String ERROR_TAG                = "Error";

    public static final String TARGET_PROTOCOL_SERVICE_TAG      = "TargetProtocolService";
    public static final String SOURCE_PROTOCOL_SERVICE_TAG      = "SourceProtocolService";
    public static final String COORDINATOR_PROTOCOL_SERVICE_TAG = "CoordinatorProtocolService";
    public static final String PARTICIPANT_PROTOCOL_SERVICE_TAG = "ParticipantProtocolService";
    public static final String ERROR_CODE_TAG                   = "Errorcode";

    public static final String INVALIDSTATE_ERROR_CODE = "PhaseZeroFailure";

    // WS-ARJTX

    public static final String WSARJTX_PREFIX        = "wsarjtx";
    public static final String WSARJTX_NAMESPACE_URI = "http://schemas.arjuna.com/ws/2003/01/wsarjtx";

    public static final String TRANSACTION_IDENTIFIER_TAG = "TransactionIdentifier";
    public static final String PARTICIPANT_IDENTIFIER_TAG = "ParticipantIdentifier";
    public static final String DIALOG_IDENTIFIER_TAG      = "DialogIdentifier";

    public static final String UNKNOWNTRANSACTION_ERROR_CODE = "UnknownTransaction";
    public static final String WRONGSTATE_ERROR_CODE         = "WrongState";
    public static final String UNKNOWNERROR_ERROR_CODE       = "UnknownError";
}
