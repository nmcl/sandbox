public class Block implements SeedWorld
{
    /**
     * A Block is a 2x2 area of live cells
     * and should be static.
     */
    
    public Block ()
    {
	_theWorld = new Cell[_x][_y];

	/*
	 * Initialise the world;
	 */
	
	for (int i = 0; i < _x; i++)
	{
	    for (int j = 0; j < _y; j++)
		_theWorld[i][j] = new Cell();
	}

	_theWorld[2][2] = new Cell(1, 1);
	_theWorld[2][3] = new Cell(1, 1);
	_theWorld[3][2] = new Cell(1, 1);
	_theWorld[3][3] = new Cell(1, 1);
    }
    
    public Cell[][] getWorld ()
    {
	return _theWorld;
    }

    public int x ()
    {
	return _x;
    }
    
    public int y ()
    {
	return _y;
    }

    private Cell[][] _theWorld;
    private final int _x = 6;
    private final int _y = 6;
}
