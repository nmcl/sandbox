/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: GandivaDedicated.cc,v 1.6 1998/01/12 13:09:54 nmcl Exp $
 */

#include <os/stddef.h>
#include <os/iostream.h>
#include <os/string.h>

#ifndef GANDIVA_H_
#   include <Gandiva.h>
#endif

#ifndef COMMON_BOOLEAN_H_
#   include <Common/Boolean.h>
#endif

#ifndef COMMON_OBJECTNAME_H_
#   include <Common/ObjectName.h>
#endif

#ifndef COMMON_CLASSNAME_H_
#   include <Common/ClassName.h>
#endif

#ifndef TEMPLATE_LIST_H_
#   include <Template/List.h>
#endif

#ifndef CORE_RESOURCE_H_
#   include <Core/Resource.h>
#endif

#ifndef IMPLEMENTATION_BUFFER_QANBO_H_
#   include <Implementation/Buffer/QANBO.h>
#endif

#ifndef IMPLEMENTATION_INVENTORY_CREATEFUNCTIONLIST_H_
#   include <Implementation/Inventory/CreateFunctionList.h>
#endif

#ifndef WIN32

#ifndef IMPLEMENTATION_NAMESERVICE_INS_H_
#   include <Implementation/NameService/INS.h>
#endif

#ifndef IMPLEMENTATION_NAMESERVICE_LNS_H_
#   include <Implementation/NameService/LNS.h>
#endif

#else

#ifndef IMPLEMENTATION_NAMESERVICE_RNS_H_
#  include <Implementation/NameService/RNS.h>
#endif

#endif

#ifndef IMPLEMENTATION_NAMESERVICE_SNS_H_
#  include <Implementation/NameService/SNS.h>
#endif

#ifndef IMPLEMENTATION_INVENTORY_H_
#   include <Implementation/Inventory.h>
#endif

#ifndef INTERFACE_INVENTORY_H_
#   include <Interface/Inventory.h>
#endif

#ifndef IMPLEMENTATION_INVENTORY_GANDIVADEDICATED_H_
#   include <Implementation/Inventory/GandivaDedicated.h>
#endif

Resource *GandivaDedicatedInventoryImple::createVoid(const ClassName &className)
{
    Resource *res = NULL;

    if (className == QANBOBufferImple::name())
        res = QANBOBufferImple::create();
    else if (className == GandivaDedicatedInventoryImple::name())
        res = GandivaDedicatedInventoryImple::create();
    else if (className == CreateFunctionListInventoryImple::name())
        res = CreateFunctionListInventoryImple::create();
#ifndef WIN32
    else if (className == INSNameServiceImple::name())
        res = INSNameServiceImple::create();
    else if (className == LNSNameServiceImple::name())
        res = LNSNameServiceImple::create();
#else
	else if (className == RNSNameServiceImple::name())
		res = RNSNameServiceImple::create();
#endif
    else if (className == SNSNameServiceImple::name())
	res = SNSNameServiceImple::create();
    else
        res = NULL;

    return res;
}

Resource *GandivaDedicatedInventoryImple::createClassName(const ClassName &, const ClassName &)
{
    return NULL;
}

Resource *GandivaDedicatedInventoryImple::createObjectName(const ClassName &, const ObjectName &)
{
    return NULL;
}

Resource *GandivaDedicatedInventoryImple::createResources(const ClassName &, Resource *[])
{
    return NULL;
}

Resource *GandivaDedicatedInventoryImple::createClassNameResources(const ClassName &, const ClassName &, Resource *[])
{
    return NULL;
}

Resource *GandivaDedicatedInventoryImple::createObjectNameResources(const ClassName &, const ObjectName &, Resource *[])
{
    return NULL;
}

Boolean GandivaDedicatedInventoryImple::addCreateVoid(const ClassName &className, Resource *(*create)(void))
{
    return ((className == QANBOBufferImple::name()) &&
            (create == QANBOBufferImple::create)) ||
	((className == GandivaDedicatedInventoryImple::name()) &&
	 (create == GandivaDedicatedInventoryImple::create)) ||
	((className == CreateFunctionListInventoryImple::name()) &&
	 (create == CreateFunctionListInventoryImple::create)) ||
#ifndef WIN32
	((className == INSNameServiceImple::name()) &&
	 (create == INSNameServiceImple::create)) ||
	((className == LNSNameServiceImple::name()) &&
	 (create == LNSNameServiceImple::create)) ||
#else
	((className == RNSNameServiceImple::name()) &&
	 (create == RNSNameServiceImple::create)) ||
#endif
	((className == SNSNameServiceImple::name()) &&
	 (create == SNSNameServiceImple::create));
}

