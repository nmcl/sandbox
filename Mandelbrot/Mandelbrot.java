public class Mandelbrot
{
    /**
     * This represents the various ASCII characters we will
     * use to convert the iterations covered in to printable
     * values.
     *
     * In theory we could have to iterate over the function
     * forever to determine if it is part of the set or not.
     * Therefore, we place a limit on the number of iterations.
     * More iterations improves accuracy and if we were displaying
     * the set graphically we could us colours. Since we are using
     * ASCII characters that limit us to what we can print, we set
     * the number of iterations based on the nuumber of characters
     * from which we have to select, based on the number of iterations.
     */
    
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

    /**
     * Mandelbrot set history: https://www.scientificamerican.com/article/mandelbrot-set-1990-horgan/
     *
     * Or this: http://mathworld.wolfram.com/MandelbrotSet.html
     */
    
    public static void main (String[] args)
    {
	int width = 100;
	int height = 50;
	int maxIters = values.length;  // maximum iterations before we decide if value is part of set or not.
	
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
		
                if (iters == maxIters)  // not part of the set
		    world[col][row] = "-";
                else
		    world[col][row] = values[iters];  // part of the set
            }
        }

	print(world, height, width);
    }
}
