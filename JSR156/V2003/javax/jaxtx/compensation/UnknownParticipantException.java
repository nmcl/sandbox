package javax.jaxtx.compensation;

/**
 * Thrown if the system does not know about the participant.
 * 
 * @version 1.0
 *
 * $Id: UnknownParticipantException.java,v 1.1 2004/06/22 10:07:59 nmcl Exp $
 */

public class UnknownParticipantException extends Exception
{

    public UnknownParticipantException ()
    {
	super();
    }

    public UnknownParticipantException (String s)
    {
	super(s);
    }

}
