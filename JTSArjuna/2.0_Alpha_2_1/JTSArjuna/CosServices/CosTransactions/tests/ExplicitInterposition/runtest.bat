@echo off

if "%JTSARJUNA_HOME%"=="" goto error

java -DOBJECTSTORE_DIR=%JTSARJUNA_HOME%\ObjectStore ExplicitInterClient
goto end

:error

echo Environment variable JTSARJUNA_HOME is not set.

:end
