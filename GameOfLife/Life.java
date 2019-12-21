public class Life
{
    public static void main (String[] args)
    {
	World theWorld = new World();

	theWorld.print();

	theWorld.tick();

	theWorld.print();	
    }
}
