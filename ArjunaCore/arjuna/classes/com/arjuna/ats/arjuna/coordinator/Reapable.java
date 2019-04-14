/*
 * Copyright (C) 2001,
 *
 * Hewlett-Packard Arjuna Labs,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: Reapable.java,v 1.1 2003/06/19 10:50:16 nmcl Exp $
 */

package com.arjuna.ats.arjuna.coordinator;

import com.arjuna.ats.arjuna.common.Uid;

/**
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: Reapable.java,v 1.1 2003/06/19 10:50:16 nmcl Exp $
 * @since JTS 3.0.
 */

public interface Reapable
{

    public boolean running ();

    public boolean preventCommit ();

    public int cancel ();

    public Uid get_uid ();
    
}
