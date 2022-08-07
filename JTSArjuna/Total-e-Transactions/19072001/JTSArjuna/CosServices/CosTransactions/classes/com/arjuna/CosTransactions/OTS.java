/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS.javatmpl,v 1.2.2.1.4.2.2.1.2.1.6.2.10.3.2.4.2.1.4.3.2.1.16.1.6.1.4.1 2001/06/28 14:09:10 nmcl Exp $
 */

/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OrbPortability.h,v 1.1 2000/02/25 14:09:59 nmcl Exp $
 */




/*
 * Try to get around the differences between Ansi CPP and
 * K&R cpp with concatenation.
 */


/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: OrbPortability_stdc.h,v 1.2.4.1.2.3.26.1.2.2.4.2 2001/02/02 11:51:52 nmcl Exp $
 */
































































package com.arjuna.CosTransactions;

import com.arjuna.OrbCommon.*;
import com.arjuna.ArjunaCommon.Common.*;
import org.omg.CosTransactions.*;
import com.arjuna.ArjunaOTS.*;
import java.io.PrintStream;


import org.omg.CORBA.SystemException;
import org.omg.CORBA.BAD_PARAM;
import org.omg.CORBA.UNKNOWN;
import org.omg.CORBA.UserException;
import org.omg.CORBA.ORBPackage.InvalidName;
import java.io.IOException;

/*
 * Default visibility.
 */

class StartupOTS
{

public StartupOTS ()
    {
	if (OTS._theFactory == null)
	{
	    /*
	     * Only check once, when the factory is first created.
	     */
		
	    String transactionManager = PropertyManager.getProperty(OTS_Environment.OTS_TRANSACTION_MANAGER, "NO");
	    int resolver = ORBServices.CONFIGURATION_FILE;

	    boolean requireTransactionManager = false;

	    if (transactionManager.compareTo("YES") == 0)
	    {
		String resolveMechanism = PropertyManager.getProperty(ORBEnvironment.RESOLVE_SERVICE, "CONFIGURATION_FILE");

		if (resolveMechanism.compareTo("NAME_SERVICE") == 0)
		    resolver = ORBServices.NAME_SERVICE;
		else
		{
		    if (resolveMechanism.compareTo("BIND_CONNECT") == 0)
			resolver = ORBServices.BIND_CONNECT;
		}
	    }
		
	    if (requireTransactionManager)
	    {
		try
		{
		    if (resolver != ORBServices.BIND_CONNECT)
		    {


			String[] params = new String[1];

			params[0] = ORBServices.otsKind;
		    
			org.omg.CORBA.Object obj = ORBServices.getService(ORBServices.transactionService,
									      params, resolver);

			params = null;
		    
			OTS._theFactoryRef =  org.omg.CosTransactions.TransactionFactoryHelper.narrow(obj) ;
		    }
		    else
		    {

			ErrorStream.fatal().println("bind not supported for resolving TransactionService.");

			throw new com.arjuna.ArjunaCommon.Common.FatalError("bind not supported.");

		    }
		    
		    if (OTS._theFactoryRef == null)
			throw new BAD_PARAM();
		}
		catch (InvalidName e1)
		{
		    ErrorStream.fatal().println("Resolution of OTS server failed - invalid name: "+e1);

		    throw new com.arjuna.ArjunaCommon.Common.FatalError(e1.toString());
		}
		catch (BAD_PARAM ex1)
		{
		    ErrorStream.fatal().println("Resolution of OTS server failed - invalid name: "+ex1);

		    throw new com.arjuna.ArjunaCommon.Common.FatalError(ex1.toString());
		}
		catch (IOException e2)
		{
		    ErrorStream.fatal().println("Resolution of OTS server failed - IOException: "+e2);

		    throw new com.arjuna.ArjunaCommon.Common.FatalError(e2.toString());
		}
		catch (SystemException e3)
		{
		    ErrorStream.fatal().println("Resolution of OTS server failed - SystemException: "+e3);

		    throw new com.arjuna.ArjunaCommon.Common.FatalError(e3.toString());
		}
		catch (UserException e4)
		{
		    ErrorStream.fatal().println("Resolution of OTS server failed - UserException: "+e4);

		    throw new com.arjuna.ArjunaCommon.Common.FatalError(e4.toString());
		}
	    }
	    else
	    {
		/* force to be local */

		OTS._theFactory = new OTS_Factory();
	    }
	}

	if (OTS._current == null)
	{
	    try
	    {
		OTS._current = new OTS_Current();
		OTS._currentHandle = new  org.omg.CosTransactions. CurrentPOATie (OTS._current);

		OAInterface.objectIsReady( OTS._currentHandle ) ;
		
		OTS._currentRef =  org.omg.CosTransactions.CurrentHelper.narrow(OAInterface.corbaReference( OTS._currentHandle )) ;
	    }
	    catch (Exception e)
	    {
		OTS._current = null;
		
		ErrorStream.fatal().println("OTS.current: "+e);
		e.printStackTrace(ErrorStream.stream());
		
		throw new com.arjuna.ArjunaCommon.Common.FatalError("OTS.current: "+e.toString());
	    }
	}


    }
 
};

