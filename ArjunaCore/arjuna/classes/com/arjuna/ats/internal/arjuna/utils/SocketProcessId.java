/*
 * Copyright (C) 2001,
 *
 * Hewlett-Packard Arjuna Labs,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: SocketProcessId.java,v 1.2 2003/08/11 14:07:44 nmcl Exp $
 */

package com.arjuna.ats.internal.arjuna.utils;

import com.arjuna.ats.arjuna.logging.tsLogger;
import com.arjuna.common.util.propertyservice.PropertyManager;
import com.arjuna.ats.arjuna.common.arjPropertyManager;

import com.arjuna.ats.arjuna.utils.Process;
import com.arjuna.ats.arjuna.utils.Utility;
import java.io.*;
import java.net.*;

import com.arjuna.ats.arjuna.exceptions.FatalError;
import java.net.UnknownHostException;
import java.lang.NumberFormatException;
import java.lang.StringIndexOutOfBoundsException;
import java.io.IOException;
import java.io.FileNotFoundException;

/**
 * Obtains a unique value to represent the process id via sockets and
 * ports.
 *
 * This implementation is tied closely with the socket/port version of
 * crash recovery, since that requires a thread to listen on a port for
 * incoming requests for a specific transaction's status.
 *
 * @author Mark Little (mark_little@hp.com)
 * @version $Id: SocketProcessId.java,v 1.2 2003/08/11 14:07:44 nmcl Exp $
 * @since HPTS 3.0.
 */

public class SocketProcessId implements com.arjuna.ats.arjuna.utils.Process
{

    /**
     * @return the process id. This had better be unique between processes
     * on the same machine. If not we're in trouble!
     *
     * @message com.arjuna.ats.internal.arjuna.utils.SocketProcessId_1 [com.arjuna.ats.internal.arjuna.utils.SocketProcessId_1]- Invalid port specified 
     */
    
public int getpid ()
    {
	synchronized (SocketProcessId._lock)
	    {
		if (_thePort == 0)
		{
		    if (_theSocket == null)
		    {
			int port = _defaultPort;
      
			String portStr = arjPropertyManager.propertyManager.getProperty("com.arjuna.ats.arjuna.recovery.transactionStatusManagerPort");

			if ( portStr != null )
			{
			    try
			    {
				port = Integer.parseInt(portStr);
			    }
			    catch (Exception ex)
			    {
				if (tsLogger.arjLoggerI18N.isWarnEnabled())
				    tsLogger.arjLoggerI18N.warn("com.arjuna.ats.internal.arjuna.utils.SocketProcessId_1",ex);

				port = -1;
			    }
			}
			
			if (port != -1)
			{
			    try
			    {
				_theSocket = new ServerSocket(port);

				_thePort = _theSocket.getLocalPort();
			    }
			    catch (Exception ex)
			    {
				_thePort = -1;
			    }
			}
			else
			    _thePort = -1;
		    }
		}
	    }

	return _thePort;
    }

public static final ServerSocket getSocket ()
    {
	synchronized (SocketProcessId._lock)
	    {
		return _theSocket;
	    }
    }
    
private static int          _thePort = 0;
private static ServerSocket _theSocket = null;
private static Object       _lock = new Object();

    /**
     * Default port is any free port.
     */

private static final int _defaultPort = 0 ;

 
}
