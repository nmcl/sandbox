Author: Richard A. Begg
Last Updated: 09:40 23rd August 2002

Using the HP-TS performance suite.

java com.hp.mwtests.performance.PerformanceTestWrapper <parameters>

    parameters:

        -classname <fully qualified name of class>
            Sets the classname of the test to run.
        -threads <maximum number of concurrent threads to run>
            Sets the number of concurrent threads to use.
        -iterations <number of iterations per thread>
            Sets the number of iterations to execute per thread.
        -xdata <data_type>  where data_type = NUMBER_OF_TRANSACTIONS, NUMBER_OF_THREADS
            Sets the data to log on the X axis.
        -ydata <data_type>  where data_type = TRANSACTIONS_PER_SECOND, TIME_TAKEN
            Sets the data to log on the Y axis.
        -csv <filename>
            Creates a CSV file containing the gathered performance data
        -displaygraph
            Once the performance test has completed a line graph of the data is displayed.
        -service <service name>
            Retrieves the value stored in the name service for the given service and makes
            it available to the performance test.  This is required for remote tests.


    example:

        java com.hp.mwtests.performance.PerformanceTestWrapper
                -xdata NUMBER_OF_THREADS
                -ydata TRANSACTIONS_PER_SECOND
                -threads 10
                -iterations 1000
                -classname com.hp.mwtests.performance.implementations.local.NestedTrx
                -displaygraph
                -csv test.csv
