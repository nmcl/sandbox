package javax.jaxtx.model.btp.opt;

import javax.jaxtx.exceptions.JAXTXException;
    
/**
 * Thrown if an inferior is registered multiple times.
 */

public class DuplicateInferiorException extends JAXTXException
{

    public DuplicateInferiorException ()
    {
	super();
    }

    public DuplicateInferiorException (String s)
    {
	super(s);
    }

    public DuplicateInferiorException (String s, int errorcode)
    {
	super(s, errorcode);
    }

}
