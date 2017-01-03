/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CreateFunctionList.cc,v 1.3 1996/01/02 13:15:54 nsmw Exp $
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

#ifndef IMPLEMENTATION_INVENTORY_H_
#   include <Implementation/Inventory.h>
#endif

#ifndef INTERFACE_INVENTORY_H_
#   include <Interface/Inventory.h>
#endif

#ifndef IMPLEMENTATION_INVENTORY_CREATEFUNCTIONLIST_H_
#   include <Implementation/Inventory/CreateFunctionList.h>
#endif

class CreateVoidListElem
{
public:
   CreateVoidListElem();
   virtual ~CreateVoidListElem();

   ClassName className;
   Resource  *(*create)(void);
};

CreateVoidListElem::CreateVoidListElem() : className(NULL),
                                           create(NULL)
{
}

CreateVoidListElem::~CreateVoidListElem()
{
}

class CreateClassNameListElem
{
public:
   CreateClassNameListElem();
   virtual ~CreateClassNameListElem();

   ClassName className;
   Resource  *(*create)(const ClassName &className);
};

CreateClassNameListElem::CreateClassNameListElem() : className(NULL),
                                                     create(NULL)
{
}

CreateClassNameListElem::~CreateClassNameListElem()
{
}

class CreateObjectNameListElem
{
public:
   CreateObjectNameListElem();
   virtual ~CreateObjectNameListElem();

   ClassName className;
   Resource  *(*create)(const ObjectName &objectName);
};

CreateObjectNameListElem::CreateObjectNameListElem() : className(NULL),
                                                       create(NULL)
{
}

CreateObjectNameListElem::~CreateObjectNameListElem()
{
}

class CreateResourcesListElem
{
public:
   CreateResourcesListElem();
   virtual ~CreateResourcesListElem();

   ClassName className;
   Resource  *(*create)(Resource *resources[]);
};

CreateResourcesListElem::CreateResourcesListElem() : className(NULL),
                                                     create(NULL)
{
}

CreateResourcesListElem::~CreateResourcesListElem()
{
}

class CreateClassNameResourcesListElem
{
public:
   CreateClassNameResourcesListElem();
   virtual ~CreateClassNameResourcesListElem();

   ClassName className;
   Resource  *(*create)(const ClassName &className, Resource *resources[]);
};

CreateClassNameResourcesListElem::CreateClassNameResourcesListElem() : className(NULL),
                                                                       create(NULL)
{
}

CreateClassNameResourcesListElem::~CreateClassNameResourcesListElem()
{
}

class CreateObjectNameResourcesListElem
{
public:
   CreateObjectNameResourcesListElem();
   virtual ~CreateObjectNameResourcesListElem();

   ClassName className;
   Resource  *(*create)(const ObjectName &objectName, Resource *resources[]);
};

CreateObjectNameResourcesListElem::CreateObjectNameResourcesListElem() : className(NULL),
                                                                         create(NULL)
{
}

CreateObjectNameResourcesListElem::~CreateObjectNameResourcesListElem()
{
}

Resource *CreateFunctionListInventoryImple::createVoid(const ClassName &className)
{
    Boolean found = FALSE;
    ListIterator<CreateVoidListElem> listIterator(&_createVoidList);

    listIterator.head();
    while ((! found) && listIterator.content() != NULL)
    {
        if (listIterator.content()->className == className)
            found = TRUE;
	else
	    listIterator.next();
    }

    if (found)
        return listIterator.content()->create();
    else
        return NULL;
}

Resource *CreateFunctionListInventoryImple::createClassName(const ClassName &className, const ClassName &paramClassName)
{
    Boolean found = FALSE;
    ListIterator<CreateClassNameListElem> listIterator(&_createClassNameList);

    listIterator.head();
    while ((! found) && listIterator.content() != NULL)
    {
        if (listIterator.content()->className == className)
            found = TRUE;
	else
	    listIterator.next();
    }

    if (found)
        return listIterator.content()->create(paramClassName);
    else
        return NULL;
}

