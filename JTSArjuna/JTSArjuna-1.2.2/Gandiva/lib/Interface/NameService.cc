/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: NameService.cc,v 1.6 1996/01/02 13:34:29 nsmw Exp $
 */

#include <os/stddef.h>

#ifndef GANDIVA_H_
#   include <Gandiva.h>
#endif

#ifndef COMMON_BOOLEAN_H_
#   include <Common/Boolean.h>
#endif

#ifndef CORE_RESOURCE_H_
#   include <Core/Resource.h>
#endif

#ifndef INTERFACE_INVENTORY_H_
#   include <Interface/Inventory.h>
#endif

#ifndef IMPLEMENTATION_NAMESERVICE_H_
#   include <Implementation/NameService.h>
#endif

#ifndef INTERFACE_NAMESERVICE_H_
#   include <Interface/NameService.h>
#endif

Boolean NameService::attributeType(const char *objName, const char *attr, ObjectName::AttributeType &attrType)
{
    if (_imple != NULL)
        return _imple->attributeType(objName, attr, attrType);
    else
        return FALSE;
}

Boolean NameService::firstAttributeName(const char *objName, char *&attrName)
{
    if (_imple != NULL)
        return _imple->firstAttributeName(objName, attrName);
    else
        return FALSE;
}

Boolean NameService::nextAttributeName(const char *objName, const char *attrName, char *&nextAttrName)
{
    if (_imple != NULL)
        return _imple->nextAttributeName(objName, attrName, nextAttrName);
    else
        return FALSE;
}

Boolean NameService::getAttribute(const char *objName, const char *attrName, long &value)
{
    if (_imple != NULL)
        return _imple->getAttribute(objName, attrName, value);
    else
        return FALSE;
}

Boolean NameService::getAttribute(const char *objName, const char *attrName, unsigned long &value)
{
    if (_imple != NULL)
        return _imple->getAttribute(objName, attrName, value);
    else
        return FALSE;
}

Boolean NameService::getAttribute(const char *objName, const char *attrName, char *&value)
{
    if (_imple != NULL)
        return _imple->getAttribute(objName, attrName, value);
    else
        return FALSE;
}

Boolean NameService::getAttribute(const char *objName, const char *attrName, ObjectName &value)
{
    if (_imple != NULL)
        return _imple->getAttribute(objName, attrName, value);
    else
        return FALSE;
}

Boolean NameService::getAttribute(const char *objName, const char *attrName, ClassName &value)
{
    if (_imple != NULL)
        return _imple->getAttribute(objName, attrName, value);
    else
        return FALSE;
}

Boolean NameService::getAttribute(const char *objName, const char *attrName, Uid &value)
{
    if (_imple != NULL)
        return _imple->getAttribute(objName, attrName, value);
    else
        return FALSE;
}

Boolean NameService::setAttribute(char *&objName, const char *attrName, long value)
{
    if (_imple != NULL)
        return _imple->setAttribute(objName, attrName, value);
    else
        return FALSE;
}

Boolean NameService::setAttribute(char *&objName, const char *attrName, unsigned long value)
{
    if (_imple != NULL)
        return _imple->setAttribute(objName, attrName, value);
    else
        return FALSE;
}

Boolean NameService::setAttribute(char *&objName, const char *attrName, const char *value)
{
    if (_imple != NULL)
        return _imple->setAttribute(objName, attrName, value);
    else
        return FALSE;
}

Boolean NameService::setAttribute(char *&objName, const char *attrName, const ObjectName &value)
{
    if (_imple != NULL)
        return _imple->setAttribute(objName, attrName, value);
    else
        return FALSE;
}

Boolean NameService::setAttribute(char *&objName, const char *attrName, const ClassName &value)
{
    if (_imple != NULL)
        return _imple->setAttribute(objName, attrName, value);
    else
        return FALSE;
}

Boolean NameService::setAttribute(char *&objName, const char *attrName, const Uid &value)
{
    if (_imple != NULL)
        return _imple->setAttribute(objName, attrName, value);
    else
        return FALSE;
}

Boolean NameService::removeAttribute(char *&objName, const char *attrName)
{
    if (_imple != NULL)
        return _imple->removeAttribute(objName, attrName);
    else
        return FALSE;
}

Boolean NameService::uniqueAttributeName(const char *objName, char *&attrName)
{
    if (_imple != NULL)
        return _imple->uniqueAttributeName(objName, attrName);
    else
        return FALSE;
}

Boolean NameService::uniqueObjectName(ObjectName &objectName)
{
    if (_imple != NULL)
        return _imple->uniqueObjectName(objectName);
    else
        return FALSE;
}

NameService& NameService::operator= (const NameService& toCopy)
{
    if (this == &toCopy)
	return *this;
    
    if (_imple)
	Resource::unref(_imple);

    if (toCopy._imple)
    {
	_imple = toCopy._imple->clone();
	Resource::ref(_imple);
    }
    else
	_imple = NULL;
    
    return *this;
}
	
const ClassName &NameService::name()
{
    return Gandiva::Interface::NameService::name();
}

const ClassName &NameService::className() const
{
    return Gandiva::Interface::NameService::name();
}

const ClassName &NameService::impleClassName() const
{
    if (_imple != NULL)
        return _imple->className();
    else
        return ClassName::invalid();
}

NameService *NameService::create(const ClassName &nameServiceImpleName)
{
    NameService *res    = NULL;
    Inventory  *invent = Inventory::inventory();

    if (invent != NULL)
    {
        Resource::ref(invent);

        NameServiceImple *nameServiceImple = NameServiceImple::castup(invent->createVoid(nameServiceImpleName));

        if (nameServiceImple != NULL)
            res = new NameService(nameServiceImple);

        Resource::unref(invent);
    }

    return res;
}

Resource *NameService::control(const ClassName &ctrlInterName)
{
    if (_imple != NULL)
        return _imple->control(ctrlInterName);
    else
        return NULL;
}

NameService *NameService::castup(Resource *resource)
{
    if (resource != NULL)
        return (NameService *) resource->castup(NameService::name());
    else
        return NULL;
}

void *NameService::castup(const ClassName &className) const
{
    if (className == NameService::name())
        return (void *) this;
    else
        return NULL;
}

NameService::NameService(const ClassName &nameServiceImpleName)
{
    Inventory *invent = Inventory::inventory();

    _imple = NULL;

    if (invent != NULL)
    {
        Resource::ref(invent);

        _imple = NameServiceImple::castup(invent->createVoid(nameServiceImpleName));
        Resource::ref(_imple);

        Resource::unref(invent);
    }
}

NameService::NameService(const NameService &nameService)
{
    if (nameService._imple)
    {
	_imple = nameService._imple->clone();
	Resource::ref(_imple);
    }
    else
	_imple = NULL;
}

NameService::NameService(NameServiceImple *nameServiceImple)
{
    _imple = nameServiceImple;
    Resource::ref(_imple);
}

NameService::~NameService()
{
    Resource::unref(_imple);
}
