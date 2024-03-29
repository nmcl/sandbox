@echo off
rem
rem JTSArjuna-1.2 examples setup script
rem

if "%JTSARJUNA_HOME%"=="" goto home_error

echo Environment variable JTSARJUNA_HOME is set to "%JTSARJUNA_HOME%".
goto setup_classpath

:home_error

echo Environment variable JTSARJUNA_HOME is not set.
goto end

:setup_classpath

echo Adding Directory files to CLASSPATH.

set CLASSPATH=%CLASSPATH%;%JTSARJUNA_HOME%\lib\JavaArjunaLite.jar;%JTSARJUNA_HOME%\lib\JTS.jar;%JTSARJUNA_HOME%\lib\JavaGandiva.jar;%JTSARJUNA_HOME%\lib\JTSArjuna.jar;%JTSARJUNA_HOME%\lib\OrbCommon.jar;%JTSARJUNA_HOME%\examples\classes

echo Done.

echo
echo Ensure that the JDK and ORB are in the PATH and CLASSPATH.

:end
