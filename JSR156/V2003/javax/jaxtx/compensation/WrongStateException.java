package javax.jaxtx.compensation;

/**
 * Thrown if the state of the transaction service is not correct
 * for the operation being performed.
 * 
 * @version 1.0
 *
 * $Id: WrongStateException.java,v 1.1 2004/06/22 10:07:59 nmcl Exp $
 */

public class WrongStateException extends Exception
{

    public WrongStateException ()
    {
	super();
    }

    public WrongStateException (String s)
    {
	super(s);
    }

}
