@echo off

if "%JTSARJUNA_HOME%"=="" goto error

java -DORBservices=com.arjuna.CosTransactions.Filters.Interposition.vbroker -DOBJECTSTORE_DIR=%JTSARJUNA_HOME%\ObjectStore InterpositionClient
goto end

:error

echo Environment variable JTSARJUNA_HOME is not set.

:end
