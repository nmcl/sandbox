package javax.jaxtx.model.btp;

/**
 * The inferior votes to confirm. It will typically
 * not have confirmed at this stage but will wait for the transaction
 * outcome. Failure to do so may result in heuristics (contradictions).
 */

public interface VoteConfirm extends Vote
{
    
}
