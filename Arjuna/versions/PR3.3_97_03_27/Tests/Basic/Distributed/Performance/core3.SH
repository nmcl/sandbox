echo "** Basic Action and CC Performance: Remote Top Level Read/Write"
echo "** Create objects for top-level action performance tests **"
uid=`OperActionsDriver -obj 25 -size 1024  | grep Test | cut -d" " -f5`

echo "** Basic Action and CC Performance: Remote Top Level Read/Write Commit"

OperActionsDriver -obj 1 -num 1000 -oper BRE -uid ${uid} -top
OperActionsDriver -obj 5 -num 1000 -oper BRE -uid ${uid} -top
OperActionsDriver -obj 1 -num 1000 -oper BWE -uid ${uid} -top
OperActionsDriver -obj 5 -num 1000 -oper BWE -uid ${uid} -top

echo "** Basic Action and CC Performance: Remote Top Level Read/Write Abort"

OperActionsDriver -obj 1 -num 1000 -top -oper BRA -uid ${uid}
OperActionsDriver -obj 5 -num 1000 -top -oper BRA -uid ${uid}
OperActionsDriver -obj 1 -num 1000 -top -oper BWA -uid ${uid}
OperActionsDriver -obj 5 -num 1000 -top -oper BWA -uid ${uid}

rm {IT}/ObjectStore/FragmentedStore/defaultStore/StateManager/LockManager/OperActions/*
rm {IT}/ObjectStore/FragmentedStore/defaultStore/StateManager/LockManager/PerformanceTest/*
