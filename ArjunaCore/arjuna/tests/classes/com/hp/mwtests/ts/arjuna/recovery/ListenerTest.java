package com.hp.mwtests.ts.arjuna.recovery;

/*
 * Copyright (C) 1999-2001 by HP Bluestone Software, Inc. All rights Reserved.
 *
 * HP Arjuna Labs,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ListenerTest.java,v 1.7 2003/06/19 10:51:20 nmcl Exp $
 */

import java.io.* ;
import java.net.* ;
import java.util.* ;

import com.arjuna.ats.arjuna.recovery.Service ;
import com.arjuna.ats.internal.arjuna.recovery.Listener ;
import com.arjuna.mwlabs.testframework.unittest.Test;

class ListenerTestService implements Service
{
   public void doWork( InputStream is, OutputStream os )
      throws IOException
   {
      BufferedReader in  = new BufferedReader( new InputStreamReader(is) ) ;
      PrintWriter    out = new PrintWriter( new OutputStreamWriter(os) ) ;

      try
      {
         String test_string = in.readLine() ;
         out.println( test_string ) ;
         out.flush() ;
      }
      catch ( SocketException ex )
      {
         ; // socket closed
      }
      catch ( IOException ex )
      {
         System.err.println( "testService: failed" ) ;
      }
   }
}

public class ListenerTest extends Test
{
   public void run( String[] args )
   {
        if ( test_setup() )
        {
            try
            {
                test1() ;

                logInformation( _unit_test + "tests passed: " + _tests_passed +
                                "  tests failed: " + _tests_failed ); ;

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
                logInformation( _unit_test + "FATAL EXCEPTION: " +
                                           "tests passed: " + _tests_passed +
                                           "  tests failed: " + _tests_failed ) ;
                assertSuccess();
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

         _test_service = new ListenerTestService() ;

         _test_host = InetAddress.getLocalHost().getHostAddress() ;
         _test_service_socket = new ServerSocket( _test_port ) ;

         _test_socket = new Socket( _test_host, _test_port ) ;
         _from_test_service = new BufferedReader( new InputStreamReader
                                 ( _test_socket.getInputStream() )) ;
         _to_test_service   = new PrintWriter( new OutputStreamWriter
                                 ( _test_socket.getOutputStream() )) ;

         setupOk = true ;
      }
      catch ( Exception ex )
      {
         System.err.println( "test_setup: Failed " + ex ) ;
      }

      return setupOk ;
   }

   /**
    * Check that listener can be created and a simlple service ran.
    */
   private static void test1()
   {
      try
      {
         Listener testListener = new Listener( _test_service_socket,
                                               _test_service ) ;
         try
         {
            testListener.start() ;

            Thread.sleep( 1000 ) ; // allow testListener to startup.

            String testString = "testString" ;
            _to_test_service.println( testString ) ;
            _to_test_service.flush() ;
            String returnString = _from_test_service.readLine() ;

            if ( testString.equals( returnString ) )
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
         catch ( Exception ex )
         {
            System.err.println( _unit_test + " test1 " + ex ) ;
            _tests_failed++ ;
         }

         testListener.stopListener() ;
      }
      catch ( IOException ex )
      {
         System.err.println( _unit_test + " test1 " + ex ) ;
         _tests_failed++ ;
      }
   }

public static void main(String[] args)
    {
        ListenerTest test = new ListenerTest();
        test.initialise(null, null, args, new com.arjuna.mwlabs.testframework.unittest.LocalHarness());
        test.run(args);
    }

   private static final String _unit_test = "com.hp.mwtests.ts.arjuna.recovery.ListenerTest: " ;

   private static String    _test_host ;
   private static final int _test_port = 4321 ;

   private static ListenerTestService _test_service ;

   private static Socket       _test_socket ;
   private static ServerSocket _test_service_socket ;

   private static BufferedReader _from_test_service ;
   private static PrintWriter    _to_test_service ;

   private static int _tests_passed = 0 ;
   private static int _tests_failed = 0 ;
}
