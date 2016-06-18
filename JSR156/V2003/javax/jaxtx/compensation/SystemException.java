package javax.jaxtx.compensation;

/**
 * Thrown if an error occurs which is not met by another specific
 * exception.
 * 
 * @version 1.0
 *
 * $Id: SystemException.java,v 1.1 2004/06/22 10:07:59 nmcl Exp $
 */

public class SystemException extends Exception
{

    public SystemException ()
    {
	super();
    }

    public SystemException (String s)
    {
	super(s);
    }

}
