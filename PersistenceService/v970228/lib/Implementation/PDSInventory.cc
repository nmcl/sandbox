#include <os/string.h>
#include <os/iostream.h>

#ifndef IMPLEMENTATION_PDSINVENTORY_H_
#  include <Implementation/PDSInventory.h>
#endif

#ifndef IMPLEMENTATION_ARJUNA_BASICPDS_H_
#  include <Implementation/Arjuna_BasicPDS.h>
#endif

#ifndef IMPLEMENTATION_ARJUNA_REMOTEPDS_H_
#  include <Implementation/Arjuna_RemotePDS.h>
#endif

PDSListElement* PDSInventory::_headOfList = (PDSListElement*) 0;


class PDSListElement
{
public:
    PDSListElement ();
    ~PDSListElement ();

    CosPersistencePDS::PDS_ptr (*_createVoid)();
    CosPersistencePDS::PDS_ptr (*_createString)(const char* string);

    char*           _name;
    PDSListElement* _next;
};

PDSListElement::PDSListElement ()
			       : _createVoid(0),
				 _createString(0),
				 _name(0),
				 _next(0)
{
}

PDSListElement::~PDSListElement ()
{
    if (_name)
	::delete [] _name;

    if (_next)
	delete _next;
}


PDSInventory::PDSInventory ()
{
}

PDSInventory::~PDSInventory ()
{
    if (_headOfList)
	delete _headOfList;
}

void PDSInventory::addCreateVoid (CosPersistencePDS::PDS_ptr (*create)(), const char* name)
{
    PDSListElement* ptr = new PDSListElement();

    ptr->_createVoid = create;
    ptr->_name = ::strdup(name);
    ptr->_next = _headOfList;
    _headOfList = ptr;
}

/*
 * Raise an exception instead or returning NIL!
 * Also for PID inventory.
 */

CosPersistencePDS::PDS_ptr PDSInventory::createVoid (const char* name)
{
    PDSListElement* ptr = _headOfList;

    while (ptr)
    {
	if ((::strcmp(ptr->_name, name) == 0) && (ptr->_createVoid))
	    return (*ptr->_createVoid)();
	else
	    ptr = ptr->_next;
    }

    return CosPersistencePDS::PDS::_nil();
}
