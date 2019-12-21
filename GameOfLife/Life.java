import java.lang.Thread;

public class Life
{
    public static void main (String[] args)
    {
	SeedWorld theSeed = null;
	boolean test = false;
	
	for (int i = 0; i < args.length; i++)
	{
	    if ("-help".equals(args[i]))
	    {
		System.out.println("Usage: -strategy <block> [-test] [-help]");
		return;
	    }

	    if ("-test".equals(args[i]))
		test = true;
	    
	    if ("-strategy".equals(args[i]))
	    {
		// throw exception if necessary
		
		if ("block".equals(args[i+1]))
		{
		    theSeed = new Block();
		}
	    }
	}

	if (theSeed == null)
	{
	    System.err.println("No strategy defined!");

	    return;
	}

	World theWorld = new World(theSeed);

	if (!test)
	{
	    for (;;)
	    {
		// loop forever

		System.out.print("\033[H\033[2J");
		System.out.flush();
	  
		theWorld.print();
		theWorld.tick();

		try
		{
		    Thread.currentThread().sleep(500);
		}
		catch (Throwable ex)
		{
		}
	    }
	}
	else
	{
	    System.out.print("\033[H\033[2J");
	    System.out.flush();
	    
	    theWorld.print();
	    theWorld.tick();
	}
    }
}
