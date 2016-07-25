/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ObjStoreList.h,v 
 */


#ifndef OBJSTORELIST_H_
#define OBJSTORELIST_H_

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

class ObjectStore;


class ObjectStoreList
{
public:
    ObjectStoreList (ObjectStore* (*func1)(const char*), void (*func2)(const char*), const TypeName);
    ~ObjectStoreList();
    
    ObjectStore* create (const char*);
    void destroy (const char*);

    const TypeName type () const;

    ObjectStoreList* next;

private:
    ObjectStore* (*StoreCreate)(const char*);
    void (*StoreDestroy)(const char*);
    const TypeName typeName;
};

#endif
