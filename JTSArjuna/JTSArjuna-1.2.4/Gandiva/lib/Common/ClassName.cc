/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ClassName.cc,v 1.7 1996/05/30 13:01:00 nmcl Exp $
 */

#ifndef OS_STRING_H_
#  include <os/string.h>
#endif

#ifndef OS_IOSTREAM_H_
#  include <os/iostream.h>
#endif

#ifndef INTERFACE_BUFFER_H_
#  include <Interface/Buffer.h>
#endif

#ifndef COMMON_CLASSNAME_H_
#   include <Common/ClassName.h>
#endif

ClassName *ClassName::_invalid = NULL;

ClassName::ClassName(const ClassName &name) : _className(NULL)
{
    *this = name;
}

ClassName::ClassName(const char *className) : _className(NULL)
{
    if (className != NULL)
    {
        _className = new char[strlen(className) + 1];
	strcpy(_className, className);
    }
    else
	*this = ClassName::invalid();
}

ClassName::~ClassName()
{
    if (_className != NULL)
        delete [] _className;
}

const ClassName &ClassName::invalid()
{
    if (_invalid == NULL)
	_invalid = new ClassName("$Invalid");
    
    return *_invalid;
}

/*
 * The caller is responsible for freeing this string.
 */

char *ClassName::stringForm() const
{
    if (_className == NULL)
        return NULL;

    char *ptr = new char[strlen(_className) + 1];
    strcpy(ptr, _className);

    return ptr;
}

ClassName &ClassName::operator= (const ClassName &className)
{
    if (this == &className)
        return *this;

    if (className._className != NULL)
    {
        if (_className)
	    delete [] _className;

        _className = new char [strlen(className._className) + 1];
        strcpy(_className, className._className);
    }
    else
	cerr << "ClassName::operator= - no class name available!" << endl;

    return *this;
}

Boolean ClassName::pack(Buffer &buff) const
{
    return buff.pack(_className);
}

Boolean ClassName::unpack(Buffer &buff)
{
    if (_className)
    {
        delete [] _className;
	_className = NULL;
    }

    return buff.unpack(_className);
}
    
Boolean ClassName::operator== (const ClassName &className) const
{
    if (_className != className._className)
	return (strcmp(_className, className._className) == 0);
    else
	return TRUE;
}

Boolean ClassName::operator!= (const ClassName &className) const
{
    if (_className == className._className)
	return FALSE;
    else
	return (strcmp(_className, className._className) != 0);
}

ostream &ClassName::print(ostream &ostr) const
{
    ostr << "<ClassName:" << _className << '>';

    return ostr;
}

ostream &operator<< (ostream &strm, const ClassName &className)
{
    return className.print(strm);
}
