/*
 * Copyright (C) 1999-2001 by HP Bluestone Software, Inc. All rights Reserved.
 *
 * HP Arjuna Labs,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ActionStatusService.java,v 1.2 2003/08/11 14:07:38 nmcl Exp $
 */

package com.arjuna.ats.arjuna.recovery ;

import java.io.* ;
import java.util.* ;

import com.arjuna.ats.arjuna.AtomicAction ;
import com.arjuna.ats.arjuna.common.Uid ;
import com.arjuna.ats.arjuna.coordinator.ActionStatus ;
import com.arjuna.ats.arjuna.coordinator.BasicAction ;
import com.arjuna.ats.arjuna.coordinator.ActionManager;
import com.arjuna.ats.arjuna.coordinator.TxControl ;
import com.arjuna.ats.arjuna.objectstore.ObjectStore ;
import com.arjuna.ats.arjuna.state.InputObjectState ;
import com.arjuna.ats.arjuna.utils.Utility ;

import com.arjuna.ats.arjuna.logging.tsLogger;
import com.arjuna.ats.arjuna.logging.FacilityCode;

import com.arjuna.common.util.logging.*;

/**
 * @message com.arjuna.ats.arjuna.recovery.ActionStatusService_1 [com.arjuna.ats.arjuna.recovery.ActionStatusService_1] - transactionType: {0} uid: {1} \n Status is {2}
 * @message com.arjuna.ats.arjuna.recovery.ActionStatusService_2 [com.arjuna.ats.arjuna.recovery.ActionStatusService_2] - Other Exception:
 * @message com.arjuna.ats.arjuna.recovery.ActionStatusService_3 [com.arjuna.ats.arjuna.recovery.ActionStatusService_3] - Exception retrieving action status 
 * @message com.arjuna.ats.arjuna.recovery.ActionStatusService_4 [com.arjuna.ats.arjuna.recovery.ActionStatusService_4] - matching Uid {0} found
 * @message com.arjuna.ats.arjuna.recovery.ActionStatucService_5 [com.arjuna.ats.arjuna.recovery.ActionStatusService_5] - ActionStatusService: searching for uid: {0}
 * @message com.arjuna.ats.arjuna.recovery.ActionStatusService_6 [com.arjuna.ats.arjuna.recovery.ActionStatusService_6] - Exception when accessing transaction store {0}
 * @message com.arjuna.ats.arjuna.recovery.ActionStatusService_7 [com.arjuna.ats.arjuna.recovery.ActionStatusService_7] - Connection Lost to Recovery Manager
*/ 

public class ActionStatusService implements Service
{
   /**
    * Get a reference to the Transaction Store.
    */
   public ActionStatusService()
   {
      if ( _objectStore == null )
      {
         _objectStore = TxControl.getStore() ;
      }
   }

   /**
    * Retrieve the transaction status for the specified Uid and
    * if available transaction type.
    */
   public int getTransactionStatus( String transactionType, String strUid )
   {
      int action_status = ActionStatus.INVALID ;

      Uid tranUid = new Uid( strUid ) ;

      if ( transactionType.equals(new AtomicAction().type()) ||
           transactionType.equals(new BasicAction().type()) )
      {
         action_status = getActionStatus( tranUid, transactionType ) ;
      }
      else if ( transactionType.equals( "" ) )
      {
         action_status = getTranStatus( tranUid ) ;
      }

      return action_status ;
   }

