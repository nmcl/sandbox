package javax.jaxtx.extensions.as;

import javax.jaxtx.Outcome;

import javax.jaxtx.exceptions.InvalidStateException;
import javax.jaxtx.exceptions.NoTransactionException;

public interface ActivityCoordinator
{

    public Outcome broadcast (String signal_set_name) throws SignalSetUnknownException, NoTransactionException, javax.jaxtx.exceptions.SystemException;
 
    void setCompletionSignalSet (String signal_set_name) throws NoTransactionException, SignalSetUnknownException, InvalidStateException, javax.jaxtx.exceptions.SystemException;
    
    String getCompletionSignalSet () throws NoTransactionException, javax.jaxtx.exceptions.SystemException;

}
