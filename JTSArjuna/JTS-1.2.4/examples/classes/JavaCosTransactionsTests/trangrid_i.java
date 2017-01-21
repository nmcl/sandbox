/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: trangrid_i.javatmpl,v 1.3 1998/08/13 14:34:34 nmcl Exp $
 */

/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OrbPortability.h,v 1.4 1998/06/30 16:34:10 nmcl Exp $
 */




/*
 * Try to get around the differences between Ansi CPP and
 * K&R cpp with concatenation.
 */


/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OrbPortability_stdc.h,v 1.2 1998/11/12 08:52:40 nmcl Exp $
 */













































package JavaCosTransactionsTests;

import com.arjuna.JavaCosTransactions.OTS;
import com.arjuna.JavaCosTransactions.OTS_Current;
import TestModule.*;
import org.omg.CosTransactions.*;

import org.omg.CosTransactions.HeuristicCommit;
import org.omg.CosTransactions.HeuristicMixed;
import org.omg.CosTransactions.HeuristicHazard;
import org.omg.CosTransactions.NotPrepared;
import org.omg.CosTransactions.HeuristicRollback;
import org.omg.CORBA  .SystemException;
import org.omg.CORBA  .UserException;
import org.omg.CORBA  .UNKNOWN;

public class trangrid_i extends  TestModule ._TranGridImplBase 
{

public trangrid_i (short h, short w)
    {
	m_height = h;   // set up height
	m_width = w;    // set up width
	// now allocate the 2-D array: as an array of pointers to 1-D arrays.
	m_a = new short [h][w];
    }

public trangrid_i (short h, short w, String objectName)
    {

	m_height = h;   // set up height
	m_width = w;    // set up width
	// now allocate the 2-D array: as an array of pointers to 1-D arrays.
	m_a = new short [h][w];
    }

public short height () throws SystemException
    {
	OTS_Current current = OTS.current();
	org.omg.CosTransactions.Control control = current.get_control();
	org.omg.CosTransactions.Control ptr = control;

	if (ptr != null)
	{
	    System.out.println("trangrid_i.height - found implicit transactional context.");

	    ptr = null;
	}
	else
	{
	    System.err.println("Error: trangrid_i.height - no implicit transactional context.");

	    System.exit(0);
	}
    
	return m_height;
    }

public short width () throws SystemException
    {
	OTS_Current current = OTS.current();
	org.omg.CosTransactions.Control control = current.get_control();
	org.omg.CosTransactions.Control ptr = control;

	if (ptr != null)
	{
	    System.out.println("trangrid_i.width - found implicit transactional context.");

	    ptr = null;
	}
	else
	{
	    System.err.println("Error: trangrid_i.width - no implicit transactional context.");

	    System.exit(0);
	}
    
	return m_width;
    }

public void set (short n, short m, short value) throws SystemException
    {
	OTS_Current current = OTS.current();
	org.omg.CosTransactions.Control control = current.get_control();

	if (control != null)
	{
	    System.out.println("trangrid_i.set - found implicit transactional context!");

	    try
	    {
		org.omg.CosTransactions.Coordinator co = control.get_coordinator();

		co.register_resource(this);
	    }
	    catch (Exception e)
	    {
		System.err.println("trangrid_i.set - caught exception: "+e);
	    }
	}
	else
	    System.err.println("Error - set has no transaction control, therefore will ignore!");
	
	m_a[n][m] = value;
    }

public short get (short n, short m) throws SystemException
    {
	return m_a[n][m];
    }


public org.omg.CosTransactions.Vote prepare () throws SystemException, HeuristicMixed, HeuristicHazard

    {
	System.out.println("TRANGRID : PREPARE");
    
	return Vote.VoteCommit;
    }

public void rollback () throws SystemException, HeuristicCommit, HeuristicMixed, HeuristicHazard
    {
	System.out.println("TRANGRID : ROLLBACK");
    }

public void commit () throws SystemException, NotPrepared, HeuristicRollback, HeuristicMixed, HeuristicHazard
    {
	System.out.println("TRANGRID : COMMIT");
    }

public void forget () throws SystemException
    {
	System.out.println("TRANGRID : FORGET");
    }

public void commit_one_phase () throws SystemException, HeuristicHazard
    {
	System.out.println("TRANGRID : COMMIT_ONE_PHASE");
    }

private short m_height;  // store the height
private short m_width;   // store the width
private short[][] m_a;   // a 2-D array to store the grid data itself
 
};
