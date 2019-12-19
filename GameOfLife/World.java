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

    private int _width;
    private int _depth;
    private Cell[][] _theWorld;

}
