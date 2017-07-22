#ifndef _elastic_
#define _elastic_

#include "resizable.h"

class Elastic : public Resizable
{
protected:
    Elastic();
};

class HElastic : public Elastic
{
public:
    HElastic();

    void resize(Coord, Coord);
};

class VElastic : public Elastic
{
public:
    VElastic();

    void resize(Coord, Coord);
};

#endif
