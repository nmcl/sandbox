/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ArjunaCommonNames.java,v 1.1.2.1.32.1.34.1 2001/05/25 13:19:33 nmcl Exp $
 */

package com.arjuna.ArjunaCommon;

import com.arjuna.ArjunaCommon.Common.ClassName;

/**
 * This class contains the ClassNames for the various implementations
 * provided by this module.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ArjunaCommonNames.java,v 1.1.2.1.32.1.34.1 2001/05/25 13:19:33 nmcl Exp $
 * @since JTS 1.0.
 */

public class ArjunaCommonNames
{

public static ClassName Implementation_NameService_JNS ()
    {
	return new ClassName("JNSNameServiceImple");
    }
    
public static ClassName Implementation_NameService_PNS ()
    {
	return new ClassName("PNSNameServiceImple");
    }

public static ClassName Implementation_Inventory_StaticInventory ()
    {
	return new ClassName("StaticInventoryImple");
    }

public static ClassName Implementation_Inventory_DynamicInventory ()
    {
	return new ClassName("DynamicInventoryImple");
    }

public static ClassName Interface_Inventory ()
    {
	return new ClassName("Inventory");
    }

public static ClassName Interface_NameService ()
    {
	return new ClassName("NameService");
    }
    
};
