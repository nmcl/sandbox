package javax.jaxtx.compensation;

/**
 * Thrown if the system does not know about the transaction that
 * is associated with the operation being performed, or if there is
 * no transaction associated with the invoking thread.
 * 
 * @version 1.0
 *
 * $Id: UnknownCompensationTransactionException.java,v 1.1 2004/06/22 10:07:59 nmcl Exp $
 */

public class UnknownCompensationTransactionException extends Exception
{

    public UnknownCompensationTransactionException ()
    {
	super();
    }

    public UnknownCompensationTransactionException (String s)
    {
	super(s);
    }

}
