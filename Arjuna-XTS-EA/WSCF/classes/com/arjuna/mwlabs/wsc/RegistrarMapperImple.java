/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: RegistrarMapperImple.java,v 1.7 2003/04/04 15:00:06 nmcl Exp $
 */

package com.arjuna.mwlabs.wsc;

import com.arjuna.wsc.RegistrarMapper;
import com.arjuna.wsc.Registrar;

import java.util.HashMap;

import javax.naming.InitialContext;

/**
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: RegistrarMapperImple.java,v 1.7 2003/04/04 15:00:06 nmcl Exp $
 * @since 1.0.
 */

public class RegistrarMapperImple implements RegistrarMapper
{

    public RegistrarMapperImple ()
    {
	_registrarMap = new HashMap();
	_serviceURI = null;
    }

    public RegistrarMapperImple (String jndiName, String serviceURI) throws Exception
    {
	_registrarMap = new HashMap();

	InitialContext initialContext = new InitialContext();
	
	initialContext.bind(jndiName, this);
	initialContext.bind(localName(), this);

	_serviceURI = serviceURI;
    }

    public String getServiceURI ()
    {
	return _serviceURI;
    }

    public void addRegistrar (String protocolIdentifier, Registrar registrar)
    {
	try
	{
	    _registrarMap.put(protocolIdentifier, registrar);

	    registrar.install(_serviceURI, protocolIdentifier);
	}
	catch (Exception ex)
	{
	    ex.printStackTrace();
	}
    }
    
    public void removeRegistrar (String protocolIdentifier)
    {
	try
	{
	    Registrar registrar = (Registrar) _registrarMap.remove(protocolIdentifier);

	    registrar.uninstall(_serviceURI, protocolIdentifier);
	}
	catch (Exception ex)
	{
	    ex.printStackTrace();
	}
    }
    
    /*
     * The registrar implementation must initialise the activity service
     * with the correct HLS.
     */

    public Registrar getRegistrar (String protocolIdentifier)
    {
	try
	{
	    return (Registrar) _registrarMap.get(protocolIdentifier);
	}
	catch (Exception ex)
	{
	    ex.printStackTrace();
	}

	return null;
    }

    public static final String localName ()
    {
	//	return "com/arjuna/mwlabs/wsc/registrarmapper";
	return "comarjunamwlabswscregistrarmapper";
    }

    private HashMap _registrarMap;
    private String  _serviceURI;
    
}