Resource *CreateFunctionListInventoryImple::createObjectName(const ClassName &className, const ObjectName &paramObjectName)
{
    Boolean found = FALSE;
    ListIterator<CreateObjectNameListElem> listIterator(&_createObjectNameList);

    listIterator.head();
    while ((! found) && listIterator.content() != NULL)
    {
        if (listIterator.content()->className == className)
            found = TRUE;
	else
	    listIterator.next();
    }

    if (found)
        return listIterator.content()->create(paramObjectName);
    else
        return NULL;
}

Resource *CreateFunctionListInventoryImple::createResources(const ClassName &className, Resource *paramResources[])
{
    Boolean found = FALSE;
    ListIterator<CreateResourcesListElem> listIterator(&_createResourcesList);

    listIterator.head();
    while ((! found) && listIterator.content() != NULL)
    {
        if (listIterator.content()->className == className)
            found = TRUE;
	else
	    listIterator.next();
    }

    if (found)
        return listIterator.content()->create(paramResources);
    else
        return NULL;
}

Resource *CreateFunctionListInventoryImple::createClassNameResources(const ClassName &className, const ClassName &paramClassName, Resource *paramResources[])
{
    Boolean found = FALSE;
    ListIterator<CreateClassNameResourcesListElem> listIterator(&_createClassNameResourcesList);

    listIterator.head();
    while ((! found) && listIterator.content() != NULL)
    {
        if (listIterator.content()->className == className)
            found = TRUE;
	else
	    listIterator.next();
    }

    if (found)
        return listIterator.content()->create(paramClassName, paramResources);
    else
        return NULL;
}

Resource *CreateFunctionListInventoryImple::createObjectNameResources(const ClassName &className, const ObjectName &paramObjectName, Resource *paramResources[])
{
    Boolean found = FALSE;
    ListIterator<CreateObjectNameResourcesListElem> listIterator(&_createObjectNameResourcesList);

    listIterator.head();
    while ((! found) && listIterator.content() != NULL)
    {
        if (listIterator.content()->className == className)
            found = TRUE;
	else
	    listIterator.next();
    }

    if (found)
        return listIterator.content()->create(paramObjectName, paramResources);
    else
        return NULL;
}

Boolean CreateFunctionListInventoryImple::addCreateVoid(const ClassName &className, Resource *(*create)(void))
{
    CreateVoidListElem *elem = new CreateVoidListElem;

    elem->className = className;
    elem->create    = create;

    _createVoidList.append(elem);

    return TRUE;
}

Boolean CreateFunctionListInventoryImple::addCreateClassName(const ClassName &className, Resource *(*create)(const ClassName &className))
{
    CreateClassNameListElem *elem = new CreateClassNameListElem;

    elem->className = className;
    elem->create    = create;

    _createClassNameList.append(elem);

    return TRUE;
}

Boolean CreateFunctionListInventoryImple::addCreateObjectName(const ClassName &className, Resource *(*create)(const ObjectName &objectName))
{
    CreateObjectNameListElem *elem = new CreateObjectNameListElem;

    elem->className = className;
    elem->create    = create;

    _createObjectNameList.append(elem);

    return TRUE;
}

Boolean CreateFunctionListInventoryImple::addCreateResources(const ClassName &className, Resource *(*create)(Resource *resources[]))
{
    CreateResourcesListElem *elem = new CreateResourcesListElem;

    elem->className = className;
    elem->create    = create;

    _createResourcesList.append(elem);

    return TRUE;
}

Boolean CreateFunctionListInventoryImple::addCreateClassNameResources(const ClassName &className, Resource *(*create)(const ClassName &className, Resource *resources[]))
{
    CreateClassNameResourcesListElem *elem = new CreateClassNameResourcesListElem;

    elem->className = className;
    elem->create    = create;

    _createClassNameResourcesList.append(elem);

    return TRUE;
}

Boolean CreateFunctionListInventoryImple::addCreateObjectNameResources(const ClassName &className, Resource *(*create)(const ObjectName &objectName, Resource *resources[]))
{
    CreateObjectNameResourcesListElem *elem = new CreateObjectNameResourcesListElem;

    elem->className = className;
    elem->create    = create;

    _createObjectNameResourcesList.append(elem);

    return TRUE;
}

Boolean CreateFunctionListInventoryImple::addInventory(Inventory *)
{
    return FALSE;
}

