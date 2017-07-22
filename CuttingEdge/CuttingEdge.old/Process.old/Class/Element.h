#ifndef ELEMENT_
#define ELEMENT_

#include "Process.h"

class Element
{
public:
    Element(Process*);
    ~Element();

    Element* lson;
    Element* rson;
//    Element* son;
//    Element* brother;
    Process* Proc;
};

#endif
