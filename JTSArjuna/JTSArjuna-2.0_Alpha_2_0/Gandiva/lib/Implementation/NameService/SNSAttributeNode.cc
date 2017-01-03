/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newscastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: SNSAttributeNode.cc,v 1.4 1997/06/02 10:28:25 nmcl Exp $
 */

#ifndef OS_IOSTREAM_H_
#  include <os/iostream.h>
#endif

#ifndef OS_STDLIB_H_
#  include <os/stdlib.h>
#endif

#ifndef OS_STRING_H_
#  include <os/string.h>
#endif

#ifndef CORE_RESOURCE_H_
#  include <Core/Resource.h>
#endif

#ifndef IMPLEMENTATION_NAMESERVICE_SNSATTRIBUTENODE_H_
#   include <Implementation/NameService/SNSAttributeNode.h>
#endif

// various tokens

const char* tokens = ",<>";
const char attrSeparator = ',';
const char startType = '<';
const char endType = '>';


SNSAttributeNode::SNSAttributeNode (const SNSAttribute& attr)
				   : _name(0),
				     _value(0),
				     sameLevel(0),
				     nextLevel(0)
{
    _value = new SNSAttribute(attr);
}

SNSAttributeNode::SNSAttributeNode (const char* formatString, int& index)
				   : _name(0),
				     _value(0),
				     sameLevel(0),
				     nextLevel(0)
{
    int numberUsed = parse(formatString);

    index += numberUsed;
}

SNSAttributeNode::SNSAttributeNode (const char* name, const SNSAttribute& val)
				   : _name(0),
				     _value(new SNSAttribute(val)),
				     sameLevel(0),
				     nextLevel(0)
{
    _name = ::new char[::strlen(name)+1];
    ::strcpy(_name, name);
}

SNSAttributeNode::SNSAttributeNode (const SNSAttributeNode& assign)
				   : _name(0),
				     _value(0),
				     sameLevel(0),
				     nextLevel(0)
{
    *this = assign;
}

SNSAttributeNode::~SNSAttributeNode ()
{
    if (_name)
	::delete [] _name;
    if (_value)
        delete _value;
    if (nextLevel)
        delete nextLevel;
    if (sameLevel)
        delete sameLevel;
}

void SNSAttributeNode::addAttribute (const char* name, const SNSAttribute& val)
{
    add(new SNSAttributeNode(name, val));
}

SNSAttributeNode* SNSAttributeNode::addAttributeLevel (const char* name,
						       const SNSAttribute& val)
{
    SNSAttributeNode* ptr = new SNSAttributeNode(name, val);
    
    if (nextLevel)
	nextLevel->add(ptr);
    else
	nextLevel = ptr;

    return ptr;
}

void SNSAttributeNode::add (SNSAttributeNode* toAdd)
{
    SNSAttributeNode* ptr = (SNSAttributeNode*) 0;

    ptr = toAdd;

    if (sameLevel == (SNSAttributeNode*) 0)
        sameLevel = ptr;
    else
        sameLevel->add(ptr);
}

void SNSAttributeNode::setName (const char* name)
{
    if (!name)
	return;

    if (_name)
	::delete [] _name;

    _name = ::new char[::strlen(name)+1];
    ::strcpy(_name, name);
}

void SNSAttributeNode::setValue (const SNSAttribute& val)
{
    if (_value)
	*_value = val;
    else
	_value = new SNSAttribute(val);
}

void SNSAttributeNode::set (const char* attr, int size)
{
    char* tmpName = ::new char[size+1];
    ::memset(tmpName, '\0', size+1);
    ::memcpy(tmpName, attr, size);

    if (!_name)
	_name = tmpName;
    else
    {
	if (_value)
	{
	    cerr << "Set error. Have " << *_value << endl;
	    cerr << "getting " << tmpName << endl;
	    abort();
	}
	
	_value = new SNSAttribute(tmpName);
	::delete [] tmpName;
    }
}

int SNSAttributeNode::parse (const char* attributeString)
{
    if (attributeString == (char*) 0)
	return 0;
    
    const char* attrIndex = attributeString;
    int size = ::strlen(attrIndex);
    int numberUsed = 0;

    while (numberUsed < size)
    {
        int span = ::strcspn(attrIndex, tokens);

	if (span > 0)
	{
	    set(attrIndex, span);

	    if (span + numberUsed == size) // all remaining characters
	    {
	        return span+numberUsed;
	    }
	}

	attrIndex += span;
	numberUsed += span;

	char currentToken = attrIndex[0];

	switch (currentToken)
	{
	case startType:
	    {
		attrIndex++;
		numberUsed++;

		int munched = 0;
		
		SNSAttributeNode* toAdd = new SNSAttributeNode(attrIndex, munched);

		if (nextLevel)
		    nextLevel->add(toAdd);
		else
		    nextLevel = toAdd;

		attrIndex += munched;
		numberUsed += munched;
	    }
	break;
	case attrSeparator:
	    {
		attrIndex++;
		numberUsed++;
		span = ::strcspn(attrIndex, tokens);
		set(attrIndex, span);
		attrIndex += span;
		numberUsed += span;
	    }
	break;
	case endType:
	    {
		numberUsed++;
		attrIndex++;
		
		return numberUsed;
	    }
	break;
	}
    }

    return numberUsed;
}      

char* SNSAttributeNode::pack (char* ptr) const
{
    char* attr = _value->stringForm();
    size_t attrSize = ::strlen(_name) + ::strlen(attr) +3;

    if (ptr)
	attrSize += ::strlen(ptr);

    char* namePtr = ::new char[attrSize+1];
    size_t index = 0;
    
    ::memset(namePtr, '\0', attrSize+1);

    if (ptr)
    {
	::strcpy(namePtr, ptr);
	index = ::strlen(ptr);
	::delete [] ptr;
    }

    /*
     * Pack in <name,value> format.
     */

    namePtr[index] = startType;
    index++;
    ::strcat(namePtr, _name);
    index += ::strlen(_name);
    namePtr[index] = attrSeparator;
    ::strcat(namePtr, attr);
    index += ::strlen(attr);
    namePtr[index+1] = endType;

    ::delete [] attr;
    
    char* name2 = ((nextLevel) ? nextLevel->pack(namePtr) : namePtr);

    return ((sameLevel) ? sameLevel->pack(name2) : name2);
}

const char* SNSAttributeNode::name () const
{
    if (!_name)
	return "";
    else
	return _name;
}

const SNSAttribute& SNSAttributeNode::value () const
{
    if (!_value)
    {
	cerr << "SNSAttributeNode::value error - no value to return!" << endl;
	abort();
    }
    
    return *_value;
}

SNSAttributeNode* SNSAttributeNode::nextAttributeLevel () const
{
    return nextLevel;
}

SNSAttributeNode* SNSAttributeNode::nextAttribute () const
{
    return sameLevel;
}

ostream& SNSAttributeNode::print (ostream& strm, int indent) const
{
    for (int i = 0; i < indent; i++)
        strm << " ";

    if (!_value)
	strm << " < " << ((_name) ? _name : "NULL") << ", NULL >" << endl;
    else
	strm << " < " << ((_name) ? _name : "NULL") << ", " << *_value << " >" << endl;
    
    if (nextLevel)
	nextLevel->print(strm, indent+4);

    if (sameLevel)
        sameLevel->print(strm, indent);

    return strm;
}

ostream& operator<< (ostream& ostr, const SNSAttributeNode& attrNode)
{
    return attrNode.print(ostr);
}
