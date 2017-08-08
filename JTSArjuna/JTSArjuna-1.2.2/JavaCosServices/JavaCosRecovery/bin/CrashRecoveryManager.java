/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CrashRecoveryManager.java,v 1.2 1998/07/06 10:49:39 nmcl Exp $
 */

import com.arjuna.JavaCosRecovery.CrashMan.CRController;

/*
 * This daemon runs at startup and then periodically to resolve
 * and outstanding atomic actions.
 */

public class CrashRecoveryManager
{

public static void main (String[] args)
    {
	CRController controller = new CRController(System.out);
	controller.recover();
    }

};
