@echo off

if "%JTSARJUNA_HOME%"=="" goto error

java -DOBJECTSTORE_DIR=%JTSARJUNA_HOME%\ObjectStore %1 %2 %3 %4
goto end

:error

echo Environment variable JTSARJUNA_HOME is not set.

:end