const ClassName &CreateFunctionListInventoryImple::name()
{
    return Gandiva::Implementation::Inventory::CreateFunctionList::name();
}

const ClassName &CreateFunctionListInventoryImple::className() const
{
    return Gandiva::Implementation::Inventory::CreateFunctionList::name();
}

void CreateFunctionListInventoryImple::print(ostream &ostr)
{
    ostr << "<Inventory:" << endl;

    ostr << "void" << endl << '{' << endl;

    ListIterator<CreateVoidListElem> createVoidListIterator(&_createVoidList);

    createVoidListIterator.head();
    while (createVoidListIterator.content() != NULL)
    {
        ostr << "  [" << createVoidListIterator.content()->className 
             << ',' << (void*) createVoidListIterator.content()->create << ']'
             << endl;

	createVoidListIterator.next();
    }

    ostr << '}' << endl << "classname" << endl << '{' << endl;

    ListIterator<CreateClassNameListElem> createClassNameListIterator(&_createClassNameList);

    createClassNameListIterator.head();
    while (createClassNameListIterator.content() != NULL)
    {
        ostr << "  [" << createClassNameListIterator.content()->className
             << ',' << (void*) createClassNameListIterator.content()->create << ']'
             << endl;

	createClassNameListIterator.next();
    }

    ostr << '}' << endl << "objectname" << endl << '{' << endl;

    ListIterator<CreateObjectNameListElem> createObjectNameListIterator(&_createObjectNameList);

    createObjectNameListIterator.head();
    while (createObjectNameListIterator.content() != NULL)
    {
        ostr << "  [" << createObjectNameListIterator.content()->className
             << ',' << (void*) createObjectNameListIterator.content()->create << ']'
             << endl;

	createObjectNameListIterator.next();
    }

    ostr << '}' << endl << "resource" << endl << '{' << endl;

    ListIterator<CreateResourcesListElem> createResourcesListIterator(&_createResourcesList);

    createResourcesListIterator.head();
    while (createResourcesListIterator.content() != NULL)
    {
        ostr << "  [" << createResourcesListIterator.content()->className
             << ',' << (void*) createResourcesListIterator.content()->create << ']'
             << endl;

	createResourcesListIterator.next();
    }

    ostr << '}' << endl << "classname, resources" << endl << '{' << endl;

    ListIterator<CreateClassNameResourcesListElem> createClassNameResourcesListIterator(&_createClassNameResourcesList);

    createClassNameResourcesListIterator.head();
    while (createClassNameResourcesListIterator.content() != NULL)
    {
        ostr << "  [" << createClassNameResourcesListIterator.content()->className
             << ',' << (void*) createClassNameResourcesListIterator.content()->create << ']'
             << endl;

	createClassNameResourcesListIterator.next();
    }

    ostr << '}' << endl << "objectname, resources" << endl << '{' << endl;

    ListIterator<CreateObjectNameResourcesListElem> createObjectNameResourcesListIterator(&_createObjectNameResourcesList);

    createObjectNameResourcesListIterator.head();
    while (createObjectNameResourcesListIterator.content() != NULL)
    {
        ostr << "  [" << createObjectNameResourcesListIterator.content()->className
             << ',' << (void*) createObjectNameResourcesListIterator.content()->create << ']'
             << endl;

	createObjectNameResourcesListIterator.next();
    }

    ostr << '}' << endl << '>' << endl;
}

Resource *CreateFunctionListInventoryImple::create()
{
    return new CreateFunctionListInventoryImple;
}

Resource *CreateFunctionListInventoryImple::control(const ClassName &)
{
    return NULL;
}

CreateFunctionListInventoryImple *CreateFunctionListInventoryImple::castup(Resource *resource)
{
    if (resource != NULL)
        return (CreateFunctionListInventoryImple *) resource->castup(CreateFunctionListInventoryImple::name());
    else
        return NULL;
}

void *CreateFunctionListInventoryImple::castup(const ClassName &className) const
{
    if (className == CreateFunctionListInventoryImple::name())
        return (void *) this;
    else
        return InventoryImple::castup(className);
}

CreateFunctionListInventoryImple::CreateFunctionListInventoryImple()
{
}

CreateFunctionListInventoryImple::~CreateFunctionListInventoryImple()
{
}
