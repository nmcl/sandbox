/*
 * Copyright (C) 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ObjectStore.java,v 1.2 1998/04/09 15:47:39 nmcl Exp $
 */

package com.arjuna.JTI;

import com.arjuna.RPC.*;

import java.io.IOException;

public class ObjectStore
{

public ObjectStore (String host, int port)
    {
	sock = new ClientTCPPort();
	serverAddress = new PortAddress(host, port);
    }
	
public synchronized String create ()
    {
	try
	{
	    OutputBuffer[] request = new OutputBuffer[1];
	    OutputBuffer work = new OutputBuffer();

	    work.packInt(Opcode.CREATE);

	    request[0] = work;

	    InputBuffer[] reply = call(request);

	    if (reply[0].unpackBoolean())  // operation worked?
		return reply[0].unpackString();
	}
	catch (IOException e)
	{
	}

	return null;
    }

public synchronized boolean attach (String objId)
    {
	try
	{
	    OutputBuffer[] request = new OutputBuffer[1];
	    OutputBuffer work = new OutputBuffer();

	    work.packInt(Opcode.ATTACH);
	    work.packString(objId);
	    
	    request[0] = work;

	    InputBuffer[] reply = call(request);

	    return reply[0].unpackBoolean();
	}
	catch (IOException e)
	{
	}

	return false;
    }
    
public synchronized boolean detach (String objId)
    {
	try
	{
	    OutputBuffer[] request = new OutputBuffer[1];
	    OutputBuffer work = new OutputBuffer();

	    work.packInt(Opcode.DETACH);
	    work.packString(objId);
	    
	    request[0] = work;

	    InputBuffer[] reply = call(request);

	    return reply[0].unpackBoolean();
	}
	catch (IOException e)
	{
	}

	return false;
    }

public synchronized String begin ()
    {
	try
	{
	    OutputBuffer[] request = new OutputBuffer[1];
	    OutputBuffer work = new OutputBuffer();

	    work.packInt(Opcode.BEGIN);
	    
	    request[0] = work;

	    InputBuffer[] reply = call(request);

	    if (reply[0].unpackBoolean())
		return reply[0].unpackString();
	}
	catch (IOException e)
	{
	}

	return null;
    }

public synchronized boolean commit (String tranID)
    {
	try
	{
	    OutputBuffer[] request = new OutputBuffer[1];
	    OutputBuffer work = new OutputBuffer();

	    work.packInt(Opcode.COMMIT);
	    work.packString(tranID);
	    
	    request[0] = work;

	    InputBuffer[] reply = call(request);

	    return reply[0].unpackBoolean();
	}
	catch (IOException e)
	{
	}

	return false;
    }

public synchronized boolean abort (String tranID)
    {
	try
	{
	    OutputBuffer[] request = new OutputBuffer[1];
	    OutputBuffer work = new OutputBuffer();

	    work.packInt(Opcode.ABORT);
	    work.packString(tranID);
	    
	    request[0] = work;

	    InputBuffer[] reply = call(request);

	    return reply[0].unpackBoolean();
	}
	catch (IOException e)
	{
	}

	return false;
    }        
	
public synchronized byte[] get (String tranID, String objId)
    {
	try
	{
	    OutputBuffer[] request = new OutputBuffer[1];
	    OutputBuffer work = new OutputBuffer();

	    work.packInt(Opcode.GET);
	    work.packString(tranID);
	    work.packString(objId);
	    
	    request[0] = work;

	    InputBuffer[] reply = call(request);

	    if (reply[0].unpackBoolean())
		return reply[0].unpackBytes();
	}
	catch (IOException e)
	{
	}

	return null;
    }
    
public synchronized boolean set (String tranID, byte[] data, String objId)
    {
	try
	{
	    OutputBuffer[] request = new OutputBuffer[1];
	    OutputBuffer work = new OutputBuffer();

	    work.packInt(Opcode.SET);
	    work.packString(tranID);
	    work.packString(objId);
	    work.packBytes(data);

	    request[0] = work;

	    InputBuffer[] reply = call(request);
	    
	    return reply[0].unpackBoolean();
	}
	catch (IOException e)
	{
	}

	return false;
    }

private InputBuffer[] call (OutputBuffer[] output) throws IOException
    {
	sock.setupConnection(serverAddress);
	sock.sendMessage(output);

	InputBuffer[] reply = sock.receiveMessage();

	sock.endConnection();
	
	return reply;
    }
    
private ClientTCPPort sock;
private PortAddress serverAddress;

}
