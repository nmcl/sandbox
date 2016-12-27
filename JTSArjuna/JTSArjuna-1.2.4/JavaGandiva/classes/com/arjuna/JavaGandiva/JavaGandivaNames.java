/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: JavaGandivaNames.java,v 1.4 1998/12/11 14:21:53 nmcl Exp $
 */

package com.arjuna.JavaGandiva;

import com.arjuna.JavaGandiva.Common.ClassName;

public class JavaGandivaNames
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
	return new ClassName("StaticInventory");
    }

public static ClassName Implementation_Inventory_DynamicInventory ()
    {
	return new ClassName("DynamicInventory");
    }
    
};
