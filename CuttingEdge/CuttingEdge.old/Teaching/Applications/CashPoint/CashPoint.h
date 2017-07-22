#ifndef CASHPOINT_H_
#define CASHPOINT_H_

#include <Common/Boolean.h>

const long NumberOfAccounts = 10;

class CashPoint
{
public:
    CashPoint();

    long Balance(long);
    Boolean Withdraw(long, long);
    Boolean Insert(long, long);

private:
    Boolean CheckNumber(long);

    long Accounts[NumberOfAccounts];
};

#endif
