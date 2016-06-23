
#ifndef OCIFC_H_
#define OCIFC_H_

#include <Common/Boolean.h>

#include "RBC_stub.h"

class cOCifc
{
public:
    cOCifc (Boolean&);
    virtual ~cOCifc ();

    Boolean init();
    int rbcStatus();
    int getStatus();
    void setStatus( int s);
    void crash();
 private:
    int status;
    // When tatsu generates cOCifc_stubclass.h, cRBC is replaced with LocalcRBC !
    cRBC* rbc;
};

#endif
