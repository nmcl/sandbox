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

    public void tick ()
    {
	/*
	 * Assume the world wraps top, bottom and sides.
	 *
	 * 1) Any live cell with two or three neighbours survives.
	 * 2) Any dead cell with three live neighbours becomes a live cell.
	 * 3) All other live cells die in the next generation.
	 * 4) Dead cells remain dead unless triggered into life by rule 2).
	 */

	Cell[][] nextGeneration = new Cell[_x][_y];
	
	for (int i = 0; i < _x; i++)
	{
	    for (int j = 0; j < _y; j++)
	    {
		Cell current = _theWorld[i][j];
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
	    }
	}
    }
    
    private int _x;
    private int _y;
    private Cell[][] _theWorld;

}
