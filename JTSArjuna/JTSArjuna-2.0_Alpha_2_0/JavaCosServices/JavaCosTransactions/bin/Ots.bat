@echo off

if "%JTSARJUNA_HOME%"=="" goto error

java -DPROPERTIES_FILE=%JTSARJUNA_HOME%\etc\Arjuna.properties -DOBJECTSTORE_DIR=%JTSARJUNA_HOME%\ObjectStore OTS_Server
goto end

:error

echo Environment variable JTSARJUNA_HOME is not set.

:end

