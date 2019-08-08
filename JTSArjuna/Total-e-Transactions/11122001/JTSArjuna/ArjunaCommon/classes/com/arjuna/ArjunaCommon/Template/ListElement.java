/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ListElement.java,v 1.1.28.1.4.1 2000/12/21 11:20:55 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Template;

import com.arjuna.ArjunaCommon.Common.Uid;

/**
 * Items which are required to be put in some lists
 * must implement this interface.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ListElement.java,v 1.1.28.1.4.1 2000/12/21 11:20:55 nmcl Exp $
 * @since JTS 1.0.
 */

public interface ListElement
{

public Uid get_uid ();

}

