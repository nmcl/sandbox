package com.hp.mwtests.ts.arjuna.file;

/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: FileLocking.java,v 1.6 2003/06/19 10:51:18 nmcl Exp $
 */

import com.arjuna.ats.arjuna.common.*;
import com.arjuna.ats.arjuna.utils.*;
import com.arjuna.mwlabs.testframework.unittest.Test;

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

public class FileLocking extends Test
{

public void run(String[] args)
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
	    logInformation("An error occurred while creating file.");
            e.printStackTrace(System.err);
	    assertFailure();
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

	    logInformation("Test completed successfully.");

            assertSuccess();
	}
	catch (InterruptedException e)
	{
	    logInformation("Test did not complete successfully.");
            e.printStackTrace(System.err);
            assertFailure();
	}

	/*
	 * Now tidy up.
	 */

	theFile.delete();
    }

    public static void main(String[] args)
    {
        FileLocking fileTest = new FileLocking();

    	fileTest.initialise(null, null, args, new com.arjuna.mwlabs.testframework.unittest.LocalHarness());

    	fileTest.run(args);
    }
}
