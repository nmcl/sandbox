/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ListElement.java,v 1.1 2003/06/19 10:50:39 nmcl Exp $
 */

package com.arjuna.ats.internal.arjuna.template;

import com.arjuna.ats.arjuna.common.Uid;

/**
 * Items which are required to be put in some lists
 * must implement this interface.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ListElement.java,v 1.1 2003/06/19 10:50:39 nmcl Exp $
 * @since JTS 1.0.
 */

public interface ListElement
{

    public Uid get_uid ();

}

