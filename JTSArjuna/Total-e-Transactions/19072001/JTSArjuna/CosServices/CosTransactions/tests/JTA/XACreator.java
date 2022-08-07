/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: XACreator.java,v 1.1.28.1 2000/11/14 12:07:33 nmcl Exp $
 */

import javax.transaction.xa.*;

public interface XACreator
{
    
public XAResource create (String param);

};
