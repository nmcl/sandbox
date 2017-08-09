/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OTS_WrapperPostInit.java,v 1.2 1998/07/06 13:29:12 nmcl Exp $
 */

package com.arjuna.JavaCosTransactions.Filters.ObjectWrappers;

import com.arjuna.OrbCommon.*;
import org.omg.CosTransactions.*;
import com.arjuna.ArjunaOTS.*;

public class OTS_WrapperPostInit
{

public OTS_WrapperPostInit ()
    {
	ActionControlHelper.addClientObjectWrapperClass(ORBInterface.orb(), OTS_ActionControlWrapper.class);
	ArjunaSubtranAwareResourceHelper.addClientObjectWrapperClass(ORBInterface.orb(), OTS_ArjunaSubtranAwareResourceWrapper.class);
	ArjunaTransactionHelper.addClientObjectWrapperClass(ORBInterface.orb(), OTS_ArjunaTransactionWrapper.class);
	ControlHelper.addClientObjectWrapperClass(ORBInterface.orb(), OTS_ControlWrapper.class);
	CoordinatorHelper.addClientObjectWrapperClass(ORBInterface.orb(), OTS_CoordinatorWrapper.class);
	OTSAbstractRecordHelper.addClientObjectWrapperClass(ORBInterface.orb(), OTS_OTSAbstractRecordWrapper.class);
	RecoveryCoordinatorHelper.addClientObjectWrapperClass(ORBInterface.orb(), OTS_RecoveryCoordinatorWrapper.class);
	ResourceHelper.addClientObjectWrapperClass(ORBInterface.orb(), OTS_ResourceWrapper.class);
	SubtransactionAwareResourceHelper.addClientObjectWrapperClass(ORBInterface.orb(), OTS_SubtransactionAwareResourceWrapper.class);
	SynchronizationHelper.addClientObjectWrapperClass(ORBInterface.orb(), OTS_SynchronizationWrapper.class);
	TerminatorHelper.addClientObjectWrapperClass(ORBInterface.orb(), OTS_TerminatorWrapper.class);
	TransactionFactoryHelper.addClientObjectWrapperClass(ORBInterface.orb(), OTS_TransactionFactoryWrapper.class);
	UidCoordinatorHelper.addClientObjectWrapperClass(ORBInterface.orb(), OTS_UidCoordinatorWrapper.class);
    }

};
