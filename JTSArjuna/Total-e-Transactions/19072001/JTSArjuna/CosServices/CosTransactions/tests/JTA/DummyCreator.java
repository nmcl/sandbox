/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: DummyCreator.java,v 1.1 2000/02/25 14:07:57 nmcl Exp $
 */

import javax.transaction.xa.*;

public class DummyCreator implements XACreator
{

public XAResource create (String param)
    {
	return new DummyXA();
    }

};
