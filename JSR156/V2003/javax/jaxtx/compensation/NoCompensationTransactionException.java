package javax.jaxtx.compensation;

/**
 * Thrown if there is no transaction associated with the target
 * object/thread.
 * 
 * @version 1.0
 *
 * $Id: NoCompensationTransactionException.java,v 1.1 2004/06/22 10:29:12 nmcl Exp $
 */

public class NoCompensationTransactionException extends Exception
{

    public NoCompensationTransactionException ()
    {
	super();
    }

    public NoCompensationTransactionException (String s)
    {
	super(s);
    }

}
