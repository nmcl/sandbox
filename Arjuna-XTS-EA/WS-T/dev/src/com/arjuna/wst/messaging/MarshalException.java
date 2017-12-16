/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * MarshalException.java
 */

package com.arjuna.wst.messaging;

public class MarshalException extends Exception
{
    public MarshalException()
    {
        _cause = null;
    }

    public MarshalException(String message)
    {
        super(message);

        _cause = null;
    }

    public MarshalException(String message, Throwable cause)
    {
        super(message);

        _cause = cause;
    }

    public String toString()
    {
        if (_cause != null)
            return super.toString() + " caused by " + _cause.toString();
        else
            return super.toString();
    }

    private Throwable _cause;
}
