package javax.jaxtx.exceptions;

/**
 * This is the base class from which all JAXTX related exceptions
 * inherit. It provides several kinds of additional information:
 *
 * (i) a string describing the error.
 * (ii) an integer code for the error.
 */

public class JAXTXException extends Exception
{

    /**
     * Constructs a JAXTXException object; reason defaults to null and
     * errorcode defaults to 0.
     */

    public JAXTXException ()
    {
	super();
    }

    /**
     * Constructs a JAXTXException object with the specified reason.
     * errorcode defaults to 0.
     */

    public JAXTXException (String reason)
    {
	super(reason);
    }

    /**
     * Constructs a JAXTXException object with the specified reason and
     * errorcode.
     */

    public JAXTXException (String reason, int errorcode)
    {
	super(reason);
	
	_errorCode = errorcode;
    }

    /**
     * @return the errorcode associated with this exception.
     */

    public int getErrorCode ()
    {
	return _errorCode;
    }
    
    private int _errorCode;
    
}


