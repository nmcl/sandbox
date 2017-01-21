echo $3 $4 $5 $6 $7 $8 $9 $10 $2
case $1 in
"jdk")	idltojava -ftie $3 $4 $5 $6 $7 $8 $9 $10 $2
		;;
"dais")	stubgen -ljava -s $3 $4 $5 $6 $7 $8 $9 $10 $2
		;;
"orbacus")	jidl --auto-package --tie $3 $4 $5 $6 $7 $8 $9 $10 $2
			;;
"vbroker")	idl2java -replace -ir InterfaceRepository $3 $4 $5 $6 $7 $8 $9 $10 $2
			;;
*)			echo "Usage: idl2src <jdk|dais|orbacus|vbroker> <idl name> <idl flags>"
			exit
			;;
esac
