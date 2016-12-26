/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: grid_i.javatmpl,v 1.5.6.1 1999/02/22 11:10:10 nmcl Exp $
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

public class grid_i extends  TestModule ._gridImplBase 
{
    
public grid_i (int h, int w, String markerName)
    {


	m_height = h;   // set up height
	m_width = w;    // set up width
	// now allocate the 2-D array: as an array of pointers to 1-D arrays.
	m_a = new int [h][w];
    }

public grid_i (int h, int w)
    {
	m_height = h;   // set up height
	m_width = w;    // set up width
	// now allocate the 2-D array: as an array of pointers to 1-D arrays.
	m_a = new int [h][w];
    }

public int height () throws SystemException
    {
	return m_height;
    }

public int width () throws SystemException
    {
	return m_width;
    }

public void set (int n, int m, int value, Control cp) throws SystemException
    {
	try
	{
	    Coordinator co = cp.get_coordinator();
	    
	    if (co != null)
		co.register_resource(this);
	    else
		System.err.println("Error - no transaction coordinator!");
	    
	    m_a[n][m] = value;
	}
	catch (UserException e)
	{
	    throw new UNKNOWN();
	}
	catch (SystemException e)
	{
	    throw new UNKNOWN();
	}
    }

public int get (int n, int m, Control p) throws SystemException
    {
	return m_a[n][m];
    }


public org.omg.CosTransactions.Vote prepare () throws SystemException

    {
	System.out.println("GRID : PREPARE");
    
	return Vote.VoteCommit;
    }

public void rollback () throws SystemException, HeuristicCommit, HeuristicMixed, HeuristicHazard
    {
	System.out.println("GRID : ROLLBACK");
    }

public void commit () throws SystemException, NotPrepared, HeuristicRollback, HeuristicMixed, HeuristicHazard
    {
	System.out.println("GRID : COMMIT");
    }

public void forget () throws SystemException
    {
	System.out.println("GRID : FORGET");
    }

public void commit_one_phase () throws SystemException, HeuristicHazard
    {
	System.out.println("GRID : COMMIT_ONE_PHASE");
    }

private int m_height;  // store the height
private int m_width;   // store the width
private int[][] m_a;      // a 2-D array to store the grid data itself

};
