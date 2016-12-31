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
"dais") tmpl2java.sh $2 $3 -I$JTSARJUNA_SRC/JavaOrbPortability/include -DDAISJ2 -DHAVE_POA -DCORBA_2_3 $4 $5 $6 $7 $8 $9 $10 $11 $12 $13 $14 $15 $16 $17 $18 $19 $20
		;;
"jdk")	tmpl2java.sh $2 $3 -I$JTSARJUNA_SRC/JavaOrbPortability/include -DSUNSJDK -DCORBA_2_2 -DHAVE_NAMESERVICE -DUSE_NAMESERVICE $4 $5 $6 $7 $8 $9 $10 $11 $12 $13 $14 $15 $16 $17 $18 $19 $20
		;;
"orbacus")	tmpl2java.sh $2 $3 -I$JTSARJUNA_SRC/JavaOrbPortability/include -DOMNIBROKER -DHAVE_BOA -DCORBA_2_2 -DHAVE_NAMESERVICE $4 $5 $6 $7 $8 $9 $10 $11 $12 $13 $14 $15 $16 $17 $18 $19 $20
		;;
"vbroker")	tmpl2java.sh $2 $3 -I$JTSARJUNA_SRC/JavaOrbPortability/include -DVISIBROKER -DHAVE_BOA -DNO_ANONYMOUS_SEQUENCES -DCORBA_2_1 -DVB_3 -DHAVE_NAMESERVICE -DHAVE_MARKER_NAME -DHAVE_FILTER $4 $5 $6 $7 $8 $9 $10 $11 $12 $13 $14 $15 $16 $17 $18 $19 $20
		;;
*)		echo "Usage: tmpl2javaorb <jdk|dais|orbacus|vbroker> <java version> <template name> <cpp flags>"
		exit
		;;
esac