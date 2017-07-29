#ifndef PROTOCOL_
#define PROTOCOL_

#define ALL -1
enum lock_code { unlocked, read, write };

class Protocol
{
protected:
    Protocol(long, long, double);

public:
    ~Protocol();

    long ReadQuorum;
    long WriteQuorum;
    double RecoveryTime;
    char* name;
};

#endif
