echo "** Basic Action and CC Performance: Nested Read/Write"
echo "** Create objects for nested action performance tests **"
uid=`OperActionsDriver -obj 100 -size 1024  | grep Test | cut -d" " -f5`

echo "** Basic Action and CC Performance: Nested Read/Write Commit"

OperActionsDriver -obj 1 -num 1000 -oper BRE -uid ${uid} -nest
OperActionsDriver -obj 5 -num 1000 -oper BRE -uid ${uid} -nest
OperActionsDriver -obj 25 -num 1000 -oper BRE -uid ${uid} -nest
OperActionsDriver -obj 1 -num 1000 -oper BWE -uid ${uid} -nest
OperActionsDriver -obj 5 -num 1000 -oper BWE -uid ${uid} -nest
OperActionsDriver -obj 25 -num 1000 -oper BWE -uid ${uid} -nest

echo "** Basic Action and CC Performance: Nested Read/Write Abort"

OperActionsDriver -obj 1 -num 1000 -nest -oper BRA -uid ${uid}
OperActionsDriver -obj 5 -num 1000 -nest -oper BRA -uid ${uid}
OperActionsDriver -obj 25 -num 1000 -nest -oper BRA -uid ${uid}
OperActionsDriver -obj 1 -num 1000 -nest -oper BWA -uid ${uid}
OperActionsDriver -obj 5 -num 1000 -nest -oper BWA -uid ${uid}
OperActionsDriver -obj 25 -num 1000 -nest -oper BWA -uid ${uid}

rm {IT}/ObjectStore/FragmentedStore/defaultStore/StateManager/LockManager/OperActions/*
rm {IT}/ObjectStore/FragmentedStore/defaultStore/StateManager/LockManager/PerformanceTest/*
