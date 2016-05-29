package BankAccount;

import java.io.*;

import java.io.IOException;

class Bank
{
    
public Bank ()
    {
	sock = new ClientTCPPort();
	serverAddress = new PortAddress(Constants.ArjunaHost, Constants.ArjunaPort);
    }

public boolean beginAction (int pin)
    {
	try
	{
	    OutputBuffer[] request = new OutputBuffer[1];

	    request[0] = createWork(Opcodes.OP_BEGINTX, pin);

	    InputBuffer[] reply = call(request);

	    if (reply[0] != null)
		return reply[0].unpackBoolean();
	}
	catch (IOException e)
	{
	}
	
	return false;	
    }

public boolean commitAction (int pin)
    {
	try
	{
	    OutputBuffer[] request = new OutputBuffer[1];
	    
	    request[0] = createWork(Opcodes.OP_COMMITTX, pin);

	    InputBuffer[] reply = call(request);

	    if (reply[0] != null)
		return reply[0].unpackBoolean();
	}
	catch (IOException e)
	{
	}
	
	return false;	
    }

public boolean abortAction (int pin)
    {
	try
	{
	    OutputBuffer[] request = new OutputBuffer[1];

	    request[0] = createWork(Opcodes.OP_ABORTTX, pin);

	    InputBuffer[] reply = call(request);
	    
	    if (reply[0] != null)
		return reply[0].unpackBoolean();
	}
	catch (IOException e)
	{
	}
	
	return false;
    }
    
public int insert (int pin, int amount)
    {
	try
	{
	    OutputBuffer[] request = new OutputBuffer[1];
	    
	    request[0] = createWork(Opcodes.OP_INSERT, pin);
	    request[0].packInt(amount);

	    InputBuffer[] reply = call(request);

	    if (reply[0] != null)
		return reply[0].unpackInt();
	}
	catch (IOException e)
	{
	}
	
	return BankOutcome.UNKNOWN;
    }

public int withdraw (int pin, int amount)
    {
	try
	{
	    OutputBuffer[] request = new OutputBuffer[1];
	    
	    request[0] = createWork(Opcodes.OP_WITHDRAW, pin);
	    request[0].packInt(amount);

	    InputBuffer[] reply = call(request);
	
	    if (reply[0] != null)
		return reply[0].unpackInt();
	}
	catch (IOException e)
	{
	}
	
	return BankOutcome.UNKNOWN;
    }
    
public int inspect (int pin, Cash amount)
    {
	try
	{
	    OutputBuffer[] request = new OutputBuffer[1];
	    
	    request[0] = createWork(Opcodes.OP_INSPECT, pin);

	    InputBuffer[] reply = call(request);

	    if (reply[0] != null)
	    {
		int result = reply[0].unpackInt();

		if (result == BankOutcome.DONE)
		    amount.setValue(reply[0].unpackInt());
	    
		return result;
	    }
	}
	catch (IOException e)
	{
	}
	
	return BankOutcome.UNKNOWN;
    }

public int transfer (int pinFrom, int pinTo, int amount)
    {
	try
	{
	    OutputBuffer[] request = new OutputBuffer[1];
	    
	    request[0] = new OutputBuffer();
	    request[0].packInt(Opcodes.OP_TRANSFER);
	    request[0].packInt(pinFrom);
	    request[0].packInt(pinTo);
	    request[0].packInt(amount);

	    InputBuffer[] reply = call(request);

	    if (reply[0] != null)
		return reply[0].unpackInt();
	}
	catch (IOException e)
	{
	}
	
	return BankOutcome.UNKNOWN;
    }

private OutputBuffer createWork (int opcode, int pinNumber) throws IOException
    {
	OutputBuffer work = new OutputBuffer();

	work.packInt(opcode);
	work.packInt(pinNumber);

	return work;
    }
    
/*
 * Do the actual work of sending the message and getting the
 * reply.
 */
    
private InputBuffer[] call (OutputBuffer[] output) throws IOException
    {
	try
	{
	    InputBuffer[] toReturn = new InputBuffer[1];

	    sock.setupConnection(serverAddress);
	    sock.sendMessage(output);

	    /*
	     * Receive the reply. If the action prepares then there
	     * will only be a single buffer in the array - the result
	     * buffer comes when the action commits. If the action aborts
	     * then there will be a second buffer with the reason for
	     * the abort.
	     */

	    InputBuffer[] input = sock.receiveMessage();

	    int result = input[0].unpackInt();

	    /*
	     * Sequence should be prepare/commit.
	     */

	    if (result == Opcodes.OP_DONE)
	    {
		toReturn[0] = input[1];
	    }
	    else
		throw new IOException("Operation error");

	    sock.endConnection();

	    return toReturn;	    
	}
	catch (IOException e)
	{
	    sock.endConnection();

	    throw e;
	}
    }    

private ClientTCPPort sock;
private PortAddress serverAddress;
    
};
