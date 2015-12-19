public class PrizeB
{
    public static void main (String[] args)
    {
	final int incrementAmount = 50;
	int amount = 50;
	int runningTotal = amount;
	int[] months = new int[12];

	for (int i = 0; i < 12; i++)
	{
	    months[i] = amount;
	    amount += incrementAmount;
	    runningTotal += amount;
	    
	    System.out.println("Month "+(i+1)+" gives "+months[i]);
	}

	System.out.println("Total prize money: "+runningTotal);
    }
}