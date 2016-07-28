echo "** Object Store Performance: Effect of Object Size"

uid=`OperActionsDriver -obj 100 -size 256 | grep Test | cut -d" " -f5`
OperActionsDriver -uid $uid
rm {IT}/ObjectStore/FragmentedStore/defaultStore/StateManager/LockManager/OperActions/*
rm {IT}/ObjectStore/FragmentedStore/defaultStore/StateManager/LockManager/PerformanceTest/*

uid=`OperActionsDriver -obj 100 -size 512 | grep Test | cut -d" " -f5`
OperActionsDriver -uid $uid
rm {IT}/ObjectStore/FragmentedStore/defaultStore/StateManager/LockManager/OperActions/*
rm {IT}/ObjectStore/FragmentedStore/defaultStore/StateManager/LockManager/PerformanceTest/*

uid=`OperActionsDriver -obj 100 -size 1024 | grep Test | cut -d" " -f5`
OperActionsDriver -uid $uid
rm {IT}/ObjectStore/FragmentedStore/defaultStore/StateManager/LockManager/OperActions/*
rm {IT}/ObjectStore/FragmentedStore/defaultStore/StateManager/LockManager/PerformanceTest/*

uid=`OperActionsDriver -obj 100 -size 2048 | grep Test | cut -d" " -f5`
OperActionsDriver -uid $uid
rm {IT}/ObjectStore/FragmentedStore/defaultStore/StateManager/LockManager/OperActions/*
rm {IT}/ObjectStore/FragmentedStore/defaultStore/StateManager/LockManager/PerformanceTest/*

uid=`OperActionsDriver -obj 100 -size 4096 | grep Test | cut -d" " -f5`
OperActionsDriver -uid $uid
rm {IT}/ObjectStore/FragmentedStore/defaultStore/StateManager/LockManager/OperActions/*
rm {IT}/ObjectStore/FragmentedStore/defaultStore/StateManager/LockManager/PerformanceTest/*

uid=`OperActionsDriver -obj 100 -size 8192 | grep Test | cut -d" " -f5`
OperActionsDriver -uid $uid
rm {IT}/ObjectStore/FragmentedStore/defaultStore/StateManager/LockManager/OperActions/*
rm {IT}/ObjectStore/FragmentedStore/defaultStore/StateManager/LockManager/PerformanceTest/*
