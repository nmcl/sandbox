/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: SNS.cc,v 1.9 1997/12/22 11:58:36 nmcl Exp $
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

#ifndef OS_SYS_TYPES_H_
#  include <os/sys/types.h>
#endif

#ifndef COMMON_UTILITY_H_
#  include <Common/Utility.h>
#endif

#ifndef GANDIVA_H_
#  include <Gandiva.h>
#endif

#ifndef IMPLEMENTATION_NAMESERVICE_SNS_H_
#  include <Implementation/NameService/SNS.h>
#endif

#ifndef COMMON_OBJECTNAME_H_
#  include <Common/ObjectName.h>
#endif

#ifndef IMPLEMENTATION_NAMESERVICE_SNSATTRIBUTENODE_H_
#  include <Implementation/NameService/SNSAttributeNode.h>
#endif

#ifndef IMPLEMENTATION_NAMESERVICE_SNSATTRIBUTE_H_
#  include <Implementation/NameService/SNSAttribute.h>
#endif

static const char* snsName = "SNS:";

extern const char* nameDelim;
extern const char* tokens;


SNSNameServiceImple::SNSNameServiceImple ()
					 : root(0),
					   current(0),
					   initialized(FALSE),
					   isWindow(FALSE)
{
}

/*
 * This constructor is used when creating an ObjectName from part of
 * an already created hierarchy tree. As such we must remove the root
 * if it is abstract (SNS:) because this is what ObjectName
 * would do if it were able.
 */

SNSNameServiceImple::SNSNameServiceImple (SNSAttributeNode*& ptr)
					 : root(ptr),
					   current(0),
					   initialized(TRUE),
					   isWindow(TRUE)
{
    if (root)
        current = root->nextAttributeLevel();
}

SNSNameServiceImple::~SNSNameServiceImple ()
{
    if ((root) && (!isWindow))
        delete root;
}

Boolean SNSNameServiceImple::objectName (ObjectName& name)
{
    char* objName = (char*) 0;
    
    if (saveState(objName))
    {
	if (objName)
	{
	    name = objName;
	    ::delete [] objName;
	
	    return TRUE;
	}
    }

    return FALSE;
}

SNSNameServiceImple& SNSNameServiceImple::operator= (const char* name)
{
    if ((root) && (!isWindow))
    {
        delete root;
	root = current = (SNSAttributeNode*) 0;
    }

    (void) loadState(name);

    isWindow = FALSE;
    initialized = TRUE;

    return *this;
}

Boolean SNSNameServiceImple::uniqueAttributeName (const char*, char*& attrName)
{
    Uid uid;

    attrName = uid.stringForm();
    
    return TRUE;
}

Boolean SNSNameServiceImple::uniqueObjectName (ObjectName& objectName)
{
    Uid uid;

    char* name = uid.stringForm();
    char* objName = ::appendStrings(snsName, name);
    ObjectName uniqueName(objName);

    objectName = uniqueName;

    if (name)
	::delete [] name;

    if (objName)
	::delete [] objName;

    return TRUE;    
}

Boolean SNSNameServiceImple::removeAttribute (char*& objName,
					      const char* attrName)
{
    if (!attrName)
	return FALSE;

    if (!initialized)
	*this = objName;

    if (!root)
	return FALSE;

    SNSAttributeNode* iter = current;
    SNSAttributeNode* trail = iter;

    while (iter)
    {
	if (::strcmp(iter->name(), attrName) == 0)
	{
	    if (trail == iter)
	    {
		if (root == current)
		    root = iter->sameLevel;
		else
		    root->nextLevel = iter->sameLevel;
		
		current = iter->sameLevel;
	    }
	    else
		trail->sameLevel = iter->sameLevel;
		
	    iter->sameLevel = NULL;
	    delete iter;

	    return saveState(objName);
	}
	else
	{
	    trail = iter;
	    iter = iter->nextAttribute();
	}
    }

    return FALSE;
}

/*
 * Return root's attribute name.
 */

