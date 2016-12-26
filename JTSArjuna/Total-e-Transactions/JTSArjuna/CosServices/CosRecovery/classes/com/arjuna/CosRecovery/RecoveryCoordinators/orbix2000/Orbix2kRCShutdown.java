/*          
 * Copyright (C) 1999-2001 by HP Bluestone Software, Inc. All rights Reserved.
 * 
 * HP Arjuna Labs,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: Orbix2kRCShutdown.java,v 1.1.2.1 2001/09/14 19:34:34 jcoleman Exp $
 */

package com.arjuna.CosRecovery.RecoveryCoordinators.orbix2000;

import com.arjuna.OrbCommon.*;

class Orbix2kRCShutdown extends OAPreShutdown
{

public Orbix2kRCShutdown ()
    {
        super("Orbix2kRCShutdown");
    }

public void work ()
    {
        Orbix2kRCServiceInit.shutdownRCService();
    }
 
}
