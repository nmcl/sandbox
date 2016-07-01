#ifndef MONITOR_H_
#define MONITOR_H_

#include <sys/time.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/param.h>
#include <string.h>
#include <fstream>

using namespace std;

#ifndef COMMON_H_
#  include "Common.h"
#endif

class MachineList;


class Monitor
{
public:
    Monitor ();
    Monitor (const char*);
    ~Monitor ();

    Boolean monitor (int iterations = -1);

private:
    Boolean configure    ();
    Boolean probe        (int);
    Boolean transmitData () const;
    
    ofstream    output, error;
    char        *fileName, *hostName;
    MachineList *head;
    int         sock, msgSock, probeTime;
    Boolean     reduced;
};

#endif
