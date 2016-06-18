package javax.jaxtx.model.btp.opt;

import javax.jaxtx.exceptions.JAXTXException;

/**
 * Thrown if an inferior cancels rather than confirming or preparing.
 */

public class InferiorCancelledException extends JAXTXException
{

    public InferiorCancelledException ()
    {
	super();
    }

    public InferiorCancelledException (String s)
    {
	super(s);
    }

    public InferiorCancelledException (String s, int errorcode)
    {
	super(s, errorcode);
    }

}
