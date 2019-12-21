/**
 * The world within which the cells live.
 *
 * <x,y> coordinates for every Cell. <0,0> is the top left
 * hand corner.
 */

class World
{
    public static final int DEFAULT_WIDTH = 50;
    public static final int DEFAULT_DEPTH = 20;

    public World ()
    {
	this(DEFAULT_DEPTH, DEFAULT_WIDTH);
    }

    public World (int w, int d)
    {
	_x = w;
	_y = d;

	_theWorld = new Cell[_x][_y];

	for (int i = 0; i < _x; i++)
	{
	    for (int j = 0; j < _y; j++)
		_theWorld[i][j] = new Cell();
	}
    }

    public World (SeedWorld aWorld)
    {
	_x = aWorld.x();
	_y = aWorld.y();

	_theWorld = aWorld.getWorld();
    }
    
    /*
     * How to seed the world?
     *
     * - define a class that contains the world to use?
     * - define x/y coordinates on command line?
     * - a file with the data?
     * - graphically?
     */
    
    public void print ()
    {
	for (int i = 0; i < _x; i++)
	{
	    for (int j = 0; j < _y; j++)
	    {
		System.out.print(_theWorld[i][j]);
	    }

	    System.out.println();
	}
    }

    /**
     * The tick of the clock and the world moves on ...
     *
     * The board is assumed to be infinite in size. How do we simulate
     * that in a finite world?
     *
     * i) We could consider the board wrapping around to use the cells
     * on either side as neighbours.
     *
     * ii) We could keep expanding the world by adding new cells beyond
     * the current dimensions as they come alive, but we'd eventually
     * run out of memory.
     *
     * iii) We could assume all cells beyond the boundary are dead and
     * stay dead.
     *
     * Go with option (iii) for now.
     */
    
    public void tick ()
    {
	/**
	 * 1) Any live cell with two or three neighbours survives.
	 * 2) Any dead cell with three live neighbours becomes a live cell.
	 * 3) All other live cells die in the next generation.
	 * 4) Dead cells remain dead unless triggered into life by rule 2).
	 */

	Cell[][] nextGeneration = new Cell[_x][_y];
	
	for (int x = 0; x < _x; x++)
	{
	    for (int y = 0; y < _y; y++)
	    {
		Cell current = _theWorld[x][y];
		int liveNeighbours = 0;

		/*
		 * <_x -1, _y -1> <_x -1, _y> <_x -1, _y +1>
		 * <_x, _y -1> <_x, _y> <_x, _y +1>
		 * <_x +1, _y -1> <_x +1, _y> <_x +1, _y +1>
		 *
		 * -1,-1 -1,0 -1,1
		 *  0,-1  0,0  0,1
		 *  1,-1  1,0  1,1
		 */

		/*
		 * Option (i) wrapping ...
		 *
		 * int n1x = ((x-1 < 0) ? _x : x-1);
		 * int n1y = ((y-1 < 0) ? _y : y-1);
		 * int n2x = ((x-1 < 0) ? _x : x-1);
		 * int n2y = y;
		 * int n3x = ((x-1 < 0) ? _x : x-1);
		 * int n3y = ((y+1 == _x) ? 0 : y+1);
		 *
		 * int n4x = x;
		 * int n4y = ((y-1 < 0) ? _y : y-1);
		 * int n5x = x;
		 * int n5y = y;
		 * int n6x = x;
		 * int n6y = ((y+1 == _y) ? 0 : y+1);
		 *
		 * int n7x = ((x+1 == _x) ? 0 : x+1);
		 * int n7y = ((y-1 < 0) ? _y : y-1);
		 * int n8x = ((x+1 == _x) ? 0 : x+1);
		 * int n8y = y;
		 * int n9x = ((x+1 == _x) ? 0 : x+1);
		 * int n9y = ((y+1 == _x) ? 0 : y+1);
		 */

		/*
		 * Option (iii) dead cells beyond the grid ...
		 *
		 * Start by counting the number of live cells around us.
		 */

		int n1x = x-1;
		int n1y = y-1;

		if ((n1x >= 0) && (n1y >= 0))
		    liveNeighbours += _theWorld[n1x][n1y].isAlive();
		    
		int n2x = x-1;
		int n2y = y;

		if (n2x >= 0)
		    liveNeighbours += _theWorld[n2x][n2y].isAlive();
		
		int n3x = x-1;
		int n3y = y+1;

		if ((n3x >= 0) && (n3y < _y))
		    liveNeighbours += _theWorld[n3x][n3y].isAlive();
		
		int n4x = x;
		int n4y = y-1;

		if (n4y >=0)
		    liveNeighbours += _theWorld[n4x][n4y].isAlive();
		
		int n5x = x;
		int n5y = y;

		// nothing to do for ourself at this stage ...
		
		int n6x = x;
		int n6y = y+1;

		if (n6y < _y)
		    liveNeighbours += _theWorld[n6x][n6y].isAlive();

		int n7x = x+1;
		int n7y = y-1;

		if ((n7x < _x) && (n7y >= 0))
		    liveNeighbours += _theWorld[n7x][n7y].isAlive();
		
		int n8x = x+1;
		int n8y = y;

		if (n8x < _x)
		    liveNeighbours += _theWorld[n8x][n8y].isAlive();
		
		int n9x = x+1;
		int n9y = y+1;

		if ((n9x < _x) && (n9y < _y))
		    liveNeighbours += _theWorld[n9x][n9y].isAlive();

		if (current.isAlive() == 1)
		{
		    /**
		     * 1) Any live cell with two or three neighbours survives.
		     */

		    if ((liveNeighbours == 2) || (liveNeighbours == 3))
		    {
			// do nothing except get older

			nextGeneration[x][y] = new Cell(1, current.getAge());
		    }
		}
		else
		{
		    /**
		     * 2) Any dead cell with three live neighbours becomes a live cell.
		     */

		    if (liveNeighbours == 3)
		    {
			nextGeneration[x][y] = new Cell(1, 1);
		    }
		}

		/**
		 * 3) All other live cells die in the next generation.
		 * 4) Dead cells remain dead unless triggered into life by rule 2).
		 */

		if (nextGeneration[x][y] == null)
		{
		    nextGeneration[x][y] = new Cell();
		}
	    }
	}

	_theWorld = nextGeneration;
    }
    
    private int _x;
    private int _y;
    private Cell[][] _theWorld;

}
