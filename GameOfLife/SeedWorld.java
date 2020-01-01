/**
 * Implementations of this interface can be provided at runtime to
 * populate the world.
 */

public interface SeedWorld
{
    public Cell[][] getWorld (); // what's the world look like?

    // x and y size of world
    
    public int x ();
    public int y ();
}
