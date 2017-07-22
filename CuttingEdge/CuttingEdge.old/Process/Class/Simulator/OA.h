#ifndef OA_
#define OA_

#include "Common.h"
#include "ReplicatedObject.h"

class OA
{
public:
    OA(ReplicatedObject*, lockcode);
    ~OA();

    static boolean FindObj(ReplicatedObject*, OList*);

    ReplicatedObject* ObjRef;
    ObjectInfo data;
    lockcode Mode;
};

#endif

