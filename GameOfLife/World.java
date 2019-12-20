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
		
		int n1x = ((x-1 < 0) ? _x : x-1);
		int n1y = ((y-1 < 0) ? _y : y-1);
		int n2x = ((x-1 < 0) ? _x : x-1);
		int n2y = y;
		int n3x = ((x-1 < 0) ? _x : x-1);
		int n3y = ((y+1 == _x) ? 0 : y+1);

		int n4x = x;
		int n4y = ((y-1 < 0) ? _y : y-1);
		int n5x = x;
		int n5y = y;
		int n6x = x;
		int n6y = ((y+1 == _y) ? 0 : y+1);

		int n7x = ((x+1 == _x) ? 0 : x+1);
		int n7y = ((y-1 < 0) ? _y : y-1);
		int n8x = ((x+1 == _x) ? 0 : x+1);
		int n8y = y;
		int n9x = ((x+1 == _x) ? 0 : x+1);
		int n9y = ((y+1 == _x) ? 0 : y+1);
	    }
	}
    }
    
    private int _x;
    private int _y;
    private Cell[][] _theWorld;

}
