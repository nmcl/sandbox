package javax.jaxtx.model.btp;

import javax.jaxtx.exceptions.ProtocolViolationException;

/**
 * A heuristic mixed has occurred: some participants confirmed and some
 * cancelled. Non-atomic outcomes.
 */

public class MixedException extends ProtocolViolationException
{

    public MixedException ()
    {
	super();
    }

    public MixedException (String s)
    {
	super(s);
    }

    public MixedException (String s, int errorcode)
    {
	super(s, errorcode);
    }

}