   /**
    * Does the main work of reading in a uid and transaction type
    * from the recovery manager, retrieving the status of the 
    * transaction and sending it back to the Recovery Manager.
    */
   public void doWork( InputStream is, OutputStream os )
      throws IOException
   {
      BufferedReader in  = new BufferedReader ( new InputStreamReader(is) ) ;
      PrintWriter    out = new PrintWriter ( new OutputStreamWriter(os) ) ;

      try
      {
         // Send the process id to the recovery module so that it
         // can verify that it is talking to the right process.
         out.println ( Utility.intToHexString( Utility.getpid() )) ;
         out.flush() ;

         // recovery module returns either "OK" or "DEAD"
         String rmStatus = in.readLine() ;

         if ( rmStatus.equals( "OK" ) )
         {
            for ( ;; )
            {
               // read in a transaction type and its Uid sent by the
               // recovery module.
               String transactionType = in.readLine() ;
               String strUid = in.readLine() ;

               int status = getTransactionStatus( transactionType, strUid ) ;
               String strStatus = Integer.toString( status ) ;
               out.println( strStatus ) ;
               out.flush() ;

	       if (tsLogger.arjLoggerI18N.isInfoEnabled())
	       {
		   tsLogger.arjLoggerI18N.info("com.arjuna.ats.arjuna.recovery.ActionStatusService_1", 
						new Object[]{transactionType, strUid, strStatus});
	       }
	    }
         }
      }
      catch ( IOException ex )
      {
	  if (tsLogger.arjLoggerI18N.isWarnEnabled())
	      tsLogger.arjLoggerI18N.warn("com.arjuna.ats.arjuna.recovery.ActionStatusService_7");
      }
      catch ( Exception ex )
      {
	  if (tsLogger.arjLoggerI18N.isWarnEnabled()){
	      tsLogger.arjLoggerI18N.warn("com.arjuna.ats.arjuna.recovery.ActionStatusService_2", 
					  new Object[]{ex});
	  }
      }
   }
    
    /**
     * Check for transaction status in the local hash table,
     * if does not exist, then retrieve the status from the
     * Object Store.
     */
    private int getActionStatus( Uid tranUid, String transactionType )
    {
	int action_status = ActionStatus.INVALID ;
	
	try
	{
	    // check in local hash table
	    BasicAction basic_action = null ;
				
	    synchronized ( ActionManager.manager() )
	    {
		basic_action = (BasicAction)ActionManager.manager().get( tranUid ) ;
	    }

	    if ( basic_action != null)
	    {
		action_status = basic_action.status() ;
	    }
	    else
	    {
		/*
		 * If there is a persistent representation for this
		 * transaction, then return that status.
		 */
		action_status = getObjectStoreStatus( tranUid, transactionType ) ;
	    }
	}
	catch ( Exception ex )
	{
	    if (tsLogger.arjLoggerI18N.isWarnEnabled())
	    {
		tsLogger.arjLoggerI18N.warn("com.arjuna.ats.arjuna.recovery.ActionStatusService_3", 
					    new Object[]{ex});
	    }
	}
	
	return action_status ;
    }

   /**
    * Get transaction status for a transaction when the transactionType
    * is unknown.
    */

   private int getTranStatus( Uid tranUid )
   {
      int action_status = ActionStatus.INVALID ;

      try
      {
         BasicAction basic_action = null ;

	 synchronized ( ActionManager.manager() )
         {
            basic_action = (BasicAction)ActionManager.manager().get( tranUid ) ;
         }

         if ( basic_action != null)
         {
            action_status = basic_action.status() ;
         }
         else
         {
            /**
             * Run through the object store and try and find the matching id.
             */
            action_status = getOsStatus( tranUid ) ;
         }
      }
      catch ( Exception ex )
      {
	  if (tsLogger.arjLoggerI18N.isWarnEnabled())
	  {
	      tsLogger.arjLoggerI18N.warn("com.arjuna.ats.arjuna.recovery.ActionStatusService_3", 
					  new Object[]{ex});
	  }
      }

      return action_status ;
   }

   /**
    * Obtains the status for the specified transaction Uid when
    * the transaction type is unknown.
    */

