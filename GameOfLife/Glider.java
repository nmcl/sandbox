public class Glider implements SeedWorld
{
    /**
     * A Glider moves across the screen.
     */
    
    public Glider ()
    {
	_theWorld = new Cell[_y][_x];

	/*
	 * Initialise the world;
	 */
	
	for (int i = 0; i < _y; i++)
	{
	    for (int j = 0; j < _x; j++)
		_theWorld[i][j] = new Cell();
	}

	/*
	_theWorld[2][4] = new Cell(1, 1);
	_theWorld[3][4] = new Cell(1, 1);
	_theWorld[4][4] = new Cell(1, 1);

	_theWorld[4][3] = new Cell(1, 1);

	_theWorld[3][2] = new Cell(1, 1);
	*/

	_theWorld[3][3] = new Cell(1, 1);

	_theWorld[4][4] = new Cell(1, 1);

	_theWorld[5][2] = new Cell(1, 1);
	_theWorld[5][3] = new Cell(1, 1);
	_theWorld[5][4] = new Cell(1, 1);
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
    private final int _x = 50;
    private final int _y = 20;
}
