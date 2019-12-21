public class Blinker implements SeedWorld
{
    /**
     * A Blinker moves from vertical to horizontal
     * constantly.
     */
    
    public Blinker ()
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
	_theWorld[2][4] = new Cell(1, 1);
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
    private final int _x = 7;
    private final int _y = 7;
}
