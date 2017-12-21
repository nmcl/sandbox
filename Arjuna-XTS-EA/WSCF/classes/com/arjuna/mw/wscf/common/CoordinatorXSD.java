/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: CoordinatorXSD.java,v 1.2 2002/12/20 14:29:03 nmcl Exp $
 */

package com.arjuna.mw.wscf.common;

/**
 * The coordination protocol schema is used to allow a coordination
 * implementation to uniquely and unambiguously specify the type of
 * protocol that is provided. Obviously one coordinator implementation may
 * support multiple protocols.
 *
 * When a coordination implementation is registered with the WSCF the
 * respective XML protocol definition is also provided. Likewise, when a
 * user/application requests to use a specific coordination protocol it must
 * provide the XML protocol definition. This class represents the element names
 * that are used to parse the XML.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: CoordinatorXSD.java,v 1.2 2002/12/20 14:29:03 nmcl Exp $
 * @since 1.0.
 */

public class CoordinatorXSD
{

    public static final String coordinatorType = "coordinator-type";
    public static final String coordinatorName = "coordinator-name";  // optional
    public static final String notification = "notification";
    public static final String notificationAttributeName = "name";
    public static final String response = "response";
    public static final String responseElementName = "name";
    public static final String genericCoordinator = "generic";
    
}
