/*
 * Copyright (C) 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: PObject.java,v 1.1 1998/04/08 10:49:36 nmcl Exp $
 */

import java.io.*;

import com.arjuna.JTI.*;

public class PObject implements Serializable
{
    public PObject()
    {
    }

    public int getValue()
    {
        return _value; 
    }

    public void setValue(int value)
    {
        _value = value; 
    }

    private int _value = 0;
}
