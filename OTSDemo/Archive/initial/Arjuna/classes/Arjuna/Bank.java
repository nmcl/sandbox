import ClientTCPPort;
import PortAddress;
import InputBuffer;
import OutputBuffer;

import java.io.*;

import java.io.IOException;

class Bank
{
    
public Bank ()
    {
		sock = new ClientTCPPort();
		serverAddress = new PortAddress(Constants.ArjunaHost, Constants.ArjunaPort);
    }

public int insert (int pin, int amount) throws IOException
    {
		OutputBuffer[] request = new OutputBuffer[1];
		request[0] = new OutputBuffer();

		request[0].packInt(Opcodes.OP_INSERT);
		request[0].packInt(pin);
		request[0].packInt(amount);

		InputBuffer[] reply = call(request);

		if (reply[0] != null)
			return reply[0].unpackInt();
		else
			return BankOutcome.UNKNOWN;
    }

public int withdraw (int pin, int amount) throws IOException
    {
	 	OutputBuffer[] request = new OutputBuffer[1];
		request[0] = new OutputBuffer();

		request[0].packInt(Opcodes.OP_WITHDRAW);
		request[0].packInt(pin);
		request[0].packInt(amount);

		InputBuffer[] reply = call(request);

		if (reply[0] != null)
			return reply[0].unpackInt();
		else
			return BankOutcome.UNKNOWN;
    }
    
public int inspect (int pin, Cash amount) throws IOException
    {
	 	OutputBuffer[] request = new OutputBuffer[1];
		request[0] = new OutputBuffer();

		request[0].packInt(Opcodes.OP_INSPECT);
		request[0].packInt(pin);

		InputBuffer[] reply = call(request);

		if (reply[0] != null)
		{
			int result = reply[0].unpackInt();

			if (result == BankOutcome.DONE)
				amount.setValue(reply[0].unpackInt());

			return result;
		}
		else
			return BankOutcome.UNKNOWN;
    }

public int transfer (int pinFrom, int pinTo, int amount) throws IOException
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
	else
		return BankOutcome.UNKNOWN;
}

private boolean sendAck ()
    {
		/*
		 * Always send an ack.
		 */
		
		try
		{
			OutputBuffer[] output = new OutputBuffer[1];

			output[0] = new OutputBuffer();

			output[0].packInt(Opcodes.OP_DONE);

			sock.sendMessage(output);
		}
		catch (IOException e)
		{
			return false;
		}
		
		return true;
    }

/*
 * Do the actual work of sending the message and getting the
 * reply.
 *
 * The protocol is:
 *
 * buffer 1: is always the action protocol outcome (prepare, abort, commit).
 * buffer 2: only present if we abort or commit, and contains the operation outcome
 *			 followed by any other data, such as the cash amount withdrawn.
 */
    
private InputBuffer[] call (OutputBuffer[] output) throws IOException
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

		if (result == Opcodes.OP_PREPARE)
		{	
			if (sendAck())
			{
				input = sock.receiveMessage();

				/*
				 * Check buffer 0 in case the action aborted.
				 */

				result = input[0].unpackInt();

				if (result != Opcodes.OP_COMMIT)
					throw(new IOException("Atomic action aborted during prepare phase."));

				toReturn[0] = input[1];
			}
			else
				throw(new IOException("Receive reply error."));
		}
		else
		{
			/*
			 * The action aborted. There will be a second buffer
			 * which will contain the reason for the abort. Return it.
			 */

			toReturn[0] = input[1];
		}

		sock.endConnection();

		return toReturn;
    }    

private ClientTCPPort sock;
private PortAddress   serverAddress;
    
};
