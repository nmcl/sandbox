package javax.jaxtx.compensation;

/**
 * Thrown if the compensation transaction cancelled when it was asked
 * to confirm.
 * 
 * @version 1.0
 *
 * $Id: CompensationTransactionCancelledException.java,v 1.1 2004/06/22 10:07:59 nmcl Exp $
 */

public class CompensationTransactionCancelledException extends Exception
{

    public CompensationTransactionCancelledException ()
    {
	super();
    }

    public CompensationTransactionCancelledException (String s)
    {
	super(s);
    }

}
