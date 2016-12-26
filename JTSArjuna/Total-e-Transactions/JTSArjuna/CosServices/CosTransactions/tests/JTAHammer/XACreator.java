/*
 * Copyright (C) 1999,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: XACreator.java,v 1.1.2.1.114.1 2002/07/08 11:42:00 nmcl Exp $
 */

import javax.transaction.xa.*;

/**
 */

public interface XACreator
{
    
public XAResource create (String param);

};