   private int getOsStatus( Uid tranUid )
   {
      int action_status = ActionStatus.INVALID ;
      
      Vector matchingUidVector = new Vector() ;
      Vector matchingUidTypeVector = new Vector() ;
      
      try
      {
         InputObjectState types = new InputObjectState() ;

         // find all types
         if ( _objectStore.allTypes(types) )
         {
            String theTypeName = null ;

            try
            {
               boolean endOfList = false ;

               while ( !endOfList )
               {
                  // extract a type
                  theTypeName = types.unpackString() ;

                  if ( theTypeName.compareTo("") == 0 )
                  {
                     endOfList = true ;
                  }
                  else
                  {
                     InputObjectState uids = new InputObjectState() ;

                     try
                     {
                        boolean endOfUids = false ;

                        if ( _objectStore.allObjUids( theTypeName, uids ) )
                        {
                           Uid theUid = new Uid( Uid.nullUid() ) ;
                           
                           while ( !endOfUids )
                           {
                              // extract a uid
                              theUid.unpack( uids ) ;

                              if (theUid.equals( Uid.nullUid() ))
                              {
                                 endOfUids = true ;
                              }
                              else if ( theUid.equals( tranUid ) )
                              {
                                 // add to vector
                                 matchingUidVector.addElement( tranUid ) ;
                                 matchingUidTypeVector.addElement( theTypeName ) ;
                                 if (tsLogger.arjLoggerI18N.isInfoEnabled())
				 {
				     tsLogger.arjLoggerI18N.info("com.arjuna.ats.arjuna.recovery.ActionStatusService_4",new Object[]{tranUid});
				 }
			      }
                           }
                        }
                     }
                     catch ( Exception ex )
                     {
			 if (tsLogger.arjLoggerI18N.isWarnEnabled())
			 {
			     tsLogger.arjLoggerI18N.warn("com.arjuna.ats.arjuna.recovery.ActionStatusService_5", new Object[]{ex});
			 }
                     }
                  }
               }
            }
            catch ( IOException ex )
            {
		if (tsLogger.arjLoggerI18N.isWarnEnabled())
		{
		    tsLogger.arjLoggerI18N.warn("com.arjuna.ats.arjuna.recovery.ActionStatusService_5", new Object[]{ex});
		}
            }
            catch ( Exception ex )
            {
		if (tsLogger.arjLoggerI18N.isWarnEnabled())
		{
		    tsLogger.arjLoggerI18N.warn("com.arjuna.ats.arjuna.recovery.ActionStatusService_5", new Object[]{ex});
		}
            }
         }
      }
      catch ( Exception ex )
      {
	  if (tsLogger.arjLoggerI18N.isWarnEnabled())
	  {
	      tsLogger.arjLoggerI18N.warn("com.arjuna.ats.arjuna.recovery.ActionStatusService_5", new Object[]{ex});
	  }
      }

      int uidVectorSize = matchingUidVector.size() ;
      int first_index = 0 ;

      if ( uidVectorSize == 0 )
      {
         // no state means aborted because of presumed abort rules
         action_status = ActionStatus.ABORTED ;
      }
      else if ( uidVectorSize == 1 )
      {
         Uid uid = (Uid)matchingUidVector.get( first_index ) ;
         String typeName = (String)matchingUidTypeVector.get( first_index ) ;
         
         action_status = getObjectStoreStatus( uid, typeName ) ;
      }

      else if ( uidVectorSize > 1 )
      {
         // find root of hierarchy
         Uid rootUid = (Uid)matchingUidVector.get( first_index ) ;
         String rootTypeName = (String)matchingUidTypeVector.get( first_index ) ;

         for ( int index = first_index+1 ; index < uidVectorSize ; index++ )
         {
            String typeName = (String)matchingUidTypeVector.get( index ) ;
            if ( typeName.length() < rootTypeName.length() )
            {
               rootTypeName = typeName ;
               rootUid = (Uid)matchingUidVector.get( index ) ;
            }
         }
         
         action_status = getObjectStoreStatus( rootUid, rootTypeName ) ;
      }

      return action_status ;
   }

   /**
    * Retrieve the status of the transaction from the object store.
    */
   private int getObjectStoreStatus( Uid tranUid, String transactionType )
   {
      int action_status = ActionStatus.INVALID ;

      try
      {
         int osState = _objectStore.currentState( tranUid, transactionType ) ;

         switch ( osState )
         {
	 case ObjectStore.OS_COMMITTED :
	     action_status = ActionStatus.COMMITTED ;
	     break ;
	 case ObjectStore.OS_UNKNOWN:
	     action_status = ActionStatus.ABORTED;  // no state means aborted because of presumed abort rules
	     break;
	 case ObjectStore.OS_UNCOMMITTED        :
	 case ObjectStore.OS_HIDDEN             :
	 case ObjectStore.OS_COMMITTED_HIDDEN   :
	 case ObjectStore.OS_UNCOMMITTED_HIDDEN :
	     action_status = ActionStatus.PREPARED ;
	     break ;
         }
      }
      catch ( Exception ex )
      {
	  if (tsLogger.arjLoggerI18N.isWarnEnabled())
	  {
	      tsLogger.arjLoggerI18N.warn("com.arjuna.ats.arjuna.recovery.ActionStatusService_6", new Object[]{ex});
	  }
      }

      return action_status ;
   }

   /**
    * Reference to transaction object store.
    */

   private static ObjectStore _objectStore = null ;

}










