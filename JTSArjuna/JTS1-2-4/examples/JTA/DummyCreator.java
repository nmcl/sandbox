/*
 * Copyright (C) 1999,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DummyCreator.java,v 1.1.2.1 1999/03/03 11:18:42 nmcl Exp $
 */

import javax.transaction.xa.*;

public class DummyCreator implements XACreator
{

public XAResource create (String param)
    {
	return new DummyXA();
    }

};
