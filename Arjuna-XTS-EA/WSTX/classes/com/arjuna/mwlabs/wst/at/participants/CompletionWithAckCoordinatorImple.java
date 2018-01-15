/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: CompletionWithAckCoordinatorImple.java,v 1.4 2003/04/04 15:00:56 nmcl Exp $
 */

package com.arjuna.mwlabs.wst.at.participants;

import com.arjuna.mw.wscf.model.twophase.api.CoordinatorManager;

import com.arjuna.mw.wsas.activity.*;

import com.arjuna.mwlabs.wst.at.coordinator.TxState;

import com.arjuna.wst.*;
import com.arjuna.wst.dispatcher.*;

/**
 * This participant drives the CompletionWithAck protocol. So we retain the
 * transaction state. However, the WS-T specification doesn't provide a way
 * by any user can get this state afterwards!
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: CompletionWithAckCoordinatorImple.java,v 1.4 2003/04/04 15:00:56 nmcl Exp $
 * @since 1.0.
 */

public class CompletionWithAckCoordinatorImple implements com.arjuna.wst.CompletionWithAckCoordinatorParticipant
{

    public CompletionWithAckCoordinatorImple (CoordinatorManager cm, ActivityHierarchy hier, CompletionWithAckDispatcher ccd)
    {
	_cm = cm;
	_hier = hier;
	_ccd = ccd;
    }

    public void commit () throws TransactionRolledBackException, UnknownTransactionException, SystemException
    {
	boolean committed = false;

	try
	{
	    if (_hier != null)
		_cm.resume(_hier);

	    _cm.confirm();

	    committed = true;
	}
	catch (com.arjuna.mw.wsas.exceptions.InvalidActivityException ex)
	{
	    throw new UnknownTransactionException();
	}
	catch (com.arjuna.mw.wsas.exceptions.WrongStateException ex)
	{
	    throw new SystemException(ex.toString());
	}
	catch (com.arjuna.mw.wsas.exceptions.ProtocolViolationException ex)
	{
	    //	    throw new HeuristicHazardException();
	    
	    throw new SystemException(ex.toString());
	}
	catch (com.arjuna.mw.wscf.exceptions.NoCoordinatorException ex)
	{
	    throw new UnknownTransactionException();
	}
	catch (com.arjuna.mw.wscf.model.twophase.exceptions.CoordinatorCancelledException ex)
	{
	    throw new TransactionRolledBackException();
	}
	catch (com.arjuna.mw.wscf.model.twophase.exceptions.HeuristicMixedException ex)
	{
	    //	    throw new HeuristicMixedException();

	    throw new SystemException(ex.toString());
	}
	catch (com.arjuna.mw.wscf.model.twophase.exceptions.HeuristicHazardException ex)
	{
	    //	    throw new HeuristicHazardException();

	    throw new SystemException(ex.toString());

	}
	catch (com.arjuna.mw.wsas.exceptions.NoPermissionException ex)
	{
	    throw new SystemException(ex.toString());
	}
	catch (com.arjuna.mw.wsas.exceptions.SystemException ex)
	{
	    throw new SystemException(ex.toString());
	}
	finally
	{
	    if (committed)
		TxState.setState(_hier, TxState.COMMITTED);
	    else
		TxState.setState(_hier, TxState.ABORTED);
	}
    }
    
    public void rollback () throws UnknownTransactionException, SystemException
    {
	boolean rolledback = false;

	try
	{
	    if (_hier != null)
		_cm.resume(_hier);
	    
	    _cm.cancel();

	    rolledback = true;
	}
	catch (com.arjuna.mw.wsas.exceptions.InvalidActivityException ex)
	{
	    throw new UnknownTransactionException();
	}
	catch (com.arjuna.mw.wsas.exceptions.WrongStateException ex)
	{
	    throw new SystemException(ex.toString());
	}
	catch (com.arjuna.mw.wsas.exceptions.ProtocolViolationException ex)
	{
	    throw new SystemException();
	}
	catch (com.arjuna.mw.wscf.exceptions.NoCoordinatorException ex)
	{
	    throw new UnknownTransactionException();
	}
	catch (com.arjuna.mw.wscf.model.twophase.exceptions.CoordinatorConfirmedException ex)
	{
	    throw new SystemException();
	}
	catch (com.arjuna.mw.wscf.model.twophase.exceptions.HeuristicMixedException ex)
	{
	    throw new SystemException(ex.toString());
	}
	catch (com.arjuna.mw.wscf.model.twophase.exceptions.HeuristicHazardException ex)
	{
	    throw new SystemException(ex.toString());
	}
	catch (com.arjuna.mw.wsas.exceptions.NoPermissionException ex)
	{
	    throw new SystemException(ex.toString());
	}
	catch (com.arjuna.mw.wsas.exceptions.SystemException ex)
	{
	    throw new SystemException(ex.toString());
	}
	finally
	{
	    if (rolledback)
		TxState.setState(_hier, TxState.ABORTED);
	    else
		TxState.setState(_hier, TxState.HEURISTIC);
	}	    
    }

    public void notified () throws UnknownTransactionException, SystemException
    {
	TxState.removeState(_hier);

	if (_ccd != null)
	    _ccd.deactivateObject(this);
    }

    private CoordinatorManager          _cm;
    private ActivityHierarchy           _hier;
    private CompletionWithAckDispatcher _ccd;
    
}
