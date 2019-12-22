public class Gosper implements SeedWorld
{
    /**
     * Gosper's Glider Gun.
     */
    
    public Gosper ()
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

	_theWorld[6][2] = new Cell(1, 1);
	_theWorld[6][3] = new Cell(1, 1);
	_theWorld[7][2] = new Cell(1, 1);
	_theWorld[7][3] = new Cell(1, 1);

	_theWorld[4][14] = new Cell(1, 1);
	_theWorld[4][15] = new Cell(1, 1);
	_theWorld[5][13] = new Cell(1, 1);
	_theWorld[5][17] = new Cell(1, 1);
	_theWorld[6][12] = new Cell(1, 1);
	_theWorld[7][12] = new Cell(1, 1);
	_theWorld[8][12] = new Cell(1, 1);
	_theWorld[6][18] = new Cell(1, 1);
	_theWorld[7][18] = new Cell(1, 1);
	_theWorld[8][18] = new Cell(1, 1);
	_theWorld[7][19] = new Cell(1, 1);
	_theWorld[7][16] = new Cell(1, 1);
	_theWorld[9][13] = new Cell(1, 1);
	_theWorld[9][17] = new Cell(1, 1);
	_theWorld[10][14] = new Cell(1, 1);
	_theWorld[10][15] = new Cell(1, 1);

	_theWorld[4][22] = new Cell(1, 1);
	_theWorld[4][23] = new Cell(1, 1);
	_theWorld[5][22] = new Cell(1, 1);
	_theWorld[5][23] = new Cell(1, 1);
	_theWorld[6][22] = new Cell(1, 1);
	_theWorld[6][23] = new Cell(1, 1);
	_theWorld[3][24] = new Cell(1, 1);
	_theWorld[7][24] = new Cell(1, 1);
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
