/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ArjunaCommonNames.java,v 1.1.2.1 2000/04/25 08:35:53 nmcl Exp $
 */

package com.arjuna.ArjunaCommon;

import com.arjuna.ArjunaCommon.Common.ClassName;

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
