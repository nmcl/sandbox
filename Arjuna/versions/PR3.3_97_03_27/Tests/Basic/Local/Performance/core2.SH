echo "** Create objects for remainder of core performance tests **"
uid=`OperActionsDriver -obj 100 -size 1024  | grep Test | cut -d" " -f5`

echo "** Object Store Performance: Effect of Number of Objects"

OperActionsDriver -obj 1 -num 1000 -uid ${uid}
OperActionsDriver -obj 5 -num 1000 -uid ${uid}
OperActionsDriver -obj 25 -num 1000 -uid ${uid}
OperActionsDriver -obj 50 -num 1000 -uid ${uid}

echo "** Basic Action Performance: Top Level - Worst Case"
OperActionsDriver -obj 1 -num 1 -oper BE -uid ${uid}
OperActionsDriver -obj 5 -num 1 -oper BE -uid ${uid}
OperActionsDriver -obj 25 -num 1 -oper BE -uid ${uid}
OperActionsDriver -obj 1 -num 1 -oper BA -uid ${uid}
OperActionsDriver -obj 5 -num 1 -oper BA -uid ${uid}
OperActionsDriver -obj 25 -num 1 -oper BA -uid ${uid}

echo "** Basic Action Performance: Top Level"
OperActionsDriver -obj 1 -num 1000 -oper BE -uid ${uid}
OperActionsDriver -obj 5 -num 1000 -oper BE -uid ${uid}
OperActionsDriver -obj 25 -num 1000 -oper BE -uid ${uid}
OperActionsDriver -obj 1 -num 1000 -oper BA -uid ${uid}
OperActionsDriver -obj 5 -num 1000 -oper BA -uid ${uid}
OperActionsDriver -obj 25 -num 1000 -oper BA -uid ${uid}

echo "** Basic Action Performance: Nested - Worst Case"

OperActionsDriver -obj 1 -num 1 -oper BE -nest -uid ${uid}
OperActionsDriver -obj 5 -num 1 -oper BE -nest -uid ${uid}
OperActionsDriver -obj 25 -num 1 -oper BE -nest -uid ${uid}
OperActionsDriver -obj 1 -num 1 -oper BA -nest -uid ${uid}
OperActionsDriver -obj 5 -num 1 -oper BA -nest -uid ${uid}
OperActionsDriver -obj 25 -num 1 -oper BA -nest -uid ${uid}

echo "** Basic Action Performance: Nested"

OperActionsDriver -obj 1 -num 1000 -oper BE -nest -uid ${uid}
OperActionsDriver -obj 5 -num 1000 -oper BE -nest -uid ${uid}
OperActionsDriver -obj 25 -num 1000 -oper BE -nest -uid ${uid}
OperActionsDriver -obj 1 -num 1000 -oper BA -nest -uid ${uid}
OperActionsDriver -obj 5 -num 1000 -oper BA -nest -uid ${uid}
OperActionsDriver -obj 25 -num 1000 -oper BA -nest -uid ${uid}

rm {IT}/ObjectStore/FragmentedStore/defaultStore/StateManager/LockManager/OperActions/*
rm {IT}/ObjectStore/FragmentedStore/defaultStore/StateManager/LockManager/PerformanceTest/*
