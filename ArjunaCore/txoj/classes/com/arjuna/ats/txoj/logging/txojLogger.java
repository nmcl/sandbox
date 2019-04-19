package com.arjuna.ats.txoj.logging;

import com.arjuna.common.util.logging.*;
import com.arjuna.common.util.logging.DebugLevel;

import com.arjuna.common.internal.util.logging.commonPropertyManager;

import java.util.*;

public class txojLogger
{

    public static LogNoi18n      aitLogger;
    public static Logi18n        aitLoggerI18N;
    public static ResourceBundle log_mesg;

    private static String language;
    private static String country;
    private static Locale currentLocale;
    private static String dirLocale;

    static 
    {
	aitLogger = LogFactory.getLogNoi18n("com.arjuna.ats.txoj.logging.txojLogger");

	language = commonPropertyManager.propertyManager.getProperty("language","en");

	country  = commonPropertyManager.propertyManager.getProperty("country","US");

	currentLocale = new Locale(language,country);
	log_mesg = ResourceBundle.getBundle("txoj_msg",currentLocale);

	aitLoggerI18N = LogFactory.getLogi18n("com.arjuna.ats.txoj.logging.txojLoggerI18N",
					      "txoj_msg_"+language+"_"+country);
    }

}
