package javax.jaxtx.compensation;

/**
 * Thrown if the participant has not been registered with
 * the compensation transaction and an attempt is made to
 * unregister it.
 * 
 * @version 1.0
 *
 * $Id: NotRegisteredException.java,v 1.1 2004/06/22 10:07:59 nmcl Exp $
 */

public class NotRegisteredException extends Exception
{

    public NotRegisteredException ()
    {
	super();
    }

    public NotRegisteredException (String s)
    {
	super(s);
    }

}
