#
# Must set JTSARJUNA_SRC environment variable to point to root of the
# source tree installation.
#
# First parameter is the version of java being used.
# Second parameter is the actual template name.
# Subsequent parameters are passed to cpp.
#

case $1 in
"1.0.2")	ln -s $2.javatmpl $2.cc
			gcc -E -C -P -DWIN32 -DJDK_1_0_2 -DHASHHASH_CONCAT -DJAR_PACKAGE_PROBLEM $3 $4 $5 $6 $7 $8 $9 $10 $11 $12 $13 $14 $15 $16 $17 $18 $19 $20 $21 $22 $23 $24 $25 $26 $27 $28 $29 $30 $2.cc > $2.java
			rm $2.cc
			;;
"1.1.1")	ln -s $2.javatmpl $2.cc
			gcc -E -C -P -DWIN32 -DJDK_1_1 -DHASHHASH_CONCAT -DJAR_PACKAGE_PROBLEM $3 $4 $5 $6 $7 $8 $9 $10 $11 $12 $13 $14 $15 $16 $17 $18 $19 $20 $21 $22 $23 $24 $25 $26 $27 $28 $29 $30 $2.cc > $2.java
			rm $2.cc
			;;
"1.1.x")	ln -s $2.javatmpl $2.cc
			gcc -E -C -P -DWIN32 -DJDK_1_1 -DHASHHASH_CONCAT $3 $4 $5 $6 $7 $8 $9 $10 $11 $12 $13 $14 $15 $16 $17 $18 $19 $20 $21 $22 $23 $24 $25 $26 $27 $28 $29 $30 $2.cc > $2.java
			rm $2.cc	
			;;
"1.2")		ln -s $2.javatmpl $2.cc
			gcc -E -C -P -DWIN32 -DJDK_1_2 -DHASHHASH_CONCAT $3 $4 $5 $6 $7 $8 $9 $10 $11 $12 $13 $14 $15 $16 $17 $18 $19 $20 $21 $22 $23 $24 $25 $26 $27 $28 $29 $30 $2.cc > $2.java
			rm $2.cc
			;;
*)			echo "Usage: tmpl2java <1.0.2|1.1.1|1.1.x|1.2> <template name> <cpp flags>"
			exit
			;;
esac
