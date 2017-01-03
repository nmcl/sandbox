/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: SNSAttribute.cc,v 1.4 1997/12/22 11:58:37 nmcl Exp $
 */

#ifndef OS_STDLIB_H_
#  include <os/stdlib.h>
#endif

#ifndef OS_IOSTREAM_H_
#  include <os/iostream.h>
#endif

#ifndef WIN32

#ifndef OS_STRSTREAM_H_
#  include <os/strstream.h>
#endif

#else

#ifndef OS_STRSTREA_H_
#  include <os/strstrea.h>
#endif

#endif

#ifndef OS_STRING_H_
#  include <os/string.h>
#endif

#ifndef COMMON_OBJECTNAME_H_
#  include <Common/ObjectName.h>
#endif

#ifndef IMPLEMENTATION_NAMESERVICE_SNSATTRIBUTE_H_
#  include <Implementation/NameService/SNSAttribute.h>
#endif

static const char* signedType = "%";
static const char* unsignedType = "#";
static const char* stringType = "~";
static const char* objectnameType = "&";
static const char* classnameType = "!";
static const char* uidType = "^";


const int MaxAttributeLength = 1024;

SNSAttribute::SNSAttribute ()
			   : _attribute(0),
			     _iparser(0),
			     _oparser(0)
{
}

/*
 * Assume that the string contains the correct format
 * character as the first element.
 */

SNSAttribute::SNSAttribute (const char* attrName)
			   : _attribute(0),
			     _iparser(0),
			     _oparser(0)
{
    copyAttribute(attrName);
}

SNSAttribute::SNSAttribute (const SNSAttribute& assign)
			   : _attribute(0),
			     _iparser(0),
			     _oparser(0)
{
    *this = assign;
}

SNSAttribute::~SNSAttribute ()
{
    if (_attribute)
        ::delete [] _attribute;
    if (_iparser)
	delete _iparser;
    if (_oparser)
        delete _oparser;
}

char* SNSAttribute::stringForm () const
{
    if (_attribute)
    {
	char* ptr = ::new char[::strlen(_attribute)+1];
	::strcpy(ptr, _attribute);
	return ptr;
    }
    else
	return (char*) 0;
}

size_t SNSAttribute::length () const
{
    return ((_attribute) ? ::strlen(_attribute) : 0);
}

void SNSAttribute::copyAttribute (const char* attrName)
{
    if (_iparser)
    {
	delete _iparser;
	_iparser = NULL;
    }

    if (_oparser)
    {
        delete _oparser;
	_oparser = NULL;
    }

    if (_attribute)
    {
	::delete [] _attribute;
	_attribute = (char*) 0;
    }

    if (attrName)
    {
	_attribute = ::new char[::strlen(attrName)+1];
	::strcpy(_attribute, attrName);

#ifndef HAS_ISTRSTREAM_BUG	
	_iparser = new istrstream(_attribute);
#endif	
    }
}

SNSAttribute& SNSAttribute::operator>> (long& l)
{
#ifndef HAS_ISTRSTREAM_BUG    
    if (_iparser)    
    {
	char t;
	(*_iparser) >> t >> l;
    }
#else
    l = ::atol(&_attribute[1]);
#endif    
    
    return *this;
}

SNSAttribute& SNSAttribute::operator>> (unsigned long& l)
{
#ifndef HAS_ISTRSTREAM_BUG    
    if (_iparser)
    {
	char t;
        (*_iparser) >> t >> l;
    }
#else
    l = (unsigned long) ::atol(&_attribute[1]);
#endif    
    
    return *this;
}

SNSAttribute& SNSAttribute::operator>> (short& s)
{
#ifndef HAS_ISTRSTREAM_BUG    
    if (_iparser)
    {
	char t;
	(*_iparser) >> t >> s;
    }
#else
    s = (short) ::atoi(&_attribute[1]);
#endif    
    
    return *this;
}

