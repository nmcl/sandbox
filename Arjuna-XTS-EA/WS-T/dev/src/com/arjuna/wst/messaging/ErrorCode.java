/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * ErrorCode.java
 */

package com.arjuna.wst.messaging;

public class ErrorCode
{
    public String namespace = null;
    public String name      = null;

    public String toString()
    {
        return namespace + ":" + name;
    }
}
