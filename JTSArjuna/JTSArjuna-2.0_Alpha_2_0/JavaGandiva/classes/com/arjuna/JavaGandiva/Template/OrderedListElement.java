/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OrderedListElement.java,v 1.2 1998/07/06 13:31:07 nmcl Exp $
 */

package com.arjuna.JavaGandiva.Template;

public interface OrderedListElement
{
    
public boolean equals (OrderedListElement e);
public boolean lessThan (OrderedListElement e);
public boolean greaterThan (OrderedListElement e);

};
