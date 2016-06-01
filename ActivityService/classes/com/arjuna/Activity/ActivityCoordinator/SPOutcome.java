/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: SPOutcome.java,v 1.1 2001/07/23 13:44:54 nmcl Exp $
 */

package com.arjuna.Activity.ActivityCoordinator;

import java.io.*;
import java.util.*;

class SPOutcome
{

public static final int SP_OK = 0;
public static final int SP_NOTOK = 1;
public static final int SP_UNKNOWN = 2;
public static final int SP_NEXTSIGNAL = 3;

public static final String outcome (int o)
    {
	switch (o)
	{
	case SPOutcome.SP_OK:
	    return "SPOutcome.SP_OK";
	case SPOutcome.SP_NOTOK:
	    return "SPOutcome.SP_NOTOK";
	case SPOutcome.SP_NEXTSIGNAL:
	    return "SPOutcome.SP_NEXTSIGNAL";
	default:
	    return "SPOutcome.SP_UNKNOWN";
	}
    }
    
};
