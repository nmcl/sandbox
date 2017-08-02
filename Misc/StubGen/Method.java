/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id$
 */

import java.util.Vector;

public class Method
{

public Method ()
    {
    }

public Method (String r, String n, Vector p, Vector e)
    {
	rettype = r;
	name = n;
	params = p;
	excepts = e;
    }

public String rettype;
public String name;
public Vector params;
public Vector excepts;

};
