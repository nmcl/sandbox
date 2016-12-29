/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ThreadAssociationControl.java,v 1.1.6.1 2000/05/22 10:23:53 nmcl Exp $
 */

package com.arjuna.CosTransactions.ArjunaOTS;

import org.omg.CosTransactions.*;
import com.arjuna.CosTransactions.OTS_Control;
import java.util.*;

public class ThreadAssociationControl
{

public final static void updateAssociation (Control tx, int reason)
    {
	ThreadAssociations.updateAssociation(tx, reason);
    }

public final static void updateAssociation (OTS_Control tx, int reason)
    {
	ThreadAssociations.updateAssociation(tx, reason);
    }

};