Boolean SNSNameServiceImple::firstAttributeName (const char* objName,
						 char*& attrName)
{
    if (!initialized)
	*this = objName;
    
    if (!root)
	return FALSE;

    SNSAttributeNode* node = root->nextAttributeLevel();

    if (!node)
	return FALSE;

    if (attrName)
	::delete [] attrName;
    
    const char* name = node->name();
    
    attrName = ::new char[::strlen(name)+1];
    ::strcpy(attrName, name);
    
    return TRUE;
}

/*
 * Relative to name given.
 */

Boolean SNSNameServiceImple::nextAttributeName (const char* objName,
						const char* attrName,
						char*& nextName)
{
    if (!initialized)
	*this = objName;
    
    if (!root)
	return FALSE;

    SNSAttributeNode* iter = (SNSAttributeNode*) 0;
    
    if ((current) && (::strcmp(current->name(), attrName) == 0))
	iter = current->nextAttribute();
    else
    {
	Boolean found = FALSE;
	
	iter = root->nextAttributeLevel();

	while ((iter) && (!found))
	{
	    const char* name = iter->name();

	    if (::strcmp(name, attrName) == 0)
		found = TRUE;
	    else
		iter = iter->nextAttribute();
	}
    }

    if (iter)
    {
	const char* name = iter->name();
	
	if (nextName)
	    ::delete [] nextName;
	    
	nextName = ::new char[::strlen(name)+1];
	::strcpy(nextName, name);
	
	return TRUE;
    }
    else
	return FALSE;
}

/*
 * Find the given attribute name, and get the value at the next
 * level in the tree. Return the node which corresponds to the
 * name.
 */

SNSAttributeNode* SNSNameServiceImple::findAttribute (const char* attrName,
						      SNSAttribute& attr)
{
    if (!root)
	return NULL;

    if (!attrName)
    {
	cerr << "WARNING: SNSNameServiceImple::findAttribute - No attribute name given." << endl;
	return (SNSAttributeNode*) 0;
    }
    
    SNSAttributeNode* iter = current;

    while (iter)
    {
	if (::strcmp(iter->name(), attrName) == 0)
	{
	    attr = iter->value();
	    return iter;
	}
	else
	    iter = iter->nextAttribute();
    }

    return NULL;
}
	    
Boolean SNSNameServiceImple::getAttribute (const char* objName,
					   const char* attrName,
					   long& value)
{
    Boolean result = FALSE;
    SNSAttribute a;
    
    if (!initialized)
	*this = objName;
    
    if (findAttribute(attrName, a))
    {
	ObjectName::AttributeType _type;
	result = a.type(_type);

	if ((!result) || (_type != ObjectName::SIGNED_NUMBER))
	    return FALSE;

        a >> value;
    }

    return result;
}

Boolean SNSNameServiceImple::getAttribute (const char* objName,
					   const char* attrName,
					   unsigned long& value)
{
    Boolean result = FALSE;
    SNSAttribute a;

    if (!initialized)
	*this = objName;	

    if (findAttribute(attrName, a))
    {
	ObjectName::AttributeType _type;
	result = a.type(_type);

	if ((!result) || (_type != ObjectName::UNSIGNED_NUMBER))
	    return FALSE;

        a >> value;
    }

    return result;
}

Boolean SNSNameServiceImple::getAttribute (const char* objName,
					   const char* attrName,
					   char*& value)
{
    Boolean result = FALSE;
    SNSAttribute a;

    if (!initialized)
	*this = objName;	

    if (findAttribute(attrName, a))
    {
	ObjectName::AttributeType _type;
	result = a.type(_type);

	if ((!result) || (_type != ObjectName::STRING))
	    return FALSE;

        if (value)
	    ::delete [] value;

	size_t s = a.length();
	
	value = ::new char[s+1];
	::memset(value, '\0', s+1);

	a >> value;
    }

    return result;
}

Boolean SNSNameServiceImple::getAttribute (const char* objName,
					   const char* attrName,
					   ClassName& value)
{
    Boolean result = FALSE;
    SNSAttribute a;

    if (!initialized)
	*this = objName;

    if (findAttribute(attrName, a))
    {
	ObjectName::AttributeType _type;
	result = a.type(_type);

	if ((!result) || (_type != ObjectName::CLASSNAME))
	    return FALSE;

	a >> value;
    }
    
    return result;
}

