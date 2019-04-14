/*
 * Copyright (C) 1999-2001 by HP Bluestone Software, Inc. All rights Reserved.
 *
 * HP Arjuna Labs,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: Service.java,v 1.1 2003/06/19 10:50:24 nmcl Exp $
 */
 
package com.arjuna.ats.arjuna.recovery ;

import java.io.* ;

public interface Service
{
   public void doWork ( InputStream in, OutputStream out )
      throws IOException ;
}

