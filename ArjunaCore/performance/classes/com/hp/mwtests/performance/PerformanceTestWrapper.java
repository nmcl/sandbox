package com.hp.mwtests.performance;

import com.arjuna.mwlabs.testframework.unittest.Test;
import com.arjuna.mwlabs.testframework.unittest.LocalHarness;
import com.arjuna.orbportability.ORB;
import com.arjuna.orbportability.OA;
import com.arjuna.ats.jts.OTSManager;

import java.util.ArrayList;

public class PerformanceTestWrapper extends Test
{
    private final static String ADD_SERVICE_NAME = "-service";
    private final static String DISPLAY_GRAPH_PARAMETER = "-displaygraph";
    private final static String CSV_FILENAME_PARAMETER = "-csv";
    private final static String CLASSNAME_PARAMETER = "-classname";
    private final static String NUMBER_OF_THREADS_PARAMETER = "-threads";
    private final static String NUMBER_OF_ITERATIONS_PARAMETER = "-iterations";
    private final static String XDATA_PARAMETER = "-xdata";
    private final static String YDATA_PARAMETER = "-ydata";

    private final static int DEFAULT_NUMBER_OF_THREADS = 5;
    private final static int DEFAULT_NUMBER_OF_ITERATIONS = 100;

    /**
     * The main test method which must assert either a pass or a fail.
     */
    public void run(String[] args)
    {
        String classname = null;
        int numberOfThreads = DEFAULT_NUMBER_OF_THREADS;
        int numberOfIterations = DEFAULT_NUMBER_OF_ITERATIONS;
        boolean displayGraph = false;
        int xData = -1;
        int yData = -1;
        String csvFilename = null;
        ArrayList configList = new ArrayList();

        try
        {
            if ( args.length > 0 )
            {
                for (int count=0;count<args.length;count++)
                {
                    if ( args[count].equals( CLASSNAME_PARAMETER ) )
                    {
                        classname = args[count + 1];
                    }

                    if ( args[count].equals( CSV_FILENAME_PARAMETER ) )
                    {
                        csvFilename = args[count + 1];
                    }

                    if ( args[count].equals( DISPLAY_GRAPH_PARAMETER ) )
                    {
                        displayGraph = true;
                    }

                    if ( args[count].equals( NUMBER_OF_THREADS_PARAMETER ) )
                    {
                        numberOfThreads = Integer.parseInt( args[count + 1] );
                    }

                    if ( args[count].equals( NUMBER_OF_ITERATIONS_PARAMETER ) )
                    {
                        numberOfIterations = Integer.parseInt( args[count + 1] );
                    }

                    if ( args[count].equals( XDATA_PARAMETER ) )
                    {
                        xData = PerformanceFramework.parseDataType( args[count + 1] );

                        if ( xData == -1 )
                        {
                            System.err.println("Error - specified x-data parameter is invalid");
                            assertFailure();
                        }
                        else
                        {
                            logInformation("X-Data set to '"+args[count + 1]+"'");
                        }
                    }

                    if ( args[count].equals( YDATA_PARAMETER ) )
                    {
                        yData = PerformanceFramework.parseDataType( args[count + 1] );

                        if ( yData == -1 )
                        {
                            System.err.println("Error - specified y-data parameter is invalid");
                            assertFailure();
                        }
                        else
                        {
                            logInformation("Y-Data set to '"+args[count + 1]+"'");
                        }
                    }

                    if ( args[count].equals( ADD_SERVICE_NAME ) )
                    {
                        logInformation("Added Service config: '"+ args[count + 1] +"'");
                        configList.add( getService( args[count + 1] ) );
                    }
                }

                if ( classname != null )
                {
                    logInformation("           Classname: "+classname);
                    logInformation("   Number of Threads: "+numberOfThreads);
                    logInformation("Number of Iterations: "+numberOfIterations);

                    try
                    {
                        /**
                         * Retrieve ORB and OA references, intialise them
                         * and then set the OTSManager ORB and OA properties
                         */
                        ORB orb = ORB.getInstance( PerformanceFramework.ORB_INSTANCE_NAME );
                        OA oa = OA.getRootOA( orb );

                        orb.initORB(args, null);
                        oa.initOA(args);

                        OTSManager.setORB(orb);
                        OTSManager.setPOA(oa);
                    }
                    catch (Exception e)
                    {
                        e.printStackTrace(System.err);
                        assertFailure();
                    }

                    PerformanceFramework pf = new PerformanceFramework( args );

                    pf.setDisplayGraph(displayGraph);
                    pf.setCSVFilename(csvFilename);

                    if ( xData != -1 )
                    {
                        pf.setXData( xData );
                    }

                    if ( yData != -1 )
                    {
                        pf.setYData( yData );
                    }

                    pf.setNumberOfIterations(numberOfIterations);
                    pf.setNumberOfThreads(numberOfThreads);

                    String[] configs = new String[ configList.size() ];
                    configList.toArray(configs);

                    if ( pf.performTest( classname, configs ) )
                    {
                        assertFailure();
                    }
                    else
                    {
                        assertSuccess();
                    }
                }
                else
                {
                    logInformation("Parameter '"+CLASSNAME_PARAMETER+"' not specified");
                    assertFailure();
                }
            }
            else
            {
                logInformation("No parameters passed");
                assertFailure();
            }
        }
        catch (Exception e)
        {
            e.printStackTrace(System.err);
            assertFailure();
        }
    }

    public static void main(String[] args)
    {
        PerformanceTestWrapper ptw = new PerformanceTestWrapper();

        ptw.initialise(null, null, args, new LocalHarness());
        ptw.runTest();
    }
}
