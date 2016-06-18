package javax.jaxtx.model.btp;

import javax.jaxtx.model.btp.Qualifier;

/**
 * All votes from prepare are instances of this interface.
 */

public interface Vote
{
    /**
     * @return any qualifiers that may be associated with the vote. May be
     * null.
     */

    public Qualifier[] getQualifiers ();

}
