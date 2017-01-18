/*
 * Copyright (C) 1999,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DummyCreator.java,v 1.3 1999/05/14 10:18:18 nmcl Exp $
 */

import javax.transaction.xa.*;

public class DummyCreator implements XACreator
{

public XAResource create (String param)
    {
	return new DummyXA();
    }

};
