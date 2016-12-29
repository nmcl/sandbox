/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 * 
 * $Id: report.java,v 1.1 2000/03/13 17:59:39 nmcl Exp $
 */

import com.arjuna.CosTransactions.Info;
import java.io.*;

public class report
{

public static void main (String[] args)
    {
	Info i = new Info();
	
	System.out.println(i);
    }

};
