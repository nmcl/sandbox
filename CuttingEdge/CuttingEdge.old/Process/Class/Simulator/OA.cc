#include "OA.h"
#include "List.h"

OA::OA(ReplicatedObject* o_ref, lockcode l_mode)
{
    ObjRef = o_ref;
    Mode = l_mode;
}

OA::~OA() {}

boolean OA::FindObj(ReplicatedObject *ORef, OList* Lst)
{
    OList *ptr;
    boolean res = false;

    if (!OList::Empty(Lst))
    {
	for (ptr = Lst; ((ptr != 0) && (ptr->ORef->ObjRef != ORef)); ptr = ptr->Next());
	
	if (ptr != 0)
	    return true;
	else
	    return false;
    }
    else
	return false;
}
