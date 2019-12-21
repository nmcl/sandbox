import java.io.IOException;

public class ClearScreen
{
    static void clrscr ()
    {
	//	System.out.print("\033\143");

	System.out.print("\033[H\033[2J");  
	System.out.flush();  	
    }
    
    public static void main (String[] args)
    {
	clrscr();
    }
}
