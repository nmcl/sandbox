package javax.jaxtx.model.btp;

import javax.jaxtx.exceptions.JAXTXException;

/**
 * The transaction (atom or cohesion) cancelled when it was instructed
 * to confirm. This may occur if all participants were prepared and
 * made the same autonomous decision to confirm before the coordinator
 * had a chance to tell them to cancel.
 */

public class TransactionConfirmedException extends JAXTXException
{

    public TransactionConfirmedException ()
    {
	super();
    }

    public TransactionConfirmedException (String s)
    {
	super(s);
    }

    public TransactionConfirmedException (String s, int errorcode)
    {
	super(s, errorcode);
    }

}