Boolean SNSNameServiceImple::getAttribute (const char* objName,
					   const char* attrName,
					   ObjectName& value)
{
    Boolean result = FALSE;
    SNSAttribute a;
    SNSAttributeNode* tmpNode = NULL;

    if (!initialized)
	*this = objName;

    if ((tmpNode = findAttribute(attrName, a)))
    {
	ObjectName::AttributeType _type;
	result = a.type(_type);
    
	if ((!result) || (_type != ObjectName::OBJECTNAME))
	    return FALSE;

	/*
	 * If there is a next level then this is effectively
	 * a cached (SNS) object name, so no further
	 * resolution is required. Therefore, we return a window
	 * into the current tree.
	 */
	    
	if (tmpNode->nextAttributeLevel())
	{
	    char* tempPtr = tmpNode->value().stringForm();

	    if (tempPtr)
	    {
		SNSNameServiceImple* p1 = new SNSNameServiceImple(tmpNode);
		ObjectName windowView(new NameService(p1), tempPtr);
		::delete [] tempPtr;
		value = windowView;
	    }
	    else
		return FALSE;
	}
	else
	    a >> value;
    }
    
    return result;
}

Boolean SNSNameServiceImple::getAttribute (const char* objName,
					   const char* attrName,
					   Uid& value)
{
    Boolean result = FALSE;
    SNSAttribute a;

    if (!initialized)
	*this = objName;

    if (findAttribute(attrName, a))
    {
	ObjectName::AttributeType _type;
	result = a.type(_type);

	if ((!result) || (_type != ObjectName::UID))
	    return FALSE;

	a >> value;
    }
    
    return result;
}

Boolean SNSNameServiceImple::setAttribute (char*& objName,
					   const char* attrName,
					   long value)
{
    if (!attrName)
	return FALSE;

    if (!initialized)
	*this = objName;

    if (!root)
	return FALSE;

    SNSAttribute dummy;

    if (!current)
    {
	dummy << value;
	current = root->addAttributeLevel(attrName, dummy);
    }
    else
    {
	SNSAttributeNode* present = findAttribute(attrName, dummy);

	dummy << value;
    
	if (present)
	    present->setValue(dummy);
	else
	    current->addAttribute(attrName, dummy);
    }
	
    return saveState(objName);
}

Boolean SNSNameServiceImple::setAttribute (char*& objName,
					   const char* attrName,
					   unsigned long value)
{
    if (!attrName)
	return FALSE;

    if (!initialized)
	*this = objName;

    if (!root)
	return FALSE;

    SNSAttribute dummy;

    if (!current)
    {
	dummy << value;
	current = root->addAttributeLevel(attrName, dummy);
    }
    else
    {
	SNSAttributeNode* present = findAttribute(attrName, dummy);

	dummy << value;
    
	if (present)
	    present->setValue(dummy);
	else
	    current->addAttribute(attrName, dummy);
    }

    return saveState(objName);
}

Boolean SNSNameServiceImple::setAttribute (char*& objName,
					   const char* attrName,
					   const char* value)
{
    if (!attrName)
	return FALSE;

    if (!initialized)
	*this = objName;

    if (!root)
	return FALSE;

    SNSAttribute dummy;
    
    if (!current)
    {
	dummy << value;
	current = root->addAttributeLevel(attrName, dummy);
    }
    else
    {
	SNSAttributeNode* present = findAttribute(attrName, dummy);

	dummy << value;
    
	if (present)
	    present->setValue(dummy);
	else
	    current->addAttribute(attrName, dummy);
    }
	
    return saveState(objName);
}

Boolean SNSNameServiceImple::setAttribute (char*& objName,
					   const char* attrName,
					   const ObjectName& value)
{
    if (!attrName)
	return FALSE;

    if (!initialized)
	*this = objName;

    if (!root)
	return FALSE;

    SNSAttribute dummy;
    
    if (!current)
    {
	dummy << value;
	current = root->addAttributeLevel(attrName, dummy);
    }
    else
    {
	SNSAttributeNode* present = findAttribute(attrName, dummy);

	dummy << value;
    
	if (present)
	    present->setValue(dummy);
	else
	    current->addAttribute(attrName, dummy);
    }
	
    return saveState(objName);
}    

