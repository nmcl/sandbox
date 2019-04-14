/*
 * Copyright (C) 2001,
 *
 * Hewlett-Packard Arjuna Labs,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: Process.java,v 1.2 2003/08/11 14:07:40 nmcl Exp $
 */

package com.arjuna.ats.arjuna.utils;

import com.arjuna.ats.arjuna.common.*;
import com.arjuna.common.util.propertyservice.PropertyManager;
import java.util.Properties;
import java.io.*;
import java.net.InetAddress;

import com.arjuna.ats.arjuna.exceptions.FatalError;
import java.net.UnknownHostException;
import java.lang.NumberFormatException;
import java.lang.StringIndexOutOfBoundsException;
import java.io.IOException;
import java.io.FileNotFoundException;

/**
 * Provides a configurable way to get a unique process id.
 *
 * @version $Id: Process.java,v 1.2 2003/08/11 14:07:40 nmcl Exp $
 * @since JTS 1.0.
 */

public interface Process
{

    /**
     * @return the process id. This had better be unique between processes
     * on the same machine. If not we're in trouble!
     *
     * @since JTS 2.1.
     */
    
public int getpid ();
    
}
