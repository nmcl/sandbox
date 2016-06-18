package javax.jaxtx.compensation;

/**
 * Thrown if the implementation does not support nested compensation
 * transactions.
 * 
 * @version 1.0
 *
 * $Id: NotSupportedException.java,v 1.1 2004/06/22 10:07:59 nmcl Exp $
 */

public class NotSupportedException extends Exception
{

    public NotSupportedException ()
    {
	super();
    }

    public NotSupportedException (String s)
    {
	super(s);
    }

}
