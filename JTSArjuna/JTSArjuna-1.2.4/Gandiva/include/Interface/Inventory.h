/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Inventory.h,v 1.4 1996/01/02 13:12:57 nsmw Exp $
 */

#ifndef INTERFACE_INVENTORY_H_
#define INTERFACE_INVENTORY_H_

#ifndef OS_STDDEF_H_
#   include <os/stddef.h>
#endif

#ifndef COMMON_BOOLEAN_H_
#   include <Common/Boolean.h>
#endif

class Inventory;

class ObjectName;

#ifndef COMMON_CLASSNAME_H_
#   include <Common/ClassName.h>
#endif

#ifndef CORE_RESOURCE_H_
#   include <Core/Resource.h>
#endif

#ifndef AssertImplePresent
#   if defined(__STDC__) || defined(CPP_HAS_HASHHASH)
#       define AssertImplePresent(implementation) extern int imple_##implementation##_present;\
               static int uses_##implementation##=##imple_##implementation##_present;
#   else
#       define AssertImplePresent(implementation) extern int imple_/**/implementation/**/_present;\
               static int uses_/**/implementation/**/=/**/imple_/**/implementation/**/_present;
#   endif
#endif

#ifndef ImplePresent
#   if defined(__STDC__) || defined(CPP_HAS_HASHHASH)
#       define ImplePresent(implementation) int imple_##implementation##_present;
#   else
#       define ImplePresent(implementation) int imple_/**/implementation/**/_present;
#   endif
#endif

class InventoryImple;

class Inventory : virtual public Resource
{
public:
    Resource *createVoid(const ClassName &className) const;
    Resource *createClassName(const ClassName &className,
			      const ClassName &paramClassName) const;
    Resource *createObjectName(const ClassName &className,
			       const ObjectName &paramObjectName) const;
    Resource *createResources(const ClassName &className,
			      Resource *paramResources[]) const;
    Resource *createClassNameResources(const ClassName &className,
				       const ClassName &paramClassName,
				       Resource *paramResources[]) const;
    Resource *createObjectNameResources(const ClassName &className,
					const ObjectName &paramObjectName,
					Resource *paramResources[]) const;
 
    Boolean addCreateVoid(const ClassName &className,
			  Resource *(*create)(void));
    Boolean addCreateClassName(const ClassName &className,
			       Resource *(*create)(const ClassName &className));
    Boolean addCreateObjectName(const ClassName &className,
				Resource *(*create)(const ObjectName &objectName));
    Boolean addCreateResources(const ClassName &className,
			       Resource *(*create)(Resource *resources[]));
    Boolean addCreateClassNameResources(const ClassName &className,
					Resource *(*create)(const ClassName &className,
							    Resource *resources[]));
    Boolean addCreateObjectNameResources(const ClassName &className,
					 Resource *(*create)(const ObjectName &objectName,
							     Resource *resources[]));

    Boolean addInventory(Inventory *inventory);

    static  const ClassName &name();
    virtual const ClassName &className() const;
    virtual const ClassName &impleClassName() const;

    static  Inventory *create(const ObjectName &objectName);
    static  Inventory *create(const ClassName &inventImpleName);

    static  Inventory *inventory();

    virtual Resource  *control(const ClassName &ctrlInterName);

    Inventory(const ObjectName &objectName);
    Inventory(const ClassName &inventImpleName);
    Inventory(const Inventory &invent);
    virtual ~Inventory();

    void print(ostream &ostr);

    static  Inventory *castup(Resource *resource);
    virtual void      *castup(const ClassName &className) const;

protected:
    Inventory(InventoryImple *_inventImple);

private:
    static Inventory *_invent;
    InventoryImple   *_imple;
};

extern ostream &operator<< (ostream &ostr, Inventory &invent);

#endif
