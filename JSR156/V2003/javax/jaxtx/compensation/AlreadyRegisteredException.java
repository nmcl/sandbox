package javax.jaxtx.compensation;

/**
 * Thrown if the participant has already been registered with
 * the compensation transaction.
 * 
 * @version 1.0
 *
 * $Id: AlreadyRegisteredException.java,v 1.1 2004/06/22 10:07:58 nmcl Exp $
 */

public class AlreadyRegisteredException extends Exception
{

    public AlreadyRegisteredException ()
    {
	super();
    }

    public AlreadyRegisteredException (String s)
    {
	super(s);
    }

}
