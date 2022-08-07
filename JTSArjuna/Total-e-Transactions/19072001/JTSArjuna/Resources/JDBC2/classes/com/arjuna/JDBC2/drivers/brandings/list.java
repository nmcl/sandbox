/*
 * Copyright (C) 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: list.javatmpl,v 1.1.4.1 2001/07/19 14:16:09 nmcl Exp $
 */

package com.arjuna.JDBC2.drivers.brandings;

import com.arjuna.JDBC2.ArjunaJDBC2BrandingFactory;

public class list
{

public list ()
    {



	ArjunaJDBC2BrandingFactory.putBranding(brandingName, new com.arjuna.JDBC2.drivers.brandings.merant());

    }

private static final String brandingName = "sequelink";

}
