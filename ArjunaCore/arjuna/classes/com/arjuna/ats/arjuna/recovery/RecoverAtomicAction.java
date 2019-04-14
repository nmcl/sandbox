/*
 * Copyright (C) 1999-2001 by HP Bluestone Software, Inc. All rights Reserved.
 *
 * HP Arjuna Labs,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: RecoverAtomicAction.java,v 1.3 2003/08/11 15:04:15 nmcl Exp $
 */

package com.arjuna.ats.arjuna.recovery ;

import com.arjuna.ats.arjuna.AtomicAction ;
import com.arjuna.ats.arjuna.common.Uid ;
import com.arjuna.ats.arjuna.coordinator.ActionStatus ;

import com.arjuna.ats.arjuna.logging.tsLogger;
import com.arjuna.ats.arjuna.logging.FacilityCode;

import com.arjuna.common.util.logging.*;

/**
 * @message com.arjuna.ats.arjuna.recovery.RecoverAtomicAction_1 [com.arjuna.ats.arjuna.recovery.RecoverAtomicAction_1] - RecoverAtomicAction.replayPhase2 recovering {0} ActionStatus is {1}
 * @message com.arjuna.ats.arjuna.recovery.RecoverAtomicAction_2 [com.arjuna.ats.arjuna.recovery.RecoverAtomicAction_2] - RecoverAtomicAction.replayPhase2: Unexpected status: {0}
 * @message com.arjuna.ats.arjuna.recovery.RecoverAtomicAction_3 [com.arjuna.ats.arjuna.recovery.RecoverAtomicAction_3] - RecoverAtomicAction.replayPhase2( {0} )  finished 
 * @message com.arjuna.ats.arjuna.recovery.RecoverAtomicAction_4 [com.arjuna.ats.arjuna.recovery.RecoverAtomicAction_4] - RecoverAtomicAction: transaction not activated, unable to replay phase 2 commit
*/

public class RecoverAtomicAction extends AtomicAction
{
   /**
    * Re-creates/activates an AtomicAction for the specified
    * transaction Uid.
    */
   public RecoverAtomicAction ( Uid rcvUid, int theStatus )
   {
      super( rcvUid ) ;
      _theStatus = theStatus ;
      _activated = activate() ;
   }
   
   /**
    * Replays phase 2 of the commit protocol.
    */
   public void replayPhase2()
   {
       if (tsLogger.arjLoggerI18N.debugAllowed())
       {
	   tsLogger.arjLoggerI18N.debug(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					FacilityCode.FAC_CRASH_RECOVERY, 
					"com.arjuna.ats.arjuna.recovery.RecoverAtomicAction_1", 
					new Object[]{get_uid(), ActionStatus.stringForm(_theStatus)});
       }

       if ( _activated )
       {
	   if ( (_theStatus == ActionStatus.PREPARED) ||
		(_theStatus == ActionStatus.COMMITTING) ||
		(_theStatus == ActionStatus.COMMITTED) ||
		(_theStatus == ActionStatus.H_COMMIT) ||
		(_theStatus == ActionStatus.H_MIXED) ||
		(_theStatus == ActionStatus.H_HAZARD) )
	   {
	       super.phase2Commit( _reportHeuristics ) ;
	   }
	   else if ( (_theStatus == ActionStatus.ABORTED) ||
		     (_theStatus == ActionStatus.H_ROLLBACK) ||
		     (_theStatus == ActionStatus.ABORTING) ||
		     (_theStatus == ActionStatus.ABORT_ONLY) )
	   {
	       super.phase2Abort( _reportHeuristics ) ;
	   }
	   else
	   {
	       if (tsLogger.arjLoggerI18N.isWarnEnabled())
	       {
		   tsLogger.arjLoggerI18N.warn("com.arjuna.ats.arjuna.recovery.RecoverAtomicAction_2", 
					       new Object[]{ActionStatus.stringForm(_theStatus)});
	       }
	   }

	   if (tsLogger.arjLoggerI18N.debugAllowed())
	   {
	       tsLogger.arjLoggerI18N.debug(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					    FacilityCode.FAC_CRASH_RECOVERY, 
					    "com.arjuna.ats.arjuna.recovery.RecoverAtomicAction_3", 
					    new Object[]{get_uid()});
	   }
       }
       else
       {
	   tsLogger.arjLoggerI18N.warn("com.arjuna.ats.arjuna.recovery.RecoverAtomicAction_4");
       }
   }
   
   // Current transaction status 
   // (retrieved from the TransactionStatusManager)
   private int _theStatus ;

   // Flag to indicate that this transaction has been re-activated
   // successfully.
   private boolean _activated = false ;

   // whether heuristic reporting on phase 2 commit is enabled.
   private boolean _reportHeuristics = true ;


}





