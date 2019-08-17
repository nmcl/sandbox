/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ORBFilter.java,v 1.1.28.1.4.1 2000/12/21 11:21:28 nmcl Exp $
 */

package com.arjuna.OrbCommon;

/**
 * Some ORBs support the notion of interceptors (or filters) which
 * we can use to do threading, implicit context propagation, and
 * interposition. However, although we would like to be able to chain
 * different filters together so that each is only concerned with a
 * single functionality, this either tends not to be possible (e.g., RCP)
 * or does not work (e.g., Orbix). So, we have to insist on a single filter
 * to do everything.
 *
 * Since we do not want OrbCommon to be CosServices (i.e., OTS) specific, we
 * need some way of dynamically adding new filters to override old ones. This
 * class does this. Only one filter can be added.
 *
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ORBFilter.java,v 1.1.28.1.4.1 2000/12/21 11:21:28 nmcl Exp $
 * @since JTS 1.0.
 */

public interface ORBFilter
{
};
