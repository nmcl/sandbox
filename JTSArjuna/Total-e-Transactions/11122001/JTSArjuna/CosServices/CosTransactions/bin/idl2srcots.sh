if test "$JTSARJUNA_SRC" = ""
then
	echo "You must set JTSARJUNA_SRC to point to the source root."
	exit
fi

case $1 in
"jdk")	idl2src.sh $1 $2 -I$JTSARJUNA_SRC/JavaCosServices/JavaCosTransactions/include $3 $4 $5 $6 $7
		;;
"dais")	idl2src.sh $1 $2 -I$JTSARJUNA_SRC/JavaCosServices/JavaCosTransactions/include $3 $4 $5 $6 $7
		;;
"orbacus")	idl2src.sh $1 $2 -I$JTSARJUNA_SRC/JavaCosServices/JavaCosTransactions/include $3 $4 $5 $6 $7
			;;
"vbroker")	idl2src.sh $1 $2 -idl2package "::CosTransactions" org.omg.CosTransactions -idl2package "::ArjunaOTS" com.arjuna.ArjunaOTS -I$JTSARJUNA_SRC/JavaCosServices/JavaCosTransactions/include $3 $4 $5 $6 $7
			;;
*)			echo "Usage: idl2srcots <jdk|dais|orbacus|vbroker> <idl name> <idl flags>"
			exit
			;;
esac
