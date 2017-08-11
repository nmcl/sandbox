@echo off

if "%JTSARJUNA_HOME%"=="" goto error

java -DPROPERTIES_FILE=%JTSARJUNA_HOME%\etc\JavaGandiva.properties -DOBJECTSTORE_DIR=%JTSARJUNA_HOME%\ObjectStore %1 %2 %3 %4 %5
goto end

:error

echo Environment variable JTSARJUNA_HOME is not set.

:end