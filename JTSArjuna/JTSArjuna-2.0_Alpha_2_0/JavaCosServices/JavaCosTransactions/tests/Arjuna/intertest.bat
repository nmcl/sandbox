@echo off

if "%JTSARJUNA_HOME%"=="" goto error

<<<<<<< intertest.bat
java -DORBservices=com.arjuna.JavaCosTransactions.Filters.Interposition.vbroker -DOBJECTSTORE_DIR=%JTSARJUNA_HOME%\ObjectStore ImplicitArjunaClient
=======
java -DORBservices=com.arjuna.JavaCosTransactions.Filters.Interposition.vbroker -DOBJECTSTORE_DIR=%JTSARJUNA_HOME%\ObjectStore %1 %2 %3 %4 %5
>>>>>>> 1.5
goto end

:error

echo Environment variable JTSARJUNA_HOME is not set.

:end
