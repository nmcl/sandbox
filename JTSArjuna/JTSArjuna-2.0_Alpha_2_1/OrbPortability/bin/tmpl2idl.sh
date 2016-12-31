case $1 in
"jdk")	ln -s $2.tmpl $2.cc
		gcc -E -P -DWIN32 -DHASHHASH_CONCAT -DSUNSJDK -DDRAFT3 -DBROKEN_WRONG_TRANSACTION -DORB_OTS_SUPPORT -DINCLUDE_DIRECTIVE=#include $3 $4 $5 $6 $7 $2.cc > $2.idl
		rm $2.cc
		;;
"dais")	ln -s $2.tmpl $2.cc
		gcc -E -P -DWIN32 -DHASHHASH_CONCAT -DDAISJ2 -DDRAFT3 -DORB_OTS_SUPPORT -DINCLUDE_DIRECTIVE=#include $3 $4 $5 $6 $7 $2.cc > $2.idl
		rm $2.cc
			;;
"orbacus")	ln -s $2.tmpl $2.cc
			gcc -E -P -DWIN32 -DSUPPORT_IDL_PREFIX -DHASHHASH_CONCAT -DOMNIBROKER -DDRAFT3 -DORB_OTS_SUPPORT -DINCLUDE_DIRECTIVE=#include $3 $4 $5 $6 $7 $2.cc > $2.idl
			rm $2.cc	
			;;
"vbroker")	ln -s $2.tmpl $2.cc
			gcc -E -P -DNO_PRAGMA_SUPPORT -DWIN32 -DNO_ANONYMOUS_SEQUENCES -DHASHHASH_CONCAT -DDRAFT3 -DVISIBROKER -DORB_OTS_SUPPORT -DDBROKEN_WRONG_TRANSACTION -DINCLUDE_DIRECTIVE=#include $3 $4 $5 $6 $7 $2.cc > $2.idl
			rm $2.cc
			;;
*)			echo "Usage: tmpl2idl <jdk|dais|orbacus|vbroker> <template name> <cpp flags>"
			exit
			;;
esac
