#include "Common.h"
#include "List.h"
#include "Transaction.h"
#include "OA.h"

TList::TList(Transaction* body)
{
    next = 0;
    Xaction = body;
}

TList::~TList() {}

void TList::Clear(TList*& to_delete)
{
    TList* ptr = to_delete, *marker;

    do
    {
	marker = ptr;
	ptr = ptr->next;
	delete marker;
    } while (ptr != 0);

    to_delete = 0;
}

void TList::Add(TList*& list, Transaction* element)
{
    if (list == 0)
	list = new TList(element);
    else
    {
	TList* marker = list, *ptr;
	do
	{
	    ptr = marker;
	    marker = marker->next;
	} while (marker != 0);

	ptr->next = new TList(element);
    }
}

void TList::Delete(TList*& list, TList*& to_delete)
{
    if (to_delete == list)
	list = list->next;
    else
    {
	TList* marker = list;
	boolean found = false;
	do
	{
	    if (marker->next->Xaction->TransactionID() == to_delete->Xaction->TransactionID())
		found = true;
	    else
		marker = marker->next;
	} while (!found);
	marker->next = marker->next->next;
    }

    delete to_delete;
}

boolean TList::Empty(TList* list)
{
    if (list == 0)
	return true;
    else
	return false;
}

TList* TList::Next() { return this->next; }




OList::OList(OA* body)
{
    next = 0;
    ORef = body;
}

OList::~OList() {}

void OList::Clear(OList*& to_delete)
{
    OList* ptr = to_delete, *marker;

    do
    {
	marker = ptr;
	ptr = ptr->next;
	delete marker;
    } while (ptr != 0);

    to_delete = 0;
}

void OList::Add(OList*& list, OA* element)
{
    if (list == 0)
	list = new OList(element);
    else
    {
	OList* marker = list, *ptr;
	do
	{
	    ptr = marker;
	    marker = marker->next;
	} while (marker != 0);

	ptr->next = new OList(element);
    }
}

void OList::Delete(OList*& list, OList*& to_delete)
{
    if (to_delete == list)
	list = list->next;
    else
    {
	OList* marker = list;
	boolean found = false;
	do
	{
	    if (marker->next->ORef->ObjRef->ObjectID() == to_delete->ORef->ObjRef->ObjectID())
		found = true;
	    else
		marker = marker->next;
	} while (!found);
	marker->next = marker->next->next;
    }

    delete to_delete;
}

boolean OList::Empty(OList* list)
{
    if (list == 0)
	return true;
    else
	return false;
}

OList* OList::Next() { return this->next; }
