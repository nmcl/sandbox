/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: DemoParticipant.java,v 1.8 2003/03/24 10:57:02 nmcl Exp $
 */

package com.arjuna.mwtests.wst.common;

import com.hp.mw.ts.arjuna.common.Uid;

import com.arjuna.wst.*;

//import com.arjuna.mw.wst.vote.*;

//import com.arjuna.mw.wst.exceptions.*;

/**
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: DemoParticipant.java,v 1.8 2003/03/24 10:57:02 nmcl Exp $
 * @since 1.0.
 */

public class DemoParticipant implements TwoPCParticipant
{

    public DemoParticipant ()
    {
	_passed = false;
    }

    public final boolean passed ()
    {
	return _passed;
    }
    
    public Vote prepare () throws WrongStateException, SystemException
    {
	System.out.println("DemoParticipant.prepare for "+this);
	
	return new Prepared();
    }

    public void commit () throws WrongStateException, SystemException
    {
	System.out.println("DemoParticipant.commit for "+this);

	_passed = true;
    }

    public void rollback () throws WrongStateException, SystemException
    {
	System.out.println("DemoParticipant.rollback for "+this);

	_passed = false;
    }

    public void commitOnePhase () throws WrongStateException, SystemException
    {
	System.out.println("DemoParticipant.commitOnePhase for "+this);

	_passed = true;
    }

    public void unknown () throws SystemException
    {
    }

    public void error () throws SystemException
    {
    }

    public String toString ()
    {
	return identifier();
    }
    
    public String identifier ()
    {
	return _id.stringForm();
    }
    
    private boolean _passed;
    private Uid     _id = new Uid();
    
}

