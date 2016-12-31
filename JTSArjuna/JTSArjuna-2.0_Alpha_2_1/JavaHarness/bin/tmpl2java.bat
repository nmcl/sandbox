@echo off

if "%1"=="" goto error

if "%1"=="1.0.2" goto j102

if "%1"=="1.1.1" goto j111

if "%1"=="1.1.x" goto j11x

if "%1"=="1.2" goto j12

goto error

:j102

cl /EP /DJDK_1_0_2 /DJAR_PACKAGE_PROBLEM %3 %2.javatmpl > %2.java

goto end

:j111

cl /EP /DJDK_1_1 /DJAR_PACKAGE_PROBLEM %3 %2.javatmpl > %2.java

goto end

:j11x

cl /EP /DJDK_1_1 %3 %2.javatmpl > %2.java

goto end

:j12

cl /EP /DJDK_1_2 %3 %2.javatmpl > %2.java

goto end

:error

echo "Usage: tmpl2java <1.0.2|1.1.1|1.1.x|1.2> <cpp flags> <template name>"

:end