/*
 * Copyright (C) 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: FileLocking.java,v 1.1 1998/11/18 09:01:30 nmcl Exp $
 */

import com.arjuna.JavaGandiva.Common.*;
import java.io.*;

import java.io.IOException;
import java.lang.InterruptedException;

/*
 * A simple test of file locking. Create 2 threads and have them
 * contend to exclusively lock the same file.
 */

class FileContender extends Thread
{

public FileContender (File file, int id, int lmode)
    {
	_theFile = file;
	_id = id;
	_mode = lmode;
    }

public void run ()
    {
	FileLock fileLock = new FileLock(_theFile);
	
	for (int i = 0; i < 100; i++)
	{
	    if (fileLock.lock(_mode, false))
	    {
		System.out.println("thread "+_id+" locked file.");

		Thread.yield();
		
		fileLock.unlock();

		System.out.println("thread "+_id+" unlocked file.");
		
		Thread.yield();
	    }
	    else
	    {
		System.out.println("thread "+_id+" could not lock file.");

		Thread.yield();
	    }
	}
    }

private File _theFile;
private int _id;
private int _mode;
    
};
    
public class FileLocking
{
    
public static void main (String[] args)
    {
	int lmode = FileLock.F_WRLCK;
	
	for (int i = 0; i < args.length; i++)
	{
	    if (args[i].compareTo("-read") == 0)
		lmode = FileLock.F_RDLCK;
	}
	
	/*
	 * Create the file to contend over.
	 */

	File theFile = new File("foobar");

	try
	{
	    DataOutputStream ofile = new DataOutputStream(new FileOutputStream(theFile));

	    ofile.writeInt(0);
	}
	catch (IOException e)
	{
	    System.out.println("An error occurred while creating file.");
	    System.exit(0);
	}

	/*
	 * Now create the threads.
	 */

	FileContender thread1 = new FileContender(theFile, 1, lmode);
	FileContender thread2 = new FileContender(theFile, 2, lmode);

	thread1.start();
	thread2.start();

	try
	{
	    thread1.join();
	    thread2.join();
	}
	catch (InterruptedException e)
	{
	}

	/*
	 * Now tidy up.
	 */

	theFile.delete();
    }
    
}
