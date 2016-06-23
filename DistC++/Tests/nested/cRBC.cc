
#define CRBC_SERVER
#ifndef CRBC_H_
#  include "cRBC_stub.h"
#endif


cRBC::cRBC (Boolean& res)
{
  status = 21;


  res = TRUE;
}

cRBC::~cRBC ()
{
}

int cRBC::getStatus()
{
  return status;
}

void cRBC::setStatus(int s)
{
  status = s;
}
