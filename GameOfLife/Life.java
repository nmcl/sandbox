public class Life
{
    public static void main (String[] args)
    {
	SeedWorld theSeed = null;
	
	for (int i = 0; i < args.length; i++)
	{
	    if ("-help".equals(args[i]))
	    {
		System.out.println("Usage: -strategy <block> [-help]");
		return;
	    }
	    
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
    }
}