Boolean GandivaDedicatedInventoryImple::addCreateClassName(const ClassName &, Resource *(*)(const ClassName &))
{
    return FALSE;
}

Boolean GandivaDedicatedInventoryImple::addCreateObjectName(const ClassName &, Resource *(*)(const ObjectName &))
{
    return FALSE;
}

Boolean GandivaDedicatedInventoryImple::addCreateResources(const ClassName &, Resource *(*)(Resource *[]))
{
    return FALSE;
}

Boolean GandivaDedicatedInventoryImple::addCreateClassNameResources(const ClassName &, Resource *(*)(const ClassName &, Resource *[]))
{
    return FALSE;
}

Boolean GandivaDedicatedInventoryImple::addCreateObjectNameResources(const ClassName &, Resource *(*)(const ObjectName &, Resource *[]))
{
    return FALSE;
}

Boolean GandivaDedicatedInventoryImple::addInventory(Inventory *)
{
    return FALSE;
}

const ClassName &GandivaDedicatedInventoryImple::name()
{
    return Gandiva::Implementation::Inventory::GandivaDedicated::name();
}

const ClassName &GandivaDedicatedInventoryImple::className() const
{
    return Gandiva::Implementation::Inventory::GandivaDedicated::name();
}

void GandivaDedicatedInventoryImple::print(ostream &ostr)
{
    ostr << "<Inventory:" << endl;

    ostr << "void" << endl << '{' << endl;
    ostr << "  [" << QANBOBufferImple::name() << ',' 
         << (void*) QANBOBufferImple::create << ']' << endl;
    ostr << "  [" << GandivaDedicatedInventoryImple::name() << ',' 
         << (void*) GandivaDedicatedInventoryImple::create << ']' << endl;
    ostr << "  [" << CreateFunctionListInventoryImple::name() << ',' 
         << (void*) CreateFunctionListInventoryImple::create << ']' << endl;
#ifndef WIN32
    ostr << "  [" << INSNameServiceImple::name() << ',' 
         << (void*) INSNameServiceImple::create << ']' << endl;
    ostr << "  [" << LNSNameServiceImple::name() << ',' 
         << (void*) LNSNameServiceImple::create << ']' << endl;
#else
    ostr << "  [" << RNSNameServiceImple::name() << ',' 
         << (void*) RNSNameServiceImple::create << ']' << endl;
#endif
    ostr << "  [" << SNSNameServiceImple::name() << ',' 
         << (void*) SNSNameServiceImple::create << ']' << endl;
    ostr << '}' << endl << "classname" << endl << '{' << endl;
    ostr << '}' << endl << "objectname" << endl << '{' << endl;
    ostr << '}' << endl << "resource" << endl << '{' << endl;
    ostr << '}' << endl << "classname, resources" << endl << '{' << endl;
    ostr << '}' << endl << "objectname, resources" << endl << '{' << endl;
    ostr << '}' << endl << '>' << endl;
}

Resource *GandivaDedicatedInventoryImple::create()
{
    return new GandivaDedicatedInventoryImple;
}

Resource *GandivaDedicatedInventoryImple::control(const ClassName &)
{
    return NULL;
}

GandivaDedicatedInventoryImple *GandivaDedicatedInventoryImple::castup(Resource *resource)
{
    if (resource != NULL)
        return (GandivaDedicatedInventoryImple *) resource->castup(GandivaDedicatedInventoryImple::name());
    else
        return NULL;
}

void *GandivaDedicatedInventoryImple::castup(const ClassName &className) const
{
    if (className == GandivaDedicatedInventoryImple::name())
        return (void *) this;
    else
        return InventoryImple::castup(className);
}

GandivaDedicatedInventoryImple::GandivaDedicatedInventoryImple()
{
}

GandivaDedicatedInventoryImple::~GandivaDedicatedInventoryImple()
{
}
