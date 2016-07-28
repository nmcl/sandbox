#include <os/string.h>
#include <os/iostream.h>

#ifndef IMPLEMENTATION_PIDINVENTORY_H_
#  include <Implementation/PIDInventory.h>
#endif

PIDListElement* PIDInventory::_headOfList = (PIDListElement*) 0;

class PIDListElement
{
public:
    PIDListElement ();
    ~PIDListElement ();

    CosPersistencePID::PID_ptr (*_createVoid)();
    CosPersistencePID::PID_ptr (*_createString)(const char* string);

    char*           _name;
    PIDListElement* _next;
};

PIDListElement::PIDListElement ()
			       : _createVoid(0),
				 _createString(0),
				 _name(0),
				 _next(0)
{
}

PIDListElement::~PIDListElement ()
{
    if (_name)
	::delete [] _name;

    if (_next)
	delete _next;
}


PIDInventory::PIDInventory ()
{
}

PIDInventory::~PIDInventory ()
{
    if (_headOfList)
	delete _headOfList;
}

void PIDInventory::addCreateVoid (CosPersistencePID::PID_ptr (*create)(), const char* name)
{
    PIDListElement* ptr = new PIDListElement();

    ptr->_createVoid = create;
    ptr->_name = ::strdup(name);
    ptr->_next = _headOfList;
    _headOfList = ptr;
}

void PIDInventory::addCreateString (CosPersistencePID::PID_ptr (*create)(const char*), const char* name)
{
    PIDListElement* ptr = new PIDListElement();

    ptr->_createString = create;
    ptr->_name = ::strdup(name);
    ptr->_next = _headOfList;
    _headOfList = ptr;
}

CosPersistencePID::PID_ptr PIDInventory::createVoid (const char* name)
{
    PIDListElement* ptr = _headOfList;

    while (ptr)
    {
	if ((::strcmp(ptr->_name, name) == 0) && (ptr->_createVoid))
	    return (*ptr->_createVoid)();
	else
	    ptr = ptr->_next;
    }

    return CosPersistencePID::PID::_nil();
}

CosPersistencePID::PID_ptr PIDInventory::createString (const char* name, const char* string)
{
    PIDListElement* ptr = _headOfList;

    while (ptr)
    {
	if ((::strcmp(ptr->_name, name) == 0) && (ptr->_createString))
	    return (*ptr->_createString)(string);
	else
	    ptr = ptr->_next;
    }

    return CosPersistencePID::PID::_nil();
}
