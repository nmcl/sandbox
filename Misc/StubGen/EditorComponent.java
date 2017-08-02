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

public interface EditorComponent
{

public String tokenToFind ();
public String tokenToUse ();

public boolean replace ();

public boolean insertBefore ();

public String componentName ();  // can return null

};
