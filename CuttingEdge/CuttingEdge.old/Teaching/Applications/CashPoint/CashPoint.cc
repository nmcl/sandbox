#include "CashPoint.h"

CashPoint::CashPoint()
{
    for (int i = 0; i < NumberOfAccounts; i++, Accounts[i] = 0);
}

Boolean CashPoint::CheckNumber (long account_number)
{
    if ((account_number < 0) || (account_number >= NumberOfAccounts))
	return FALSE;
    else
	return TRUE;
}

long CashPoint::Balance(long account_number)
{
    if (!CheckNumber(account_number))
	return -1;
    else
	return Accounts[account_number];
}

Boolean CashPoint::Withdraw (long account_number, long amount)
{
    if (!CheckNumber(account_number))
	return FALSE;

    if ((amount > Accounts[account_number]) || (amount < 0))
	return FALSE;

    Accounts[account_number] -= amount;
    return TRUE;
}

Boolean CashPoint::Insert (long account_number, long amount)
{
    if ((amount < 0) || (!CheckNumber(account_number)))
	return FALSE;

    Accounts[account_number] += amount;
    return TRUE;
}
