/**
 * A cell is either alive or dead.
 */

class Cell
{
    public Cell ()
    {
	_alive = false;
    }

    public Cell (boolean alive)
    {
	_alive = alive;
    }

    public boolean isAlive ()
    {
	return _alive;
    }

    public boolean kill ()
    {
	_alive = false;

	return _alive;
    }

    public boolean regenerate ()
    {
	_alive = true;

	return _alive;
    }

    public String toString ()
    {
	return ((_alive) ? "@" : "-");
    }
    
    private boolean _alive;
}
