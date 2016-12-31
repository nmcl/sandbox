if "%JTSARJUNA_HOME%"=="" goto home_error

goto setup_classpath

:home_error

echo You must set the JTSARJUNA_HOME environment variable"

goto end

:setup_classpath

set CLASSPATH=%CLASSPATH%;%JTSARJUNA_HOME%\lib\JavaHarness.jar;%JTSARJUNA_HOME%\lib\ArjunaCommon.jar;%JTSARJUNA_HOME%\lib\ArjunaLite.jar;%JTSARJUNA_HOME%\lib\OrbCommon.jar;%JTSARJUNA_HOME%\lib\JTS.jar;%JTSARJUNA_HOME%\lib\JTSArjuna.jar;%JTSARJUNA_HOME%\lib\JDBCArjuna.jar;%JTSARJUNA_HOME%\lib\JDBC2Arjuna.jar;%JTSARJUNA_HOME%\lib\JTSArjunaInfo.jar;${CLASSPATH}

java -DPROPERTIES_FILE=%JTSARJUNA_HOME%\etc\Arjuna.properties %1 %2 %3 %4 %5 %6 %7 %8 %9 %10
