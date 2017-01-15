@echo off

if "%JTSARJUNA_HOME%"=="" goto error

<<<<<<< runtest.bat
java -DOBJECTSTORE_DIR=%JTSARJUNA_HOME%\ObjectStore %1 %2 %3 %4 %5
=======
java !-DOBJECTSTORE_DIR=%JTSARJUNA_HOME%\ObjectStore %1 %2 %3 %4 %5
>>>>>>> 1.3
goto end

:error

echo Environment variable JTSARJUNA_HOME is not set.

:end
