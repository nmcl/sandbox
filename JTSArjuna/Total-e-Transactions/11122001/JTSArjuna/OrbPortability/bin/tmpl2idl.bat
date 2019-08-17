@echo off

if "%1"=="" goto error

if "%1"=="jdk" goto jdk

if "%1"=="dais" goto dais

if "%1"=="orbacus" goto orbacus

if "%1"=="vbroker" goto vbroker

goto error

:jdk

cl /EP /DSUNSJDK /DDRAFT3 /DORB_OTS_SUPPORT /DBROKEN_WRONG_TRANSACTION /DINCLUDE_DIRECTIVE##include %3 %2.tmpl > %2.idl

goto end

:dais

cl /EP /DDAIS /DDRAFT3 /DORB_OTS_SUPPORT /DINCLUDE_DIRECTIVE##include %3 %2.tmpl > %2.idl

goto end

:orbacus

cl /EP /DOMNIBROKER /DDRAFT3 /DORB_OTS_SUPPORT /DBROKEN_WRONG_TRANSACTION /DINCLUDE_DIRECTIVE##include %3 %2.tmpl > %2.idl

goto end

:vbroker

cl /EP /DVISIBROKER /DDRAFT3 /DORB_OTS_SUPPORT /DBROKEN_WRONG_TRANSACTION /DINCLUDE_DIRECTIVE##include %3 %2.tmpl > %2.idl

goto end

:error

echo "Usage: tmpl2idl <omniorb|orbacus|dais|vbroker> <template name> <cpp flags>"

:end