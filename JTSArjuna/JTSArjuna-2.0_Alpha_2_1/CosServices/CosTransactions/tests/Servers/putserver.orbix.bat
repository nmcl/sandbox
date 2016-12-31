#!/bin/sh

if test "'x$JTSARJUNA_HOME'" = "'x'"
then
    echo Environment variable JTSARJUNA_HOME is not set.
else
    putit.bat -j $1 " -DOBJECTSTORE_DIR=$JTSARJUNA_HOME/ObjectStore $2" -jdk2 -classpath \""$CLASSPATH"\"
fi

