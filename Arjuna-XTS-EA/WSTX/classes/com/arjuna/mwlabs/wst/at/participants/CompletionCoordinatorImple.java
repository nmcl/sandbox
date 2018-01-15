/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: CompletionCoordinatorImple.java,v 1.6 2003/04/04 15:21:53 nmcl Exp $
 */

package com.arjuna.mwlabs.wst.at.participants;

import com.arjuna.mw.wscf.model.twophase.api.CoordinatorManager;

import com.arjuna.mw.wsas.activity.*;

import com.arjuna.wst.*;
import com.arjuna.wst.dispatcher.*;

/**
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: CompletionCoordinatorImple.java,v 1.6 2003/04/04 15:21:53 nmcl Exp $
 * @since 1.0.
 */

public class CompletionCoordinatorImple implements com.arjuna.wst.CompletionCoordinatorParticipant
{

    public CompletionCoordinatorImple (CoordinatorManager cm, ActivityHierarchy hier, CompletionDispatcher ccd)
    {
	_cm = cm;
	_hier = hier;
	_ccd = ccd;
    }
    
    public void commit () throws TransactionRolledBackException, UnknownTransactionException, SystemException
    {
	try
	{
	    if (_hier != null)
		_cm.resume(_hier);

	    _cm.confirm();
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
	    if (_ccd != null)
		_ccd.deactivateObject(this);
	}
    }
    
    public void rollback () throws UnknownTransactionException, SystemException
    {
	try
	{
	    if (_hier != null)
		_cm.resume(_hier);
	    
	    _cm.cancel();
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
	    if (_ccd != null)
		_ccd.deactivateObject(this);
	}
    }

    private CoordinatorManager   _cm;
    private ActivityHierarchy    _hier;
    private CompletionDispatcher _ccd;
    
}
