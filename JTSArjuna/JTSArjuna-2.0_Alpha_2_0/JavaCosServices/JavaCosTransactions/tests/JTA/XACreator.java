/*
 * Copyright (C) 1999,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: XACreator.java,v 1.3 1999/05/14 10:18:19 nmcl Exp $
 */

import javax.transaction.xa.*;

public interface XACreator
{
    
public XAResource create (String param);

};
