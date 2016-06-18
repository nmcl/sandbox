package javax.jaxtx.exceptions;

/**
 * Thrown if an attempt is made to delist a participant that the coordinator
 * has no knowledge about.
 */

public class UnknownParticipantException extends JAXTXException
{

    public UnknownParticipantException ()
    {
	super();
    }

    public UnknownParticipantException (String s)
    {
	super(s);
    }

    public UnknownParticipantException (String s, int errorcode)
    {
	super(s, errorcode);
    }
    
}


