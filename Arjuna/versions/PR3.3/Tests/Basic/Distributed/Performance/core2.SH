echo "** Create objects for remainder of core performance tests **"
uid=`OperActionsDriver -obj 25 -size 1024  | grep Test | cut -d" " -f5`

echo "** Basic Action Performance: Remote Top Level"

OperActionsDriver -obj 1 -num 1000 -oper BE -uid ${uid} -top
OperActionsDriver -obj 5 -num 1000 -oper BE -uid ${uid} -top
OperActionsDriver -obj 1 -num 1000 -oper BA -uid ${uid} -top
OperActionsDriver -obj 5 -num 1000 -oper BA -uid ${uid} -top

echo "** Basic Action Performance: Remote Nested"

OperActionsDriver -obj 1 -num 1000 -oper BE -nest -uid ${uid}
OperActionsDriver -obj 5 -num 1000 -oper BE -nest -uid ${uid}
OperActionsDriver -obj 1 -num 1000 -oper BA -nest -uid ${uid}
OperActionsDriver -obj 5 -num 1000 -oper BA -nest -uid ${uid}

rm {IT}/ObjectStore/FragmentedStore/defaultStore/StateManager/LockManager/OperActions/*
rm {IT}/ObjectStore/FragmentedStore/defaultStore/StateManager/LockManager/PerformanceTest/*