Boolean SNSNameServiceImple::setAttribute (char*& objName,
					   const char* attrName,
					   const ClassName& value)
{
    if (!attrName)
	return FALSE;

    if (!initialized)
	*this = objName;

    if (!root)
	return FALSE;

    SNSAttribute dummy;
    
    if (!current)
    {
	dummy << value;
	current = root->addAttributeLevel(attrName, dummy);
    }
    else
    {
	SNSAttributeNode* present = findAttribute(attrName, dummy);

	dummy << value;
    
	if (present)
	    present->setValue(dummy);
	else
	    current->addAttribute(attrName, dummy);
    }
	
    return saveState(objName);    
}

Boolean SNSNameServiceImple::setAttribute (char*& objName,
					   const char* attrName,
					   const Uid& value)
{
    if (!attrName)
	return FALSE;

    if (!initialized)
	*this = objName;

    if (!root)
	return FALSE;

    SNSAttribute dummy;
    
    if (!current)
    {
	dummy << value;
	current = root->addAttributeLevel(attrName, dummy);
    }
    else
    {
	SNSAttributeNode* present = findAttribute(attrName, dummy);

	dummy << value;
    
	if (present)
	    present->setValue(dummy);
	else
	    current->addAttribute(attrName, dummy);
    }
	
    return saveState(objName);    
}

Boolean SNSNameServiceImple::attributeType (const char* objName,
					    const char* attrName,
					    ObjectName::AttributeType& attrType)
{
    if (!initialized)
	*this = objName;

    SNSAttribute attr;

    if (findAttribute(attrName, attr))
	return attr.type(attrType);

    return FALSE;
}

Boolean SNSNameServiceImple::loadState (const char* state)
{
    if (!state)
	state = "";

    if (state)
    {
	int dummy = 0;
	
	root = new SNSAttributeNode(state, dummy);
	current = root->nextAttributeLevel();

	return TRUE;
    }
    else
	return FALSE;
}

Boolean SNSNameServiceImple::saveState (char*& objName)
{
    if (root)
    {
        if (objName)
	    ::delete [] objName;

	/*
	 * Consider root separately as it does not have the same
	 * format, i.e., it is only a name and not a name/value pair.
	 */

	const char* _name = root->name();
	char* tmpPtr = ::new char[::strlen(_name)+1];
	::strcpy(tmpPtr, _name);
	
	SNSAttributeNode* realName = root->nextAttributeLevel();

	if (realName)
	    objName = realName->pack(tmpPtr);
	else
	    objName = tmpPtr;

	return TRUE;
    }
    else
        return FALSE;
}

ostream& SNSNameServiceImple::print (ostream& strm) const
{
    strm << "SNSNameServiceImple ";
    
    if ((root) && (!isWindow))
    {
	strm << ":\n";
	root->print(strm);
    }
    else
    {
	if (current)
	{
	    strm << "(copy) :\n";
	    current->print(strm);
	}
	else
	    strm << ": NULL";
    }
    
    return strm;
}

ostream& operator<< (ostream& strm, const SNSNameServiceImple& s)
{
    return s.print(strm);
}

const ClassName& SNSNameServiceImple::name ()
{
    return Gandiva::Implementation::NameService::SNS::name();
}

const ClassName& SNSNameServiceImple::className () const
{
    return Gandiva::Implementation::NameService::SNS::name();
}

Resource* SNSNameServiceImple::create ()
{
    return new SNSNameServiceImple;
}

NameServiceImple* SNSNameServiceImple::clone ()
{
    if (isWindow)
	return this;
    else
	return NameServiceImple::castup(create());
}

Resource* SNSNameServiceImple::control (const ClassName&)
{
    return (Resource*) 0;
}

void* SNSNameServiceImple::castup (const ClassName& _type) const
{
    if (_type == name())
        return (void*) this;
    else
        return NameServiceImple::castup(_type);
}

SNSNameServiceImple* SNSNameServiceImple::castup (Resource* toCast)
{
    if (toCast == (Resource*) 0)
        return (SNSNameServiceImple*) 0;
    else
        return (SNSNameServiceImple*) toCast->castup(name());
}
