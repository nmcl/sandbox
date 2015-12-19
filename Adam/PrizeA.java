public class PrizeA
{
    public static void main (String[] args)
    {
	int amount = 1;
	int runningTotal = amount;
	int[] months = new int[12];

	for (int i = 0; i < 12; i++)
	{
	    months[i] = amount;
	    amount *= 2;
	    runningTotal += amount;
	    
	    System.out.println("Month "+(i+1)+" gives "+months[i]);
	}

	System.out.println("Total prize money: "+runningTotal);
    }
}