/*
 * Default visibility.
 */

class ShutdownOTS extends OAPreShutdown
{

public ShutdownOTS ()
    {
	super("ShutdownOTS");
    }

public void work ()
    {
	OTS.purge();
    }
 
};

/**
 * This class is essentially here for convenience purposes, and until
 * all ORBs provide a means to set their initial references for Current
 * and the TransactionFactory.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: OTS.javatmpl,v 1.2.2.1.4.2.2.1.2.1.6.2.10.3.2.4.2.1.4.3.2.1.16.1.6.1.4.1 2001/06/28 14:09:10 nmcl Exp $
 * @since JTS 1.0.
 */

public class OTS
{

    /**
     * @return the Current object implementation. This has the advantage of
     * not needing to register the object with the ORB, which can affect
     * performance.
     */

public static OTS_Current current () throws org.omg.CORBA.SystemException
    {
	init();

	_current.contextManager().associate();

	return OTS._current;
    }

    /**
     * @return the Current object.
     */

public static org.omg.CosTransactions.Current get_current () throws org.omg.CORBA.SystemException
    {
	init();

	_current.contextManager().associate();

	return OTS._currentRef;
    }

    /**
     * @return the TransactionFactory object implementation. This has the
     * advantage of not needing to register the object withm the ORB, which
     * can affect performance.
     */

public static OTS_Factory factory () throws org.omg.CORBA.SystemException
    {
	init();

	return _theFactory;
    }

    /**
     * @return the TransactionFactory object.
     */

public static TransactionFactory get_factory () throws org.omg.CORBA.SystemException
    {
	init();

	if (_theFactoryRef == null)
	{
	    try
	    {
		_theFactoryRef = _theFactory.getReference();
	    }
	    catch (Exception e)
	    {
		throw new UNKNOWN();
	    }
	}
	
	return _theFactoryRef;
    }

    /**
     * Is a co-located TransactionFactory required?
     */

public static boolean localFactory ()
    {
	init();

	return (_theFactory != null);
    }

    /**
     * Used to destroy a transaction control. Normally garbage collection
     * will take care of this, but in certain circumstances (e.g., a context
     * is propagated implicitly but we do not use interposition and we
     * have to manufacture a local control object) it is not possible for the
     * OTS to know when controls can be removed. This is a problem with the
     * specification and CORBA in general.
     */

public static void destroyControl (OTS_Control control) throws ActiveTransaction, ActiveThreads, BadControl, Destroyed, SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_OTS, "OTS::destroyControl ( "+control+" )");
	}
	
	if (control == null)
	    throw new BadControl();

	/*
	 * Just in case control is a top-level transaction, and has
	 * been registered with the reaper, we need to get it removed.
	 *
	 * Don't bother if the reaper has not been created.
	 */
    
	if (OTS_TransactionReaper.transactionReaper() != null)
	{
	    Coordinator coord = null;
	
	    try
	    {
		coord = control.get_coordinator();
	    }
	    catch (Exception e)
	    {
		coord = null;  // nothing else we can do!
	    }

	    if (coord != null)
	    {
		try
		{
		    if (coord.is_top_level_transaction())
		    {
			/*
			 * Transaction is local, but was registered as
			 * a Control. If this is a performance hit then
			 * add explicit add/removes for local instances.
			 */

			if (DebugController.enabled())
			{
			    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
								 FacilityCode.FAC_OTS, "OTS::destroyControl - removing control from reaper.");
			}

			OTS_TransactionReaper.transactionReaper().remove((OTS_Control) control);
		    }
		}
		catch (Exception e)
		{
		}

		coord = null;
	    }
	}
	
	/*
	 * Watch out for conflicts with multiple threads deleting
	 * the same control!
	 */

	/*
	 * If local, then delete it here, rather than
	 * calling the destroy method.
	 *
	 * Possible problem if a local factory is being accessed
	 * remotely?
	 */

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_OTS, "OTS::destroyControl - local transaction: "+control.get_uid());
	}

	control.destroy();
	    
	control = null;
    }

    /**
     * Destroy the transaction control.
     */
	
