/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: Schema.java,v 1.3 2003/02/26 14:48:50 nmcl Exp $
 */

package com.arjuna.mwlabs.wsc.common;

/**
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: Schema.java,v 1.3 2003/02/26 14:48:50 nmcl Exp $
 * @since 1.0.
 */

public class Schema
{

    public static final String REGISTRATION_SERVICE = "wscoor:RegistrationService";
    public static final String ADDRESS = "wsu:Address";
    public static final String IDENTIFIER = "wsu:Identifier";
    public static final String EXPIRES = "wsu:Expires";
    public static final String COORDINATION_TYPE = "wscoor:CoordinationType";
    public static final String COORDINATION_CONTEXT = "wscoor:CoordinationContext";
    
    public static final String WSCOOR_NAMESPACE = "http://schemas.xmlsoap.org/ws/2002/08/wscoor";
    public static final String WSU_NAMESPACE = "http://schemas.xmlsoap.org/ws/2002/07/utility";
    public static final String ARJUNA_NAMESPACE = "http://arjuna.com/schemas/wsc/2003/01/extension";

}
