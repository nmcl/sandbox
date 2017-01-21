@echo off

if "%1"=="" goto error

if "%1"=="omniorb" goto omniorb

if "%1"=="orbix" goto orbix

if "%1"=="rcp" goto rcp

if "%1"=="orbacus" goto orbacus

if "%1"=="vbroker" goto vbroker

goto error

:omniorb

omniidl2 -a -s .cc -h .h %2 %3

goto end

:orbix

idl -B -A -c C.cc -s S.cc -h .h %2 %3

goto end

:rcp

corba -t -lc -lc++ %2 %3

goto end

:orbacus

idl --c-suffix .cc --h-suffix .h %2 %3

goto end

:vbroker

idl2cpp -client_extC -server_extS -hdr_suffixh %2 %3

goto end

:error

echo "Usage: idl2src <omniorb|orbacus|orbix|rcp|vbroker> <idl> <idl flags>"

:end