public static void destroyControl (Control control) throws ActiveTransaction, ActiveThreads, BadControl, Destroyed, SystemException
    {
	if (control == null)
	    throw new BadControl();
	
	OTS_Control lCont = OTS_Helper.localControl(control);

	if (lCont != null)
	{
	    destroyControl(lCont);
	}
	else
	{
	    /*
	     * Just in case control is a top-level transaction, and has
	     * been registered with the reaper, we need to get it removed.
	     *
	     * Don't bother if the reaper has not been created.
	     */
    
	    if (OTS_TransactionReaper.transactionReaper() != null)
	    {
		Coordinator coord = null;
	
		try
		{
		    coord = control.get_coordinator();
		}
		catch (Exception e)
		{
		    coord = null;  // nothing else we can do!
		}

		if (coord != null)
		{
		    try
		    {
			if (coord.is_top_level_transaction())
			    OTS_TransactionReaper.transactionReaper().remove(control);
		    }
		    catch (Exception e)
		    {
		    }

		    coord = null;
		}
	    }
    
	    /*
	     * Watch out for conflicts with multiple threads deleting
	     * the same control!
	     */

	    if (DebugController.enabled())
	    {
		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						     FacilityCode.FAC_OTS, "OTS::destroyControl - remote control.");
	    }

	    /*
	     * Remote transaction, so memory management is different!
	     */
	
	    ActionControl action = null;
	    
	    try
	    {
		action =  com.arjuna.ArjunaOTS.ActionControlHelper.narrow(control) ;

		if (action == null)
		    throw new BAD_PARAM();
	    }
	    catch (Exception e)
	    {
		action = null;
	    }

	    if (action != null)
	    {
		if (DebugController.enabled())
		{
		    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
							 FacilityCode.FAC_OTS, "OTS::destroyControl - Arjuna control.");
		}

		/*
		 * Is an Arjuna control, so we can call destroy on it?
		 */

		action.destroy();
		
		action = null;
		control = null;
	    }
	    else
	    {
		/*
		 * Just call release on the control.
		 *
		 * We could throw a BadControl exception, but
		 * what would that do for the programmer?
		 */

		control = null;
	    }
	}
    }

public static final void setLocalSlotId (int slotId)
    {
	_localSlotId = slotId;
    }
    
public static final int getLocalSlotId ()
    {
	return _localSlotId;
    }

public static final void setReceivedSlotId (int slotId)
    {
	_receivedSlotId = slotId;
    }
    
public static final int getReceivedSlotId ()
    {
	return _receivedSlotId;
    }

    /**
     * Tidy-up the system prior to exiting.
     */

protected static void purge ()
    {
	if (OTS._currentRef != null)
	{
	    OAInterface.shutdownObject( OTS._currentHandle ) ;
	    OTS._current = null;
	}
	
	if (OTS._theFactoryRef != null)
	{
	    OAInterface.shutdownObject( OTS._theFactory ) ;
	    OTS._theFactory = null;
	}
    }

private static final synchronized void init ()
    {
	if (_current == null)
	{
	    StartupOTS s = new StartupOTS();
	}
    }
    
public static final int serviceId = 0xDEADBEEF;

static OTS_Factory		       _theFactory = null;
static TransactionFactory	       _theFactoryRef = null;
static OTS_Current		       _current = null;
static org.omg.CosTransactions.Current _currentRef = null;
static  org.omg.CosTransactions. CurrentPOATie  _currentHandle;

private static int _localSlotId = -1;
private static int _receivedSlotId = -1;

    static
    {
	OAInterface.addPreOAShutdown(new ShutdownOTS());

	if (!com.arjuna.CosTransactions.Implementation.Implementations.added())
	    com.arjuna.CosTransactions.Implementation.Implementations.initialise();
    }

}