SNSAttribute& SNSAttribute::operator>> (unsigned short& s)
{
#ifndef HAS_ISTRSTREAM_BUG
    if (_iparser)
    {
	char t;
	(*_iparser) >> t >> s;
    }
#else
    s = (unsigned short) ::atoi(&_attribute[1]);
#endif    
    
    return *this;
}

SNSAttribute& SNSAttribute::operator>> (float& f)
{
#ifndef HAS_ISTRSTREAM_BUG    
    if (_iparser)
    {
	char t;
	(*_iparser) >> t >> f;
    }
#else
    f = (float) ::atof(&_attribute[1]);
#endif    
    
    return *this;
}

SNSAttribute& SNSAttribute::operator>> (char* c)
{
    if (!c)
	return *this;
    
#ifndef HAS_ISTRSTREAM_BUG
    if (_iparser)
    {
	char t;
	(*_iparser) >> t >> c;
    }
#else
    ::strcpy(c, &_attribute[1]);
#endif    
    
    return *this;
}

SNSAttribute& SNSAttribute::operator>> (ClassName& cn)
{
#ifndef HAS_ISTRSTREAM_BUG    
    if (_iparser)
    {
        char* tmp = new char[::strlen(_attribute)+1];
	::memset(tmp, '\0', ::strlen(_attribute)+1);

	char t;
	
	(*_iparser) >> t >> tmp;
	cn = tmp;

	::delete [] tmp;
    }
#else
    char* tmp = new char[::strlen(_attribute)+1];
    ::strcpy(tmp, &_attribute[1]);

    cn = tmp;

    ::delete [] tmp;
#endif    

    return *this;
}

SNSAttribute& SNSAttribute::operator>> (ObjectName& on)
{
#ifndef HAS_ISTRSTREAM_BUG    
    if (_iparser)
    {
	char* tmp = ::new char[::strlen(_attribute)+1];
	::memset(tmp, '\0', ::strlen(_attribute)+1);

	char t;

	(*_iparser) >> t >> tmp;
	
	on = tmp;
	::delete [] tmp;
    }
#else
    char* tmp = new char[::strlen(_attribute)+1];
    ::strcpy(tmp, &_attribute[1]);

    on = tmp;

    ::delete [] tmp;
#endif    
    
    return *this;
}

SNSAttribute& SNSAttribute::operator>> (Uid& uid)
{
#ifndef HAS_ISTRSTREAM_BUG    
    if (_iparser)
    {
	char* tmp = ::new char[::strlen(_attribute)+1];
	::memset(tmp, '\0', ::strlen(_attribute)+1);

	char t;

	(*_iparser) >> t >> tmp;
	
	uid = tmp;
	::delete [] tmp;
    }
#else
    char* tmp = new char[::strlen(_attribute)+1];
    ::strcpy(tmp, &_attribute[1]);

    uid = tmp;

    ::delete [] tmp;
#endif    
    
    return *this;
}

void SNSAttribute::setOutput ()
{
    _attribute = ::new char[MaxAttributeLength+1];
    ::memset(_attribute, '\0', MaxAttributeLength+1);
    _oparser = new ostrstream(_attribute, MaxAttributeLength+1, ios::out);
}

SNSAttribute& SNSAttribute::operator<< (long l)
{
    if (_iparser)
        return *this;

    if (!_oparser)
        setOutput();

    (*_oparser) << signedType << l;
    
    return *this;
}

SNSAttribute& SNSAttribute::operator<< (unsigned long l)
{
    if (_iparser)
        return *this;

    if (!_oparser)
        setOutput();

    (*_oparser) << unsignedType << l;
    
    return *this;
}

SNSAttribute& SNSAttribute::operator<< (short s)
{
    if (_iparser)
        return *this;

    if (!_oparser)
        setOutput();

    (*_oparser) << signedType << s;

    return *this;
}

