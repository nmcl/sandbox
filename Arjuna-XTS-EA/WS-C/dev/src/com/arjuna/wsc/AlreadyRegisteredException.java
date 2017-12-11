/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * InvalidCreateParametersException.java
 */

package com.arjuna.wsc;

public class AlreadyRegisteredException extends Exception
{
    public AlreadyRegisteredException()
    {
    }

    public AlreadyRegisteredException(String message)
    {
        super(message);
    }
}
