/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ByteBuffer.java,v 1.1 2003/06/19 10:50:25 nmcl Exp $
 */

package com.arjuna.ats.arjuna.state;

/**
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ByteBuffer.java,v 1.1 2003/06/19 10:50:25 nmcl Exp $
 * @since JTS 3.1.
 */

public interface ByteBuffer
{

    public byte [] getBytes ();
    
    public int size ();
    
}
