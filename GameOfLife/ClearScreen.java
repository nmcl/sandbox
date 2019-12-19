import java.io.IOException;

public class ClearScreen
{
    static void clrscr ()
    {
	System.out.print("\033\143");
    }
    
    public static void main (String[] args)
    {
	clrscr();
    }
}
