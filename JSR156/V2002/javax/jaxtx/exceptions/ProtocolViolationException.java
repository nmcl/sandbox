package javax.jaxtx.exceptions;

/**
 * Thrown if the underlying protocol is violated in some way during
 * termination. For example, a two-phase commit protocol is necessarily
 * blocking to ensure consensus in the precence of failures. However,
 * this could mean that participants who have been prepared have to wait
 * forever if they don't get told the results of the transaction by the
 * (failed) coordinator. As such, heuristics were introduced to allow
 * a participant to make a unilateral decision about what to do. If this
 * decision goes against the coordinator's choice then the two-phase
 * protocol is violated.
 */

public class ProtocolViolationException extends JAXTXException
{

    public ProtocolViolationException ()
    {
	super();
    }

    public ProtocolViolationException (String s)
    {
	super(s);
    }

    public ProtocolViolationException (String s, int errorcode)
    {
	super(s, errorcode);
    }
    
}


