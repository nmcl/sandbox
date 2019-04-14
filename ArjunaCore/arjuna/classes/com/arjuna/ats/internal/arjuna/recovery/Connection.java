/*
 * Copyright (C) 1999-2001 by HP Bluestone Software, Inc. All rights Reserved.
 *
 * HP Arjuna Labs,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: Connection.java,v 1.1 2003/06/19 10:50:36 nmcl Exp $
 */
 
package com.arjuna.ats.internal.arjuna.recovery ;

import java.io.* ;
import java.net.* ;
import java.util.* ;

import com.arjuna.ats.arjuna.logging.tsLogger;

import com.arjuna.ats.arjuna.recovery.Service ;

public class Connection extends Thread
{
   /**
    * Takes socket and service to execute.
    */
   public Connection( Socket server_socket, Service service )
   {
      super( "Server.Connection:" + 
             server_socket.getInetAddress().getHostAddress() +
             ":" + server_socket.getPort() ) ;
              
      _server_socket = server_socket ;
      _service = service ;
   }
   
   /**
    * Obtains input and output streams and executes work
    * required by the service.
    *
    * @message com.arjuna.ats.internal.arjuna.recovery.Connection_1 [com.arjuna.ats.internal.arjuna.recovery.Connection_1] - Connection - IOException
    */
   public void run()
   {
      try
      {
         InputStream  is = _server_socket.getInputStream() ;
         OutputStream os = _server_socket.getOutputStream() ;

         _service.doWork ( is, os ) ;
      }
      catch ( IOException ex )
      {
	  if (tsLogger.arjLoggerI18N.isWarnEnabled())
	      tsLogger.arjLoggerI18N.warn("com.arjuna.ats.internal.arjuna.recovery.Connection_1");
      }
   }

   // What client (RecoveryManager) talks to.
   private Socket  _server_socket ;
   
   // What Service is provided to the client(RecoveryManager).
   private Service _service ;        
}

