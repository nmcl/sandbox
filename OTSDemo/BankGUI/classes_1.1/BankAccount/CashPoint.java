package BankAccount;

import java.applet.*;
import java.awt.*;
import java.awt.image.*;
import java.awt.event.*;
import java.io.*;
import java.net.URL;

import java.lang.InterruptedException;

/*
 * Some private variables.
 */

class Bitmaps
{
    
public static Image cash_img[];
    
}

public class CashPoint extends Applet implements ActionListener
{
  
public CashPoint ()
    {
	theBank = new Bank();
	cashAmount = -1;
	numberOfDigits = 0;
	tracker = new MediaTracker(this);

	CashPoint.theCashPoint = this;
    }

public void init ()
    {
	setLayout(new BorderLayout());

	Panel mainPanel = new Panel();
	mainPanel.setLayout(new BorderLayout());

	Panel p1 = new Panel();
	p1.setLayout(new GridLayout(3, 3));

	pin = new TextField(10);
	pin.setEditable(false);	  // we will update this

	actionId = new TextField(128);
	
	amount = new TextField(10);
		
	p1.add(new Label("Pin No."));
	p1.add(pin);
		
	p1.add(new Label("Amount"));
	p1.add(amount);

	p1.add(new Label("Transaction Id"));
	p1.add(actionId);

	mainPanel.add("North", p1);
		
	Panel p2 = new Panel();
	p2.setLayout(new GridLayout(4, 3));

	for (int i = 0; i <= 9; i++)
	{
	    Button digit = new Button("" + (char)('0' + i));
	    digit.addActionListener(this);
	    p2.add(digit);
	}

	Button clear = new Button("Clear");
	clear.addActionListener(this);
	p2.add(clear);

	mainPanel.add("Center", p2);

	Panel p3 = new Panel();
	p3.setLayout(new GridLayout(1, 4));

	Button insert = new Button("insert");
	insert.addActionListener(this);
	
	Button inspect = new Button("inspect");
	inspect.addActionListener(this);
	
	Button withdraw = new Button("withdraw");
	withdraw.addActionListener(this);

	Button recover = new Button("recover");
	recover.addActionListener(this);

	p3.add(insert);
	p3.add(inspect);
	p3.add(withdraw);
	p3.add(recover);

	Panel p4 = new Panel();
	p4.setLayout(new BorderLayout());

	status = new TextField(20);
	status.setEditable(false);
	
	p4.add("North", p3);
	p4.add("South", status);

	mainPanel.add("South", p4);

	add("North", mainPanel);

	Bitmaps.cash_img = new Image[Constants.numberOfBitmaps];
	Bitmaps.cash_img[0] = getImage(this.getDocumentBase(), "images/cashpoint.gif");
	tracker.addImage(Bitmaps.cash_img[0], Constants.bitmapIndex);

	for (int i = 1; i < Constants.numberOfBitmaps; i++)
	{
	    String s = new String("images/money"+i+".gif");

	    /*
	     * Bug in Visual J++?
	     * Bitmaps.cash_img[i] = getImage(this.getDocumentBase(), "images/money"+i+".gif");
	     */

	    Bitmaps.cash_img[i] = getImage(this.getDocumentBase(), s);
	    tracker.addImage(Bitmaps.cash_img[i], Constants.bitmapIndex);
	}

	this.showStatus("Loading images");
	
	try
	{
	    tracker.waitForID(Constants.bitmapIndex);
	}
	catch (InterruptedException e)
	{
	}

	if (tracker.isErrorID(Constants.bitmapIndex))
	{
	    this.showStatus("Error loading images");
	    return;
	}

	resize(Constants.cashPointSizeX, Constants.cashPointSizeY);
    }

public void actionPerformed (ActionEvent evt)
    {
	int workOutcome = BankOutcome.NOTDONE;  // not used.
	String target = evt.getActionCommand();

	if (target.equals("insert"))
	    workOutcome = doWork(Opcodes.OP_INSERT);
	else
	{
	    if (target.equals("withdraw"))
		workOutcome = doWork(Opcodes.OP_WITHDRAW);
	    else
	    {
		if (target.equals("inspect"))
		    workOutcome = doWork(Opcodes.OP_INSPECT);
		else
		{
		    if (target.equals("recover"))
			workOutcome = recoverWork();
		    else
		    {
			/*
			 * Is this a "pin" button?
			 */

			char ch = target.charAt(0);
		    
			if (ch >= '0' && ch <= '9')
			{
			    if (numberOfDigits != Constants.pinSize)
			    {
				if (numberOfDigits == 0)
				    pin.setText(target);
				else
				    pin.setText(pin.getText() + target);
					
				numberOfDigits++;
			    }
			}
			else
			{
			    if (ch == 'C')
			    {
				amount.setText("");				
				pin.setText("");
				actionId.setText("");
				numberOfDigits = 0;
			    }
			}
		    }
		}
	    }
	}
    }

