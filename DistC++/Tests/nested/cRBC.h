

#ifndef CRBC_H_
#define CRBC_H_

#include <Common/Boolean.h>


/* @Remote @NoMarshall */
class cRBC
{
public:
    cRBC (Boolean&);
    virtual ~cRBC ();
    int getStatus();
    void setStatus( int s);
 private:
    int status;
};

#endif
