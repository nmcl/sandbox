/*
 * Copyright (C) 1997, 1998, 1999,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ORBServices.javatmpl,v 1.10.2.1.4.4 1999/09/10 14:24:47 nmcl Exp $
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
 * Copyright (C) 1996, 1997, 1998, 1999,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OrbPortability_krc.h,v 1.3 1999/09/10 13:49:11 nmcl Exp $
 */















































package com.arjuna.OrbCommon;


import org.omg.CosNaming.*;

import java.io.*;
import java.io.LineNumberReader;
import java.io.FileReader;

import java.io.FileNotFoundException;
import java.io.IOException;
import org.omg.CORBA.SystemException;
import org.omg.CORBA.BAD_PARAM;
import org.omg.CORBA.UserException;
import org.omg.CosNaming.NamingContextPackage.InvalidName;
import org.omg.CosNaming.NamingContextPackage.CannotProceed;
import org.omg.CosNaming.NamingContextPackage.NotFound;

/**
 * An attempt at some ORB portable ways of accessing ORB services.
 */

public class ORBServices
{

    /**
     * The various means used to locate a service.
     *
     */

    /*
     * Could have a method that tries all of them.
     */
    
public static final int RESOLVE_INITIAL_REFERENCES = 0;
public static final int NAME_SERVICE = 1;
public static final int CONFIGURATION_FILE = 2;
public static final int FILE = 3;
public static final int NAMED_CONNECT = 4;
public static final int BIND_CONNECT = 5;
    
    /**
     * The default implementation (probably specified on a per-ORB basis.)
     */
    
public static org.omg.CORBA.Object getService (String serviceName,
						   Object[] params) throws InvalidName, CannotProceed, NotFound, IOException
    {

	return getService(serviceName, params, CONFIGURATION_FILE);

    }
    
public static org.omg.CORBA.Object getService (String serviceName, Object[] params,
						   int mechanism) throws InvalidName, CannotProceed, NotFound, IOException
    {
	org.omg.CORBA.Object objRef = null;
	
	switch (mechanism)
	{
	case RESOLVE_INITIAL_REFERENCES:
	    {
		try
		{
		    objRef = ORBInterface.orb().resolve_initial_references(serviceName);
		}
		catch (Exception e)
		{
		    System.err.println("ORBServices.getService - resolve_initial_references on "+serviceName+" failed");

		    throw new NotFound();
		}
	    }
	break;
	case NAME_SERVICE:
	    {
		String kind = ((params == null) ? null : (String) params[0]);
		
		try
		{
		    org.omg.CORBA.Object nsRef = ORBInterface.orb().resolve_initial_references(ORBServices.nameService);
	    
		    NamingContext ncRef = NamingContextHelper.narrow(nsRef);
	    
		    // bind the Object Reference in Naming
		    
		    NameComponent nc = new NameComponent(serviceName, kind);
		    NameComponent path[] = {nc};

		    objRef = ncRef.resolve(path);
		}
		catch (UserException e)
		{
		    throw new InvalidName();
		}
	    }
	break;
	case CONFIGURATION_FILE:
	    {
		String cosservicesRoot = ORBPropertyManager.getProperty(environment.INITIAL_REFERENCES_ROOT, "/usr/local/JTSArjuna/etc/");
		String configLocation = ORBPropertyManager.getProperty(environment.INITIAL_REFERENCES_FILE, "CosServices.cfg");
		String configFile = cosservicesRoot+File.separatorChar+configLocation;
		LineNumberReader input;

		try
		{
		    input = new LineNumberReader(new FileReader(configFile));
		}
		catch (FileNotFoundException e)
		{
		    System.err.println("ORBServices.getService - could not open config file: "+configFile);

		    throw new CannotProceed();
		}
    
		String ior = null;
    
		try
		{
		    boolean finished = false;
	    
		    while ((ior == null) && !finished)
		    {
			String line = input.readLine();

			if (line == null)
			    finished = true;
			else
			{
			    int occur = line.indexOf(serviceName);

			    if (occur == 0)  // should be first on line
				ior = line.substring(serviceName.length() +1);  // +1 for space separator
			}
		    }
		}
		catch (Exception e)
		{
		    System.err.println("ORBServices.getService - caught unexpected exception: "+e);

		    throw new CannotProceed();
		}

		if (ior == null)
		{
		    System.err.println("ORBServices.getService - could not find service: "+
				       serviceName+" in configuration file: "+configFile);

		    throw new NotFound();
		}
		else
		{
		    try
		    {
			objRef = ORBInterface.orb().string_to_object(ior);
		    }
		    catch (Exception e)
		    {
			objRef = null;
		    }
		}
	    }
	break;
	case FILE:
	    {
		try
		{
		    String fileDir = ORBPropertyManager.getProperty(environment.FILE_DIR, "");
		    File f = new File(fileDir+serviceName);
		    FileInputStream ifile = new FileInputStream(f);
		    int size = (int) f.length();
		    byte b[] = new byte[size];

		    ifile.read(b);
		    ifile.close();
	
		    String objString = new String(b);
		    objRef = ORBInterface.orb().string_to_object(objString);

		    objString = null;
		}
		catch (FileNotFoundException e)
		{
		    throw new NotFound();
		}
	    }
	break;
	case NAMED_CONNECT:
	    {

		if ((params == null) || (params.length < 1))
		{
		    System.err.println("ORBServices.getService - NAMED_CONNECT no host or port specified.");
		    throw new CannotProceed();
		}

		String host = (String) params[0];

		if (host == null)
		{
		    System.err.println("ORBServices.getService - NAMED_CONNECT no host specified.");
		    throw new CannotProceed();
		}

		if ((params.length == 1) || (params[1] == null))
		{
		    System.err.println("ORBServices.getService - NAMED_CONNECT no port specified.");
		    throw new CannotProceed();
		}

		Integer port = (Integer) params[1];

		try
		{
		    objRef = ((com.ooc.CORBA.ORB) ORBInterface.orb()).get_inet_object(host, port.intValue(), serviceName);
		}
		catch (Exception e)
		{
		    System.err.println("ORBServices.getService - NAMED_CONNECT caught exception: "+e);
		    throw new InvalidName();
		}

	    }
	break;
	}

	return objRef;
    }

