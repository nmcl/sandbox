/**
 * The world within which the cells live.
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
	_width = w;
	_depth = d;

	_theWorld = new Cell[_width][_depth];

	for (int i = 0; i < _width; i++)
	{
	    for (int j = 0; j < _depth; j++)
		_theWorld[i][j] = new Cell();
	}
    }

    /*
     * How to seed the world?
     */
    
    public void print ()
    {
	for (int i = 0; i < _width; i++)
	{
	    for (int j = 0; j < _depth; j++)
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

	Cell[][] nextGeneration = new Cell[_width][_depth];
	
	for (int i = 0; i < _width; i++)
	{
	    for (int j = 0; j < _depth; j++)
	    {
		Cell current = _theWorld[i][j];
		int liveNeighbours
		
	    }
	}
    }
    
    private int _width;
    private int _depth;
    private Cell[][] _theWorld;

}
