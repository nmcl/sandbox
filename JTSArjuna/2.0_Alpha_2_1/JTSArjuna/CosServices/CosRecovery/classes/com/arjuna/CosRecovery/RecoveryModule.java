/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: RecoveryModule.java,v 1.1.2.1 2000/07/27 16:12:11 ndbi Exp $
 */

package com.arjuna.CosRecovery;

/**
 * Standard interface for Recovery manager plug-ins
 */

public interface RecoveryModule
{
    public void periodicWork ();
};
