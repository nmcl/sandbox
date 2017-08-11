/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Resource.java,v 1.2 1998/07/06 13:30:53 nmcl Exp $
 */

package com.arjuna.JavaArjuna.Core;

import com.arjuna.JavaArjuna.Common.ClassName;

/*
 * Severely cut down version of original Gandiva Resource
 * since Java provides reference counting for us, and
 * everything is created on the heap!
 */

public interface Resource
{

public abstract Object castup (ClassName theType);

};
