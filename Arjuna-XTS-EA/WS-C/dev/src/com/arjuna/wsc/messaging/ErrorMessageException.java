/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * ErrorMessageException.java
 */

package com.arjuna.wsc.messaging;

public class ErrorMessageException extends Exception
{
    public ErrorMessageException(ErrorMessage errorMessage)
    {
        _errorMessage = errorMessage;
    }

    public ErrorMessageException(String message, ErrorMessage errorMessage)
    {
        super(message);

        _errorMessage = errorMessage;
    }

    public ErrorMessage getErrorMessage()
    {
        return _errorMessage;
    }

    public String toString()
    {
        if (_errorMessage != null)
            return "ErrorMessageException: " + getMessage() + " [errorMessage=(" + _errorMessage.toString() + ")]";
        else
            return "ErrorMessageException: " + getMessage() + " [errorMessage=(null)]";
    }

    private ErrorMessage _errorMessage = null;
}