    /**
     * The default implementation (probably specified on a per-ORB basis.)
     */
    
public static void registerService (org.omg.CORBA.Object objRef, String serviceName, Object[] params) throws InvalidName, IOException, CannotProceed, NotFound
    {

	registerService(objRef, serviceName, params, CONFIGURATION_FILE);

    }
    
public static void registerService (org.omg.CORBA.Object objRef, String serviceName, Object[] params,
				    int mechanism) throws InvalidName, IOException, CannotProceed, NotFound
    {
	switch (mechanism)
	{
	case NAME_SERVICE:
	    {
		String kind = ((params == null) ? null : (String) params[0]);
		
		try
		{
		    org.omg.CORBA.Object initServ = ORBInterface.orb().resolve_initial_references(ORBServices.nameService);
		    NamingContext rootContext = NamingContextHelper.narrow(initServ);
		    NameComponent[] contextName = new NameComponent[1];
		    contextName[0] = new NameComponent(serviceName, kind);
	    
		    rootContext.rebind(contextName, objRef);

		    if (ORBDebugController.debugAllowed())
			System.out.println("ORBServices.registerService - object "+serviceName+" registered with name service.");
		}
		catch (org.omg.CORBA.ORBPackage.InvalidName e)
		{
		    throw new InvalidName();
		}
	    }
	break;
	case CONFIGURATION_FILE:
	    {
		String cosservicesRoot = ORBPropertyManager.getProperty(environment.INITIAL_REFERENCES_ROOT, "/usr/local/JTSArjuna/etc/");
		String configLocation = ORBPropertyManager.getProperty(environment.INITIAL_REFERENCES_FILE, "CosServices.cfg");
		String configFile = cosservicesRoot+File.separatorChar+configLocation;
		LineNumberReader input = null;
		String objString = ORBInterface.orb().object_to_string(objRef);
		File currFile = null;
		String newFileName = configFile+ORBServices.tmpFile;

		try
		{
		    currFile = new File(configFile);
		    input = new LineNumberReader(new FileReader(currFile));
		}
		catch (FileNotFoundException e)
		{
		    if (ORBDebugController.debugAllowed())
			System.err.println("ORBServices.registerService - could not open config file: "+configFile);
		    
		    currFile = null;
		    input = null;

		    /*
		     * File not present, so this must be the first
		     * entry.
		     */
		    
		    newFileName = configFile;  // file does not exist, so write directly.
		}

		File nuFile = new File(newFileName);
		PrintWriter outputFile = new PrintWriter(new FileOutputStream(nuFile));
		boolean found = false;

		if (input != null)
		{
		    String line = null;

		    do
		    {
			line = input.readLine();

			if (line != null)
			{
			    int occur = line.indexOf(serviceName);

			    if (occur == 0)  // should be first on line
			    {
				if (line.substring(serviceName.length() +1) != null) // found old line?
				{
				    found = true;
				    outputFile.println(serviceName+" "+objString);
				}
			    }
			    else
				outputFile.println(line);

			    outputFile.flush();
			}
			
		    } while (line != null);
		}

		if (!found)
		{
		    outputFile.println(serviceName+" "+objString);
		    outputFile.flush();
		}

		outputFile.close();

		if (input != null)  // file existed.
		{
		    input.close();
		    
		    if (currFile.exists())
		    {
			currFile.delete();
		    }
		    
		    nuFile.renameTo(currFile);
		    input = null;
		}
		
		newFileName = null;
		outputFile = null;
		nuFile = null;
		
		if (ORBDebugController.debugAllowed())
		    System.out.println("ORBServices.registerService - object "+serviceName+" registered with configuration file: "+configFile);
	    }
	break;
	case FILE:
	    {
		String fileDir = ORBPropertyManager.getProperty(environment.FILE_DIR, "");
		FileOutputStream ofile = new FileOutputStream(fileDir+serviceName);
		String objString = ORBInterface.orb().object_to_string(objRef);
		byte b[] = objString.getBytes();

		ofile.write(b);
		ofile.close();

		if (ORBDebugController.debugAllowed())
		    System.out.println("ORBServices.registerService - object "+serviceName+" reference file created: "+fileDir+serviceName);
	    }
	break;
	case NAMED_CONNECT:
	    {

		Object[] name = new Object[1];
		name[0] = serviceName;
		
		if (!ORBInterface.objectIsReady(objRef, name))
		    throw new CannotProceed();

	    }
	break;
	default:
	    break;
	}
    }
		    
public static final String nameService = "NameService";
public static final String transactionService = "TransactionService";
    
public static final String otsKind = "OTS";

private static final String tmpFile = ".tmp";

};
