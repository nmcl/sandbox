/*
 * Copyright (C) 2001, 2002,
 *
 * Hewlett-Packard Arjuna Labs,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: tsLogger.java,v 1.3 2003/08/11 15:04:15 nmcl Exp $
 */

package com.arjuna.ats.arjuna.logging;

import com.arjuna.common.util.logging.*;

import com.arjuna.common.internal.util.logging.commonPropertyManager;

import java.util.*;

public class tsLogger
{

    public static LogNoi18n      arjLogger;
    public static Logi18n        arjLoggerI18N;
    public static ResourceBundle log_mesg;

    private static String language;
    private static String country;
    private static Locale currentLocale;
    private static String dirLocale;

    static
    {
	arjLogger = LogFactory.getLogNoi18n("com.arjuna.ats.arjuna.logging.arjLogger");

	language = commonPropertyManager.propertyManager.getProperty("language","en");
	country  = commonPropertyManager.propertyManager.getProperty("country","US");

	currentLocale = new Locale(language,country);
	log_mesg = ResourceBundle.getBundle("arjuna_msg",currentLocale);
	
	arjLoggerI18N = LogFactory.getLogi18n("com.arjuna.ats.arjuna.logging.arjLoggerI18N",
					     "arjuna_msg_"+language+"_"+country);
	
    }

}
