package com.hp.mwtests.ts.arjuna.recovery;

/*
 * Copyright (C) 1999-2001 by HP Bluestone Software, Inc. All rights Reserved.
 *
 * HP Arjuna Labs,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ActionTestServer.java,v 1.8 2003/06/19 10:51:20 nmcl Exp $
 */

import java.io.* ;
import java.net.* ;
import java.util.* ;

import com.arjuna.ats.arjuna.AtomicAction ;
import com.arjuna.ats.arjuna.coordinator.ActionStatus ;
import com.arjuna.ats.arjuna.coordinator.AddOutcome ;
import com.arjuna.ats.arjuna.coordinator.BasicAction ;
import com.arjuna.ats.arjuna.coordinator.TxControl ;
import com.arjuna.ats.arjuna.common.Uid ;
import com.arjuna.ats.arjuna.coordinator.ActionStatus ;
import com.arjuna.ats.arjuna.recovery.Service ;
import com.arjuna.ats.arjuna.recovery.ActionStatusService ;
import com.arjuna.ats.arjuna.utils.Utility ;
import com.arjuna.ats.internal.arjuna.recovery.Listener ;
import com.arjuna.ats.internal.arjuna.recovery.TransactionStatusConnector ;
import com.arjuna.ats.internal.arjuna.recovery.TransactionStatusManagerItem ;
import com.arjuna.mwlabs.testframework.unittest.Test;
import com.arjuna.mwlabs.testframework.unittest.LocalHarness;
import com.arjuna.ats.arjuna.objectstore.ObjectStore ;

public class ActionTestServer extends Test
{
   private static boolean test_setup()
   {
      boolean setupOk = false ;

      try
      {
         _tests_passed = 0 ;
         _tests_failed = 0 ;

         // needed to force TxControl static initialization
         ObjectStore os = TxControl.getStore() ;

         _transaction_1 = new AtomicAction() ;
         _transaction_2 = new AtomicAction() ;
         _transaction_3 = new AtomicAction() ;

         _test_tran_type_1 = _transaction_1.type() ;
         _test_tran_type_2 = _transaction_2.type() ;
         _test_tran_type_3 = _transaction_3.type() ;

         _test_uid_1 = _transaction_1.getSavingUid() ;
         _test_uid_2 = _transaction_2.getSavingUid() ;
         _test_uid_3 = _transaction_3.getSavingUid() ;

         _test_host = InetAddress.getLocalHost().getHostAddress() ;

         final int one_second = 1000 ;
         final int connection_tries = 10 ;
         Socket sok = null ;

         for ( int i = 0 ; i < connection_tries ; i++ )
         {
            try
            {
               sok = new Socket( _test_host, _test_port ) ;
               Thread.sleep( one_second ) ;
               if ( sok != null )
                  break ;
            }
            catch ( ConnectException ex )
            {
               if ( connection_tries == i + 1 )
               {
                  throw ex ;
               }
            }
         }

         // for sending transaction data to client
         _from_client = new BufferedReader( new InputStreamReader
                           ( sok.getInputStream() )) ;

         _to_client   = new PrintWriter( new OutputStreamWriter
                           ( sok.getOutputStream() )) ;

         // Send the process id to the client, so that it can setup
         // a TransactionStatusConnector to this process.
         _to_client.println( Utility.getProcessUid().toString() ) ;
         _to_client.println( Utility.intToHexString( Utility.getpid() )) ;
         _to_client.flush() ;

         setupOk = true ;
      }
      catch ( Exception ex )
      {
         System.err.println( "test_setup: Failed " + ex ) ;
      }

      return setupOk ;
   }

   public void run( String[] args )
   {
        if ( test_setup() )
        {
            try
            {
                test1() ;
                test2() ;
                test3() ;

                logInformation( _unit_test + "tests passed: " + _tests_passed +
                                    "  tests failed: " + _tests_failed );

                if ( _tests_failed > 0 )
                {
                    assertFailure();
                }
                else
                {
                    assertSuccess();
                }
            }
            catch ( Exception ex )
            {
                System.err.println( _unit_test + "FATAL EXCEPTION: " +
                                               "tests passed: " + _tests_passed +
                                               "  tests failed: " + _tests_failed ) ;
                assertFailure();
            }
        }
        else
        {
            logInformation("Test setup failed");
            assertFailure();
        }
   }

   private static void test1()
   {
      try
      {
         String test_uid  = _test_uid_1.toString() ;
         String test_type = _test_tran_type_1 ;
         String reply ;

         // send transaction id to client
         _to_client.println( test_type ) ;
         _to_client.println( test_uid ) ;
         _to_client.flush() ;

         reply = _from_client.readLine() ;

         _transaction_1.begin() ;

         CrashAbstractRecordImpl cr1 =
            new CrashAbstractRecordImpl( CrashAbstractRecordImpl.CRASH_IN_COMMIT ) ;
         CrashAbstractRecordImpl cr2 =
            new CrashAbstractRecordImpl( CrashAbstractRecordImpl.CRASH_IN_COMMIT ) ;

         int addcr1 = _transaction_1.add( cr1 ) ;
         int addcr2 = _transaction_1.add( cr2 ) ;

         // send transaction id to client
         _to_client.println( test_type ) ;
         _to_client.println( test_uid ) ;
         _to_client.flush() ;

         reply = _from_client.readLine() ;

         _transaction_1.commit() ;

         // send transaction id to client
         _to_client.println( test_type ) ;
         _to_client.println( test_uid ) ;
         _to_client.flush() ;

         reply = _from_client.readLine() ;

         if ( ( addcr1 == AddOutcome.AR_ADDED ) &&
              ( addcr2 == AddOutcome.AR_ADDED ) )
         {
            System.out.println( _unit_test + "test1 passed" ) ;
            _tests_passed++ ;
         }
         else
         {
            System.out.println( _unit_test + "test1 failed" ) ;
            _tests_failed++ ;
         }
      }
      catch ( Exception ex )
      {
         System.err.println( _unit_test + " test1 " + ex ) ;
         _tests_failed++ ;
      }
   }

