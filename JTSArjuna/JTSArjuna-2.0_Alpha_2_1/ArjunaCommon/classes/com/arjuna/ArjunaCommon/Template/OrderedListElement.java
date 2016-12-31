/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OrderedListElement.java,v 1.1 2000/02/25 14:02:59 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Template;

public interface OrderedListElement
{
    
public boolean equals (OrderedListElement e);
public boolean lessThan (OrderedListElement e);
public boolean greaterThan (OrderedListElement e);

};
