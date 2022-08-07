@echo off

if "%JTSARJUNA_HOME%"=="" goto error

java -DORBservices=com.arjuna.CosTransactions.Filters.Context.vbroker -DOBJECTSTORE_DIR=%JTSARJUNA_HOME%\ObjectStore ImplicitClient
goto end

:error

echo Environment variable JTSARJUNA_HOME is not set.

:end
