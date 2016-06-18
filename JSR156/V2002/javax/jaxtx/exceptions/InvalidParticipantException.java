package javax.jaxtx.exceptions;

/**
 * The participant reference was invalid. Typically this means that
 * the coordinator does not support the type of participant that
 * was passed to it on an enrol request.
 */

public class InvalidParticipantException extends JAXTXException
{

    public InvalidParticipantException ()
    {
	super();
    }

    public InvalidParticipantException (String s)
    {
	super(s);
    }

    public InvalidParticipantException (String s, int errorcode)
    {
	super(s, errorcode);
    }
    
}


