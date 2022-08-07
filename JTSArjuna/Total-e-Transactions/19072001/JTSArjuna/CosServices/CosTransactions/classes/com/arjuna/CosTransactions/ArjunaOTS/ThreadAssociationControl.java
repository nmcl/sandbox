/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ThreadAssociationControl.java,v 1.1.6.1.26.1.4.2 2001/01/04 12:32:17 nmcl Exp $
 */

package com.arjuna.CosTransactions.ArjunaOTS;

import org.omg.CosTransactions.*;
import com.arjuna.CosTransactions.OTS_ControlWrapper;
import java.util.*;

/**
 * Used to process thread associations.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ThreadAssociationControl.java,v 1.1.6.1.26.1.4.2 2001/01/04 12:32:17 nmcl Exp $
 * @since JTS 2.0.
 */

public class ThreadAssociationControl
{

public final static void updateAssociation (OTS_ControlWrapper tx, int reason)
    {
	ThreadAssociations.updateAssociation(tx, reason);
    }

};
