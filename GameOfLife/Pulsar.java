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

	_theWorld[7][6] = new Cell(1, 1);
	_theWorld[7][7] = new Cell(1, 1);
	_theWorld[7][8] = new Cell(1, 1);

	_theWorld[7][12] = new Cell(1, 1);
	_theWorld[7][13] = new Cell(1, 1);
	_theWorld[7][14] = new Cell(1, 1);

	_theWorld[9][4] = new Cell(1, 1);
	_theWorld[10][4] = new Cell(1, 1);
	_theWorld[11][4] = new Cell(1, 1);

	_theWorld[9][9] = new Cell(1, 1);
	_theWorld[10][9] = new Cell(1, 1);
	_theWorld[11][9] = new Cell(1, 1);

	_theWorld[9][11] = new Cell(1, 1);
	_theWorld[10][11] = new Cell(1, 1);
	_theWorld[11][11] = new Cell(1, 1);

	_theWorld[9][16] = new Cell(1, 1);
	_theWorld[10][16] = new Cell(1, 1);
	_theWorld[11][16] = new Cell(1, 1);
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