    /*
     * All we do here is draw the slot for the money.
     */

public void paint (Graphics g)
    {
	g.drawImage(Bitmaps.cash_img[0], Constants.cashPointx, Constants.cashPointy, this);
    }

public void displayActionUid (String id)
    {
	actionId.setText(id);

	repaint();
    }
    
private void animateCashpoint (Graphics g, int operation)
    {
	try
	{
	    if (operation == Opcodes.OP_INSPECT)
	    {
		status.setText("Balance is " + cashAmount);
		
		Thread.sleep(Constants.frameDuration*5);
		
		status.setText("");
		actionId.setText("");
	    }
	    else
	    {
		if (operation == Opcodes.OP_WITHDRAW)
		{
		    for (int i = 1; i < Constants.numberOfBitmaps; i++)
		    {
			g.drawImage(Bitmaps.cash_img[i], Constants.cashPointx, Constants.cashPointy, this);
			
			Thread.sleep(Constants.frameDuration);
		    }
		}
		else
		{
		    for (int i = Constants.numberOfBitmaps-1; i > 0; i--)
		    {
			g.drawImage(Bitmaps.cash_img[i], Constants.cashPointx, Constants.cashPointy, this);
			
			Thread.sleep(Constants.frameDuration);
		    }
		}
	    }
	}
	catch (InterruptedException e)
	{
	}
    }

    /*
     * Retrieve the input values from the user.
     */

private int doWork (int opcode)
    {
	if (numberOfDigits != 4)
	{
	    printStatus("No PIN number supplied.");
	    return BankOutcome.NOTDONE;
	}

	int workOutcome = BankOutcome.NOTDONE;
	String errorString = new String("An error has occurred.");
		
	int acc = Integer.valueOf(pin.getText()).intValue();
	
	if (opcode == Opcodes.OP_WITHDRAW || opcode == Opcodes.OP_INSERT)
	{
	    if (amount.getText().length() == 0)
	    {
		printStatus("No money supplied.");
		return BankOutcome.NOTDONE;
	    }

	    cashAmount = Integer.valueOf(amount.getText()).intValue();
	}

	switch (opcode)
	{
	case Opcodes.OP_INSPECT:
	    {
		Cash cash = new Cash(-1);
		    
		workOutcome = theBank.inspect(acc, cash);
		    
		if (workOutcome == BankOutcome.DONE)
		    cashAmount = cash.getValue();
		else
		    errorString = getErrorString(workOutcome);
	    }
	break;
	case Opcodes.OP_INSERT:
	    {
		workOutcome = theBank.insert(acc, cashAmount);
		
		if (workOutcome != BankOutcome.DONE)
		    errorString = getErrorString(workOutcome);
	    }
	break;
	case Opcodes.OP_WITHDRAW:
	    {
		workOutcome = theBank.withdraw(acc, cashAmount);
		    
		if (workOutcome != BankOutcome.DONE)
		    errorString = getErrorString(workOutcome);
	    }
	break;
	}

	/*
	 * The event if for us, so do the work, and then
	 * animate the result.
	 */

	Graphics g = getGraphics();
	
	if (workOutcome != BankOutcome.DONE)
	    printStatus(errorString);
	else
	    animateCashpoint(g, opcode);

	g.dispose();

	/*
	 * Clear the two text bars.
	 */

	pin.setText("");
	numberOfDigits = 0;
	amount.setText("");

	/*
	 * Only clear the Uid if the operation was successful.
	 * Give the user the chance to retry later. The Clear
	 * button will remove this Uid if required.
	 */
	
	if (workOutcome == BankOutcome.DONE)
	    actionId.setText("");

	repaint();

	return workOutcome;
   }

private int recoverWork ()
    {
	String tid = actionId.getText();

	if ((tid == null) || (tid.length() < 20))
	{
	    printStatus("No transaction id supplied.");
	    return BankOutcome.NOTDONE;
	}

	String errorString = null;
	int opcode = -1;
	
	try
	{
	    /*
	     * Return value indicates the type of operation
	     * which was being performed within the transaction.
	     * We use this to animate the cashpoint or display
	     * text.
	     */
	    
	    opcode = theBank.recover(tid);

	    if (opcode == ErrorStatus.NO_ACTION)
		errorString = "Transaction previously committed/aborted.";
	    else
		if (opcode == ErrorStatus.INVALID_UID)
		    errorString = "Invalid transaction identifier.";
	}
	catch (IOException e)
	{
	    errorString = e.toString();
	}

	/*
	 * The event is for us, so do the work, and then
	 * animate the result.
	 */

	Graphics g = getGraphics();

	if (errorString != null)
	    printStatus(errorString);
	else
	    animateCashpoint(g, opcode);

	g.dispose();

	/*
	 * Clear the two text bars.
	 */

	pin.setText("");
	numberOfDigits = 0;
	amount.setText("");
	actionId.setText("");

	repaint();

	if (errorString != null)
	    return BankOutcome.NOTDONE;
	else
	    return BankOutcome.DONE;
   }
    
private void printStatus (String message)
	{
	    status.setText(message);

	    try
	    {
		Thread.sleep(Constants.frameDuration*10);
	    }
	    catch (InterruptedException e)
	    {
	    }

	    status.setText("");
	}

private String getErrorString (int outcome)
    {
	switch (outcome)
	{
	case BankOutcome.INSUFFICIENT_FUNDS:
	    return new String("Insufficient funds.");
	case BankOutcome.ACCOUNT_ERROR:
	    return new String("The pin number is invalid.");
	case BankOutcome.LOCKED:
	    return new String("The account is currently locked.");
	case BankOutcome.NOTDONE:
	    return new String("The operation was NOTDONE");
	case BankOutcome.UNKNOWN:
	    return new String("The server may be unavailable.");
	default:
	    return new String("An unknown error has occurred.");	    
	}
    }

public static CashPoint theCashPoint = null;
    
private TextField pin;
private TextField amount;
private TextField status;
private TextField actionId;
private Bank theBank;
private int cashAmount;
private int numberOfDigits;
private MediaTracker tracker;
    
}

