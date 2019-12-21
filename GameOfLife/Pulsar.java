public class Pulsar implements SeedWorld
{
    /**
     * This Pulsar moves with a repetition rate of 3.
     */
    
    public Pulsar ()
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

	/*
	 * First set.
	 */
	
	_theWorld[5][6] = new Cell(1, 1);
	_theWorld[5][7] = new Cell(1, 1);
	_theWorld[5][8] = new Cell(1, 1);

	_theWorld[5][12] = new Cell(1, 1);
	_theWorld[5][13] = new Cell(1, 1);
	_theWorld[5][14] = new Cell(1, 1);

	_theWorld[7][4] = new Cell(1, 1);
	_theWorld[8][4] = new Cell(1, 1);
	_theWorld[9][4] = new Cell(1, 1);

	_theWorld[7][9] = new Cell(1, 1);
	_theWorld[8][9] = new Cell(1, 1);
	_theWorld[9][9] = new Cell(1, 1);

	_theWorld[7][11] = new Cell(1, 1);
	_theWorld[8][11] = new Cell(1, 1);
	_theWorld[9][11] = new Cell(1, 1);

	_theWorld[7][16] = new Cell(1, 1);
	_theWorld[8][16] = new Cell(1, 1);
	_theWorld[9][16] = new Cell(1, 1);

	_theWorld[10][6] = new Cell(1, 1);
	_theWorld[10][7] = new Cell(1, 1);
	_theWorld[10][8] = new Cell(1, 1);

	_theWorld[10][12] = new Cell(1, 1);
	_theWorld[10][13] = new Cell(1, 1);
	_theWorld[10][14] = new Cell(1, 1);

	/*
	 * Second set.
	 */
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
    private final int _x = 20;
    private final int _y = 20;
}
