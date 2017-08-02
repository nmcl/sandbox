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

import java.lang.Class;
import java.io.FileWriter;
import java.io.FileReader;
import java.io.LineNumberReader;
import java.io.File;

import java.io.IOException;
import java.lang.ClassNotFoundException;
import java.lang.InstantiationException;
import java.lang.IllegalAccessException;

public class StubEditor
{
    
public static void main (String args[])
    {
	String objectClassName = null;
	String componentName = null;
	
	for (int i = 0; i < args.length; i++)
	{
	    if (args[i].compareTo("-help") == 0)
	    {
		System.out.println("Usage: StubEditor -file <classname> -component <comp> [-help]");
		System.exit(0);
	    }
	    if (args[i].compareTo("-file") == 0)
		objectClassName = args[i+1];
	    if (args[i].compareTo("-component") == 0)
		componentName = args[i+1];
	}

	if (objectClassName == null)
	{
	    System.err.println("Error - no file name specified.");
	    System.exit(-1);
	}

	if (componentName == null)
	{
	    System.err.println("Error - no editor component specified.");
	    System.exit(-1);
	}

	Class componentClass = null;
	Object o = null;

	try
	{
	    componentClass = Class.forName(componentName);
	    o = componentClass.newInstance();
	}
	catch (ClassNotFoundException e1)
	{
	    System.err.println(e1);
	    System.exit(-1);
	}
	catch (InstantiationException e2)
	{
	    System.err.println(e2);
	    System.exit(-1);
	}
	catch (IllegalAccessException e3)
	{
	    System.err.println(e3);
	    System.exit(-1);
	}

	if (o instanceof EditorComponent)
	    component = (EditorComponent) o;
	else
	{
	    System.err.println("Error - class "+componentName+" is not an EditorComponent.");
	    System.exit(-1);
	}

	try
	{
	    FileWriter writer = new FileWriter(objectClassName + EditedExtension);
	    LineNumberReader reader = new LineNumberReader(new FileReader(objectClassName));

	    edit(reader, writer);

	    File old = new File(objectClassName + EditedExtension);
	    old.renameTo(new File(objectClassName));
	}
	catch (IOException e4)
	{
	    System.err.println(e4);
	    System.exit(-1);
	}
    }

private static void edit (LineNumberReader reader, FileWriter writer) throws IOException
    {
	boolean found = false;
	String line = null;
	
	while (!found)
	{
	    line = reader.readLine();

	    if (line.compareTo(component.tokenToFind()) == 0)
		found = true;
	    else
	    {
		writer.write(line, 0, line.length());
		writer.write('\n');
		writer.flush();
	    }
	}

	if (component.replace())
	    writer.write(component.tokenToUse(), 0, component.tokenToUse().length());
	else
	{
	    if (component.insertBefore())
	    {
		writer.write(component.tokenToUse(), 0, component.tokenToUse().length());
		writer.write('\n');		
		writer.write(line, 0, line.length());
	    }
	    else
	    {
		writer.write(line, 0, line.length());
		writer.write('\n');		
		writer.write(component.tokenToUse(), 0, component.tokenToUse().length());
	    }
	}

	writer.write('\n');	
	writer.flush();

	boolean finished = false;

	while (!finished)
	{
	    try
	    {
		line = reader.readLine();

		if (line == null)
		    finished = true;
		else
		{
		    writer.write(line, 0, line.length());
		    writer.write('\n');
		}
	    }
	    catch (IOException e)
	    {
		finished = true;
	    }
	}

	writer.flush();
    }

private static EditorComponent component;
    
private static final String EditedExtension = ".ed";
private static final int maxLine = 2048;

};
