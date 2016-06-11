/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Packable.java,v 1.1 1997/08/05 14:23:10 nmcl Exp $
 */

package JavaStubGenerator.Arjuna.simplerpc;

import JavaArjuna.Common.InputBuffer;
import JavaArjuna.Common.OutputBuffer;
import java.io.*;

import java.io.IOException;

public interface Packable
{

public void packInto (OutputBuffer buff) throws IOException;
public void unpackFrom (InputBuffer buff) throws IOException;

}