   private static void test2()
   {
      try
      {
         String test_uid  = _test_uid_2.toString() ;
         String test_type = _test_tran_type_2 ;
         String reply ;

         // send transaction id to client
         _to_client.println( test_type ) ;
         _to_client.println( test_uid ) ;
         _to_client.flush() ;

         reply = _from_client.readLine() ;

         _transaction_2.begin() ;

         CrashAbstractRecordImpl cr1 =
            new CrashAbstractRecordImpl( CrashAbstractRecordImpl.CRASH_IN_PREPARE ) ;
         CrashAbstractRecordImpl cr2 =
            new CrashAbstractRecordImpl( CrashAbstractRecordImpl.CRASH_IN_PREPARE ) ;

         int addcr1 = _transaction_2.add( cr1 ) ;
         int addcr2 = _transaction_2.add( cr2 ) ;

         // send transaction id to client
         _to_client.println( test_type ) ;
         _to_client.println( test_uid ) ;
         _to_client.flush() ;
         reply = _from_client.readLine() ;

         _transaction_2.commit() ;

         // send transaction id to client
         _to_client.println( test_type ) ;
         _to_client.println( test_uid ) ;
         _to_client.flush() ;
         reply = _from_client.readLine() ;

         if ( ( addcr1 == AddOutcome.AR_ADDED ) &&
              ( addcr2 == AddOutcome.AR_ADDED ) )
         {
            System.out.println( _unit_test + "test2 passed" ) ;
            _tests_passed++ ;
         }
         else
         {
            System.out.println( _unit_test + "test2 failed" ) ;
            _tests_failed++ ;
         }
      }
      catch ( Exception ex )
      {
         System.err.println( _unit_test + " test2 " + ex ) ;
         _tests_failed++ ;
      }
   }

   private static void test3()
   {
      try
      {
         String test_uid  = _test_uid_3.toString() ;
         String test_type = _test_tran_type_3 ;
         String reply ;

         // send transaction id to client
         _to_client.println( test_type ) ;
         _to_client.println( test_uid ) ;
         _to_client.flush() ;
         reply = _from_client.readLine() ;

         _transaction_3.begin() ;

         CrashAbstractRecordImpl cr1 =
            new CrashAbstractRecordImpl( CrashAbstractRecordImpl.NO_CRASH ) ;
         CrashAbstractRecordImpl cr2 =
            new CrashAbstractRecordImpl( CrashAbstractRecordImpl.NO_CRASH ) ;

         int addcr1 = _transaction_3.add( cr1 ) ;
         int addcr2 = _transaction_3.add( cr2 ) ;

         // send transaction id to client
         _to_client.println( test_type ) ;
         _to_client.println( test_uid ) ;
         _to_client.flush() ;
         reply = _from_client.readLine() ;

         _transaction_3.commit() ;

         // send transaction id to client
         _to_client.println( test_type ) ;
         _to_client.println( test_uid ) ;
         _to_client.flush() ;
         reply = _from_client.readLine() ;

         if ( ( addcr1 == AddOutcome.AR_ADDED ) &&
              ( addcr2 == AddOutcome.AR_ADDED ) )
         {
            System.out.println( _unit_test + "test3 passed" ) ;
            _tests_passed++ ;
         }
         else
         {
            System.out.println( _unit_test + "test3 failed" ) ;
            _tests_failed++ ;
         }
      }
      catch ( Exception ex )
      {
         System.err.println( _unit_test + " test3 " + ex ) ;
         _tests_failed++ ;
      }
   }

   public static void main(String[] args)
   {
       ActionTestServer test = new ActionTestServer();
       test.initialise( null, null, args, new LocalHarness() );
       test.runTest();
   }

   private static final String _unit_test = "com.hp.mwtests.ts.arjuna.recovery.ActionTestServer: " ;

   private static String _test_host ;
   private static final int _test_port = 4321 ;

   private static BufferedReader _from_client ;
   private static PrintWriter    _to_client ;

   private static AtomicAction _transaction_1 ;
   private static AtomicAction _transaction_2 ;
   private static AtomicAction _transaction_3 ;

   private static Uid _test_uid_1 ;
   private static Uid _test_uid_2 ;
   private static Uid _test_uid_3 ;

   private static String _test_tran_type_1 ;
   private static String _test_tran_type_2 ;
   private static String _test_tran_type_3 ;

   private static int _tests_passed = 0 ;
   private static int _tests_failed = 0 ;
}
