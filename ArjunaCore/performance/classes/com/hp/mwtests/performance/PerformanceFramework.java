package com.hp.mwtests.performance;

import com.arjuna.orbportability.ORB;
import com.arjuna.orbportability.OA;
import com.arjuna.ats.jts.OTSManager;
import com.arjuna.mwlabs.testframework.utils.PerformanceLogger;
import com.arjuna.mwlabs.testframework.utils.XYData;
import com.arjuna.mwlabs.testframework.dtfweb.performance.frame.PerformanceGraphFrame;

import java.io.PrintWriter;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;

public class PerformanceFramework
{
    private final static int    TRANSACTIONS_PER_SECOND = 0,
                                TIME_TAKEN = 1,
                                NUMBER_OF_TRANSACTIONS = 2,
                                NUMBER_OF_THREADS = 3;

    private final static String[] DATA_TYPES = { "TRANSACTIONS_PER_SECOND", "TIME_TAKEN",
                                                 "NUMBER_OF_TRANSACTIONS", "NUMBER_OF_THREADS" };

    private final static int[]  DATA_TYPE_EQUIV_ENUM_VALUE = { TRANSACTIONS_PER_SECOND, TIME_TAKEN,
                                                               NUMBER_OF_TRANSACTIONS, NUMBER_OF_THREADS };

    private final static int    DEFAULT_NUMBER_OF_THREADS = 5;
    private final static int    DEFAULT_NUMBER_OF_ITERATIONS = 100;

    public final static String  ORB_INSTANCE_NAME = "com.hp.mwtests.performance.ORB";

    private final static String PERF_DATA_PREFIX = "-PERF-DATA";

    private int         _numberOfThreads = DEFAULT_NUMBER_OF_THREADS;
    private int         _numberOfIterations = DEFAULT_NUMBER_OF_ITERATIONS;
    private String[]    _args = null;
    private int         _xDataToLog = NUMBER_OF_THREADS;
    private int         _yDataToLog = TRANSACTIONS_PER_SECOND;
    private boolean     _displayGraph = false;
    private String      _csvFilename = null;

    public PerformanceFramework(String[] args)
    {
        _args = args;
    }

    public static int parseDataType( String type )
    {
        for (int count=0;count<DATA_TYPES.length;count++)
        {
            if ( DATA_TYPES[count].equals( type ) )
            {
                return DATA_TYPE_EQUIV_ENUM_VALUE[count];
            }
        }

        return -1;
    }

    public void setXData(int xDataToLog)
    {
        _xDataToLog = xDataToLog;
    }

    public void setYData(int yDataToLog)
    {
        _yDataToLog = yDataToLog;
    }

    public void setNumberOfThreads(int numberOfThreads)
    {
        _numberOfThreads = numberOfThreads;
    }

    public void setDisplayGraph(boolean value)
    {
        _displayGraph = value;
    }

    public void setNumberOfIterations(int numberOfIterations)
    {
        _numberOfIterations = numberOfIterations;
    }

    public void setCSVFilename(String filename)
    {
        _csvFilename = filename;
    }

    public int getNumberOfThreads()
    {
        return _numberOfThreads;
    }

    public int getNumberOfIterations()
    {
        return _numberOfIterations;
    }

    private void generateCSV(String csvFilename, PerformanceLogger perfLogger)
    {
        try
        {
            PrintWriter out = new PrintWriter( new FileOutputStream( csvFilename ) );

            out.println("Performance data '"+perfLogger.getDataName()+"'\n");

            ArrayList data = perfLogger.getData();

            out.println( perfLogger.getXAxisLabel() +","+ perfLogger.getYAxisLabel() );

            for (int count=0;count<data.size();count++)
            {
                XYData point = (XYData)data.get(count);

                out.println( point.getX() +","+ point.getY() );
            }

            out.close();
        }
        catch (IOException e)
        {
            System.err.println("Failed to create CSV file '"+csvFilename+"'");
        }
    }

    public boolean performTest( String classname, String[] configs )
    {
        boolean success = false;

        try
        {
            PerformanceLogger perfLogger = new PerformanceLogger( classname + PERF_DATA_PREFIX );

            perfLogger.setXAxisLabel("Number of Threads");
            perfLogger.setYAxisLabel("Tx/sec");

            for (int threadCount=1;threadCount<=getNumberOfThreads();threadCount++)
            {
                PerformanceTest perfTest = (PerformanceTest)Class.forName( classname ).newInstance();
                perfTest.setServiceConfigs( configs );
                perfTest.setParameters( _args );

                PerformanceTestRunnerThread[] pRunner = new PerformanceTestRunnerThread[threadCount];

                long startTimeForThreadSet= System.currentTimeMillis();

                for (int count=0;count<threadCount;count++)
                {
                    pRunner[count] = new PerformanceTestRunnerThread( perfTest, getNumberOfIterations() );

                    pRunner[count].start();
                }

                for (int count=0;count<threadCount;count++)
                {
                    pRunner[count].join();
                    success &= pRunner[count].success();
                }

                long endTimeForThreadSet= System.currentTimeMillis();
                long timeTaken = endTimeForThreadSet - startTimeForThreadSet;

                long numberOfTransactions = getNumberOfIterations()*threadCount;
                System.out.println( threadCount +" \t\t "+timeTaken+" \t\t"+(float) (numberOfTransactions/(timeTaken / 1000.0))+"\t\t"+numberOfTransactions);

                double dataX, dataY;

                switch ( _xDataToLog )
                {
                    case NUMBER_OF_TRANSACTIONS :
                    {
                        dataX = numberOfTransactions;
                        break;
                    }
                    case NUMBER_OF_THREADS :
                    default:
                    {
                        dataX = threadCount;
                        break;
                    }
                }

                switch ( _yDataToLog )
                {
                    case TIME_TAKEN :
                    {
                        dataY = timeTaken;
                        break;
                    }
                    case TRANSACTIONS_PER_SECOND :
                    default:
                    {
                        dataY = (numberOfTransactions/(timeTaken / 1000.0));
                        break;
                    }
                }

                perfLogger.addData( dataX, dataY );
            }

            perfLogger.output(System.err);

            if ( _csvFilename != null )
            {
                generateCSV(_csvFilename, perfLogger);
            }
            if ( _displayGraph )
            {
                new PerformanceGraphFrame(perfLogger);
            }
        }
        catch (Exception e)
        {
            e.printStackTrace(System.err);
            success = false;
        }

        return success;
    }
}
