/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK. 
 *
 * $Id: Resource.java,v 1.1 2003/06/19 10:50:19 nmcl Exp $
 */

package com.arjuna.ats.arjuna.gandiva;

import com.arjuna.ats.arjuna.gandiva.ClassName;

/**
 * Severely cut down version of original Gandiva Resource
 * since Java provides reference counting for us, and
 * everything is created on the heap!
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: Resource.java,v 1.1 2003/06/19 10:50:19 nmcl Exp $
 * @since JTS 1.0.
 */

public interface Resource
{

public abstract Object castup (ClassName theType);

}
