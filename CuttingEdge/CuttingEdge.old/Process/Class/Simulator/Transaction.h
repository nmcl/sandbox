#ifndef Transaction_
#define Transaction_

#include <Process.h>

class OList;

class Transaction : public Process
{
    int ID, ObjectCount;
    double ComputeTime, CompletionTime;
    boolean Completed, LostQuorum;
    OList *p;
public:
    Transaction();
    ~Transaction();

    int TransactionID();
    void Body();
};

#endif
