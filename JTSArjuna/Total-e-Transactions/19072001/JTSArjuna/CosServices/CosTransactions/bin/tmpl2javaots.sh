#
# Must set JTSARJUNA_SRC environment variable to point to root of the
# source tree installation.
#
# First parameter is the ORB being used.
# Second parameter is the version of java being used.
# Third parameter is the actual template name.
# Subsequent parameters are passed to cpp.
#

if test "$JTSARJUNA_SRC" = ""
then
	echo "You must set JTSARJUNA_SRC to point to the source root."
	exit
fi

case $1 in
"dais") tmpl2javaorb.sh $1 $2 $3 -I$JTSARJUNA_SRC/JavaCosServices/JavaCosTransactions/include -DDRAFT3 -DORB_OTS_SUPPORT $4 $5 $6 $7 $8
		;;
"jdk")	tmpl2javaorb.sh $1 $2 $3 -I$JTSARJUNA_SRC/JavaCosServices/JavaCosTransactions/include -DDRAFT3 -DBROKEN_WRONG_TRANSACTION -DORB_OTS_SUPPORT $4 $5 $6 $7 $8
		;;
"orbacus")	tmpl2javaorb.sh $1 $2 $3 -I$JTSARJUNA_SRC/JavaCosServices/JavaCosTransactions/include -DOMNIBROKER -DCORBA_2_2 -DHAVE_NAMESERVICE $4 $5 $6 $7 $8
		;;
"vbroker")	tmpl2javaorb.sh $1 $2 $3 -I$JTSARJUNA_SRC/JavaCosServices/JavaCosTransactions/include -DVISIBROKER -DDRAFT3 -DBROKEN_WRONG_TRANSACTION -DORB_OTS_SUPPORT -DNO_ANONYMOUS_SEQUENCES -DCORBA_2_1 -DVB_3 -DHAVE_NAMESERVICE -DHAVE_MARKER_NAME -DHAVE_FILTER $4 $5 $6 $7 $8
		;;
*)		echo "Usage: tmpl2javaots <jdk|dais|orbacus|vbroker> <java version> <template name> <cpp flags>"
		exit
		;;
esac