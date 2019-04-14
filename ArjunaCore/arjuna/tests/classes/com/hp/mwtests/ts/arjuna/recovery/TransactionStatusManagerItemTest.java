package com.hp.mwtests.ts.arjuna.recovery;

/*
 * Copyright (C) 1999-2001 by HP Bluestone Software, Inc. All rights Reserved.
 *
 * HP Arjuna Labs,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: TransactionStatusManagerItemTest.java,v 1.3 2003/06/19 10:51:20 nmcl Exp $
 */

import java.net.* ;

import com.arjuna.ats.arjuna.utils.Utility ;
import com.arjuna.ats.internal.arjuna.recovery.TransactionStatusManagerItem ;

public class TransactionStatusManagerItemTest
{
   public static void main( String[] args )
   {
      if ( test_setup() )
      {
         try
         {
            test1() ;
            test2() ;
            
            System.out.println( _unit_test + "tests passed: " + _tests_passed +
                                "  tests failed: " + _tests_failed ) ;
         }
         catch ( Exception ex )
         {
            System.err.println( "FATAL EXCEPTION: " +
                                           "tests passed: " + _tests_passed +
                                           "  tests failed: " + _tests_failed ) ;         
            
         }
      }
   }
   
   /**
    * Pre-test setup.
    */
   private static boolean test_setup()
   {
      boolean setupOk = false ;
      
      try
      {
         _tests_passed = 0 ;
         _tests_failed = 0 ;
         _test_host = InetAddress.getLocalHost().getHostAddress() ;
         setupOk = true ;
      }
      catch ( UnknownHostException ex )
      {
         System.err.println( "test_setup: UnknownHost " + ex ) ;
      }
      
      return setupOk ;
   }
   
   /**
    * Check that a transaction status manager item can be created
    * correctly.
    */
   private static void test1()
   {
      if ( TransactionStatusManagerItem.createAndSave( _test_port ) )
      {
         _tsmi = TransactionStatusManagerItem.get() ;

         if ( ( _tsmi.port() == _test_port ) &&
              ( _tsmi.host().equals( _test_host ) ) )
         {
            System.out.println( _unit_test + "test1: passed" ) ;
            _tests_passed++ ;
         }
         else
         {
            System.out.println( _unit_test + "test1: failed" ) ;
            _tests_failed++ ;
         }
      }
      else
      {
         System.out.println( _unit_test + "test1: failed" ) ;
         _tests_failed++ ;
      }
   }

   /**
    * Check that a transaction status manager item can be re-created
    * correctly.
    */
   private static void test2()
   {
      _tsmi = null ;
      _tsmi = TransactionStatusManagerItem.recreate( Utility.getProcessUid() ) ;

      if ( ( _tsmi.port() == _test_port ) &&
           ( _tsmi.host().equals( _test_host ) ) )
      {
         System.out.println( _unit_test + "test2: passed" ) ;
         _tests_passed++ ;
      }
      else
      {
         System.out.println( _unit_test + "test2: failed" ) ;
         _tests_failed++ ;
      }
   }
   
   private static final int _test_port = 4321 ;
   private static String _test_host = "" ;
   private static final String _unit_test = "com.hp.mwtests.ts.arjuna.recovery.TransactionStatusManagerItemTest: " ;
   private static TransactionStatusManagerItem _tsmi = null ;
   private static int _tests_passed = 0 ;
   private static int _tests_failed = 0 ;
}
