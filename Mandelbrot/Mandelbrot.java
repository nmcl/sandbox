public class Mandelbrot
{
    private static String[] values = "0123456789`¬!£$%^&*()_+={}[]:@~;'#<>?,./|qwertyuioplkjhgfdsazxcvbnmQWERTYUIOPLKJHGFDSAZXCVBNM".split("");

    private static void print (String[][] world, int height, int width)
    {
	System.out.print("\033[H\033[2J");
	System.out.flush();

        for (int row = 0; row < height; row++)
	{
            for (int col = 0; col < width; col++)
	    {
		System.out.print(world[col][row]);
	    }

	    System.out.println();
	}
    }
    
    public static void main (String[] args)
    {
	int width = 100;
	int height = 50;
	int maxIters = values.length;
	
	String[][] world = new String[width][height];

        for (int row = 0; row < height; row++)
	{
            for (int col = 0; col < width; col++)
	    {
                double c_re = (col - width/2)*4.0/width;
                double c_im = (row - height/2)*4.0/width;
                double x = 0, y = 0;
                int iters = 0;
		
                while (x*x+y*y < 4 && iters < maxIters)
		{
                    double x_new = x*x-y*y+c_re;
		    
                    y = 2*x*y+c_im;
                    x = x_new;
		    
                    iters++;
                }
		
                if (iters == maxIters)
		    world[col][row] = "-";
                else
		    world[col][row] = values[iters];
            }
        }

	print(world, height, width);
    }
}
