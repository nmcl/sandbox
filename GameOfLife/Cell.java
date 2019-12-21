/**
 * A cell is either alive or dead.
 */

class Cell
{
    public Cell ()
    {
	this(0, 0);
    }

    public Cell (int alive, int age)
    {
	_alive = alive;
	_age = age;
    }

    public int isAlive ()
    {
	return _alive;
    }

    public int getAge ()
    {
	return _age;
    }

    public void setAge (int value)
    {
	_age = value;
    }
    
    public void incrementAge ()
    {
	_age++;
    }
    
    public void kill ()
    {
	_alive = 0;
	_age = 0;
    }

    public void regenerate ()
    {
	_alive = 1;
	_age = 1;
    }

    public String toString ()
    {
	return ((_alive == 1) ? "@" : "-");
    }
    
    private int _alive;
    private int _age;
}