SNSAttribute& SNSAttribute::operator<< (unsigned short s)
{
    if (_iparser)
        return *this;

    if (!_oparser)
        setOutput();

    (*_oparser) << unsignedType << s;

    return *this;
}

SNSAttribute& SNSAttribute::operator<< (float f)
{
    if (_iparser)
        return *this;

    if (!_oparser)
        setOutput();

    (*_oparser) << signedType << f;
    
    return *this;
}

SNSAttribute& SNSAttribute::operator<< (const char* c)
{
    if ((_iparser) || (!c))
        return *this;

    if (!_oparser)
        setOutput();

    (*_oparser) << stringType << c;
    
    return *this;
}

SNSAttribute& SNSAttribute::operator<< (const ClassName& cn)
{
    if (_iparser)
        return *this;

    if (!_oparser)
        setOutput();

    char* ptr = cn.stringForm();

    (*_oparser) << classnameType << ptr;
    ::delete [] ptr;

    return *this;
}

SNSAttribute& SNSAttribute::operator<< (const ObjectName& objName)
{
    if (_iparser)
        return *this;

    if (!_oparser)
        setOutput();

    char* ptr = objName.stringForm();
    
    (*_oparser) << objectnameType << ptr;
    ::delete [] ptr;
    
    return *this;
}

SNSAttribute& SNSAttribute::operator<< (const Uid& uid)
{
    if (_iparser)
        return *this;

    if (!_oparser)
        setOutput();

    char* ptr = uid.stringForm();
    
    (*_oparser) << uidType << ptr;
    ::delete [] ptr;
    
    return *this;
}

Boolean SNSAttribute::type (ObjectName::AttributeType& attrType) const
{
    if (!_attribute)
        return FALSE;

#ifndef HAS_ISTRSTREAM_BUG

#ifndef WIN32
    istrstream tmpParser((const char*) _attribute);
#else
    istrstream tmpParser(_attribute);
#endif

    if (!tmpParser)
        return FALSE;
    
    char t;

    tmpParser >> t;
#else
    char t = _attribute[0];
#endif    

    switch (t)
    {
    case '%':
	attrType = ObjectName::SIGNED_NUMBER;
	break;
    case '#':
	attrType = ObjectName::UNSIGNED_NUMBER;
	break;
    case '~':
	attrType = ObjectName::STRING;
	break;
    case '&':
	attrType = ObjectName::OBJECTNAME;
	break;
    case '!':
	attrType = ObjectName::CLASSNAME;
	break;
    case '^':
	attrType = ObjectName::UID;
	break;
    }

    return TRUE;
}

Boolean SNSAttribute::operator! () const
{
    if (!_iparser)
	return TRUE;
    else
	return !(*_iparser);
}

SNSAttribute& SNSAttribute::operator= (const char* assign)
{
    copyAttribute(assign);
    return *this;
}

SNSAttribute& SNSAttribute::operator= (const SNSAttribute& assign)
{
    if (this == &assign)
        return *this;

    copyAttribute(assign._attribute);
    return *this;
}

Boolean SNSAttribute::operator== (const SNSAttribute& ptr) const
{
    if ((_attribute) && (ptr._attribute))
	return (Boolean) (::strcmp(_attribute, ptr._attribute) == 0);
    else
	return (Boolean) ((_attribute == NULL) && (ptr._attribute == NULL));
}

Boolean SNSAttribute::operator!= (const SNSAttribute& ptr) const
{
    return ((*this == ptr) ? FALSE : TRUE);
}

ostream& SNSAttribute::print (ostream& strm) const
{
    if (_attribute)
    {
	ObjectName::AttributeType t;
	char* realAttr = &_attribute[1];
	
	type(t);
	
        strm << " [ " << t << " ] " << realAttr;
    }
    else
        strm << "NULL";

    return strm;
}

ostream& operator<< (ostream& ostr, const SNSAttribute& attr)
{
    return attr.print(ostr);
}
