/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: INS.cc,v 1.14 1998/01/12 13:10:10 nmcl Exp $
 */

#include <os/sys/stat.h>
#include <os/fcntl.h>
#include <os/ctype.h>
#include <os/stdlib.h>
#include <os/stdio.h>
#include <os/string.h>
#include <os/unistd.h>
#include <os/ndbm.h>

#ifndef GANDIVA_H_
#   include <Gandiva.h>
#endif

#ifndef COMMON_THREAD_THREAD_H_
#  include <Common/Thread/Thread.h>
#endif

#ifndef COMMON_BOOLEAN_H_
#   include <Common/Boolean.h>
#endif

#ifndef COMMON_UTILITY_H_
#  include <Common/Utility.h>
#endif

#ifndef COMMON_UID_H_
#   include <Common/Uid.h>
#endif

#ifndef CORE_RESOURCE_H_
#   include <Core/Resource.h>
#endif

#ifndef COMMON_OBJECTNAME_H_
#   include <Common/ObjectName.h>
#endif

#ifndef COMMON_CLASSNAME_H_
#   include <Common/ClassName.h>
#endif

#ifndef INTERFACE_NAMESERVICE_H_
#   include <Interface/NameService.h>
#endif

#ifndef IMPLEMENTATION_NAMESERVICE_H_
#   include <Implementation/NameService.h>
#endif

#ifndef IMPLEMENTATION_NAMESERVICE_INS_H_
#   include <Implementation/NameService/INS.h>
#endif

#ifndef INS_ROOT
#define INS_ROOT "/usr/tmp/INS_root"
#endif

#ifndef INS_GET_FLAGS
#define INS_GET_FLAGS (O_RDONLY | O_CREAT)
#endif

#ifndef INS_SET_FLAGS
#define INS_SET_FLAGS (O_RDWR | O_CREAT)
#endif

#ifndef INS_PERM
#define INS_PERM (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)
#endif

Boolean INSNameServiceImple::attributeType(const char *objName,
					   const char *attrName,
					   ObjectName::AttributeType &attrType)
{
    Boolean res = FALSE;

    if ((objName != NULL) && (attrName != NULL))
    {
        ObjectName objectName(ObjectName::invalid());

        if (getObjectName(objName, objectName))
        {
            if (strcmp(attrName, "$ObjectName") == 0)
            {
                attrType = ObjectName::OBJECTNAME;
                res = TRUE;
            }
            else
                res = objectName.attributeType(attrName, attrType);
        }
    }

    return res;
}

Boolean INSNameServiceImple::firstAttributeName(const char*, char *&attrName)
{
    attrName = new char[strlen("$ObjectName") + 1];
    strcpy(attrName, "$ObjectName");

    return TRUE;
}

Boolean INSNameServiceImple::nextAttributeName(const char *objName,
					       const char *attrName,
					       char *&nextAttrName)
{
    Boolean res = FALSE;

    if (nextAttrName != NULL)
    {
        delete nextAttrName;
        nextAttrName = NULL;
    }

    if ((objName != NULL) && (attrName != NULL))
    {
        ObjectName objectName(ObjectName::invalid());

        if (getObjectName(objName, objectName))
        {
            if (strcmp(attrName, "$ObjectName") == 0)
                res = objectName.firstAttributeName(nextAttrName);
            else
                res = objectName.nextAttributeName(attrName, nextAttrName);
        }
    }

    return res;
}

Boolean INSNameServiceImple::getAttribute(const char *objName,
					  const char *attrName, long &value)
{
    Boolean res = FALSE;

    if ((objName != NULL) && (attrName != NULL))
    {
        ObjectName objectName(ObjectName::invalid());

        if ((strcmp(attrName, "$ObjectName") != 0) && (getObjectName(objName, objectName)))
            res = objectName.getAttribute(attrName, value);
    }

    return res;
}

Boolean INSNameServiceImple::getAttribute(const char *objName,
					  const char *attrName,
					  unsigned long &value)
{
    Boolean res = FALSE;

    if ((objName != NULL) && (attrName != NULL))
    {
        ObjectName objectName(ObjectName::invalid());

        if ((strcmp(attrName, "$ObjectName") != 0) && (getObjectName(objName, objectName)))
            res = objectName.getAttribute(attrName, value);
    }

    return res;
}

Boolean INSNameServiceImple::getAttribute(const char *objName,
					  const char *attrName, char *&value)
{
    Boolean res = FALSE;

    if ((objName != NULL) && (attrName != NULL))
    {
        ObjectName objectName(ObjectName::invalid());

        if ((strcmp(attrName, "$ObjectName") != 0) && (getObjectName(objName, objectName)))
            res = objectName.getAttribute(attrName, value);
    }

    return res;
}

Boolean INSNameServiceImple::getAttribute(const char *objName,
					  const char *attrName,
					  ObjectName &value)
{
    Boolean res = FALSE;

    if ((objName != NULL) && (attrName != NULL))
    {
        if (strcmp(attrName, "$ObjectName") == 0)
            res = getObjectName(objName, value);
        else
        {
            ObjectName objectName(ObjectName::invalid());

            if (getObjectName(objName, objectName))
                res = objectName.getAttribute(attrName, value);
        }
    }

    return res;
}

Boolean INSNameServiceImple::getAttribute(const char *objName,
					  const char *attrName,
					  ClassName &value)
{
    Boolean res = FALSE;

    if ((objName != NULL) && (attrName != NULL))
    {
        ObjectName objectName(ObjectName::invalid());

        if ((strcmp(attrName, "$ObjectName") != 0) && (getObjectName(objName, objectName)))
            res = objectName.getAttribute(attrName, value);
    }

    return res;
}

Boolean INSNameServiceImple::getAttribute(const char *objName,
					  const char *attrName, Uid &value)
{
    Boolean res = FALSE;

    if ((objName != NULL) && (attrName != NULL))
    {
        ObjectName objectName(ObjectName::invalid());

        if ((strcmp(attrName, "$ObjectName") != 0) && (getObjectName(objName, objectName)))
            res = objectName.getAttribute(attrName, value);
    }

    return res;
}

Boolean INSNameServiceImple::setAttribute(char *&objName,
					  const char *attrName, long value)
{
    Boolean res = FALSE;

    if ((objName != NULL) && (attrName != NULL))
    {
        ObjectName objectName(ObjectName::invalid());

        if ((strcmp(attrName, "$ObjectName") != 0) && (getObjectName(objName, objectName)))
            res = objectName.setAttribute(attrName, value);
    }

    return res;
}

Boolean INSNameServiceImple::setAttribute(char *&objName,
					  const char *attrName,
					  unsigned long value)
{
    Boolean res = FALSE;

    if ((objName != NULL) && (attrName != NULL))
    {
        ObjectName objectName(ObjectName::invalid());

        if ((strcmp(attrName, "$ObjectName") != 0) && (getObjectName(objName, objectName)))
            res = objectName.setAttribute(attrName, value);
    }

    return res;
}

Boolean INSNameServiceImple::setAttribute(char *&objName,
					  const char *attrName,
					  const char *value)
{
    Boolean res = FALSE;

    if ((objName != NULL) && (attrName != NULL))
    {
        ObjectName objectName(ObjectName::invalid());

        if ((strcmp(attrName, "$ObjectName") != 0) && (getObjectName(objName, objectName)))
            res = objectName.setAttribute(attrName, value);
    }

    return res;
}

Boolean INSNameServiceImple::setAttribute(char *&objName,
					  const char *attrName,
					  const ObjectName &value)
{
    Boolean res = FALSE;

    if ((objName != NULL) && (attrName != NULL))
    {
        if (strcmp(attrName, "$ObjectName") == 0)
            res = setObjectName(objName, value);
        else
        {
            ObjectName objectName(ObjectName::invalid());

            if (getObjectName(objName, objectName))
                res = objectName.setAttribute(attrName, value);
        }
    }

    return res;
}

Boolean INSNameServiceImple::setAttribute(char *&objName,
					  const char *attrName,
					  const ClassName &value)
{
    Boolean res = FALSE;

    if ((objName != NULL) && (attrName != NULL))
    {
        ObjectName objectName(ObjectName::invalid());

        if ((strcmp(attrName, "$ObjectName") != 0) && (getObjectName(objName, objectName)))
            res = objectName.setAttribute(attrName, value);
    }

    return res;
}

Boolean INSNameServiceImple::setAttribute(char *&objName,
					  const char *attrName,
					  const Uid &value)
{
    Boolean res = FALSE;

    if ((objName != NULL) && (attrName != NULL))
    {
        ObjectName objectName(ObjectName::invalid());

        if ((strcmp(attrName, "$ObjectName") != 0) && (getObjectName(objName, objectName)))
            res = objectName.setAttribute(attrName, value);
    }

    return res;
}

Boolean INSNameServiceImple::removeAttribute(char *&objName,
					     const char *attrName)
{
    Boolean res = FALSE;

    if ((objName != NULL) && (attrName != NULL))
    {
        if (strcmp(attrName, "$ObjectName") == 0)
            res = removeObjectName(objName);
        else
        {
            ObjectName objectName(ObjectName::invalid());

            if (getObjectName(objName, objectName))
                res = objectName.removeAttribute(attrName);
        }
    }

    return res;
}

Boolean INSNameServiceImple::uniqueAttributeName(const char *, char *&attrName)
{
    Uid uid;

    attrName = uid.stringForm();

    return TRUE;
}

Boolean INSNameServiceImple::uniqueObjectName(ObjectName &objectName)
{
    Uid uid;

    char *uniqueString           = uid.stringForm();
    char *uniqueObjectNameString = appendStrings("INS:", uniqueString);
    ObjectName uniqueObjectName(uniqueObjectNameString);

    objectName = uniqueObjectName;

    if (uniqueString != NULL)
        delete[] uniqueString;

    if (uniqueObjectNameString != NULL)
        delete[] uniqueObjectNameString;

    return TRUE;
}

Boolean INSNameServiceImple::getObjectName(const char *objName,
					   ObjectName &objectName)
{
    Boolean res  = FALSE;
    DBM     *dbm = NULL;

    res = openDBForReading(dbm);

    if (res && (dbm != NULL) && (objName != NULL))
    {
        datum key;
        datum data;
        
        key.dptr  = (char *) objName;
        key.dsize = strlen(objName) + 1;

        data = dbm_fetch(dbm, key);

        if (data.dptr != NULL)
        {
            ObjectName on(data.dptr);

            objectName = on;
            res = TRUE;
        }

        closeDB(dbm);
    }

    return res;
}

Boolean INSNameServiceImple::setObjectName(const char *objName,
					   const ObjectName &objectName)
{
    Boolean res  = FALSE;
    DBM     *dbm = NULL;

    res = openDBForWriting(dbm);

    if (res && (dbm != NULL) && (objName != NULL))
    {
        datum key;
        datum data;
        
        key.dptr   = (char *) objName;
        key.dsize  = strlen(objName) + 1;

        data.dptr  = objectName.stringForm();
        data.dsize = strlen(data.dptr) + 1;

        if (! dbm_store(dbm, key, data, DBM_REPLACE))
            res = TRUE;

        if (data.dptr != NULL)
            delete data.dptr;

        closeDB(dbm);
    }

    return res;
}

static const int maxOpenRetries  = 100;
static const int openRetryPeriod = 100; /* usec */

Boolean INSNameServiceImple::openDBForReading(DBM *&dbm)
{
    int    openRetries = 0;
    mode_t oldMode     = umask(! (S_IRWXU | S_IRWXG | S_IRWXO));

    dbm = NULL;

    dbm = dbm_open(INS_ROOT, INS_GET_FLAGS, INS_PERM);

    while ((dbm == NULL) && (openRetries < maxOpenRetries))
    {
        if ((openRetries % 2) == 0)
            dbm = dbm_open(INS_ROOT, INS_SET_FLAGS, INS_PERM);
        else
            dbm = dbm_open(INS_ROOT, INS_GET_FLAGS, INS_PERM);
        
        if (dbm == NULL)
            Thread::sleep(openRetryPeriod);

        openRetries++;
    }

    umask(oldMode);

    return (dbm != NULL);
}

Boolean INSNameServiceImple::openDBForWriting(DBM *&dbm)
{
    int    openRetries = 0;
    mode_t oldMode     = umask(! (S_IRWXU | S_IRWXG | S_IRWXO));

    dbm = NULL;

    while ((dbm == NULL) && (openRetries < maxOpenRetries))
    {
        dbm = dbm_open(INS_ROOT, INS_SET_FLAGS, INS_PERM);

        if (dbm == NULL)
            Thread::sleep(openRetryPeriod);

        openRetries++;
    }

    umask(oldMode);

    return (dbm != NULL);
}

Boolean INSNameServiceImple::closeDB(DBM *dbm)
{
    if (dbm != NULL)
        dbm_close(dbm);

    return TRUE;
}

Boolean INSNameServiceImple::removeObjectName(const char *objName)
{
    Boolean res  = FALSE;
    DBM     *dbm = NULL;

    res = openDBForWriting(dbm);

    if (res && (dbm != NULL) && (objName != NULL))
    {
        datum key;
        
        key.dptr  = (char *) objName;
        key.dsize = strlen(objName) + 1;

        if (! dbm_delete(dbm, key))
            res = TRUE;

        closeDB(dbm);
    }

    return res;
}

const ClassName &INSNameServiceImple::name()
{
    return Gandiva::Implementation::NameService::INS::name();
}

const ClassName &INSNameServiceImple::className() const
{
    return Gandiva::Implementation::NameService::INS::name();
}

Resource *INSNameServiceImple::create()
{
    return new INSNameServiceImple;
}

Resource *INSNameServiceImple::control(const ClassName &)
{
    return NULL;
}

NameServiceImple *INSNameServiceImple::clone()
{
    return NameServiceImple::castup(create());
}

INSNameServiceImple::INSNameServiceImple()
{
}

INSNameServiceImple::~INSNameServiceImple()
{
}

INSNameServiceImple *INSNameServiceImple::castup(Resource *resource)
{
    if (resource != NULL)
        return (INSNameServiceImple *) resource->castup(INSNameServiceImple::name());
    else
        return NULL;
}

void *INSNameServiceImple::castup(const ClassName &className) const
{
    if (className == INSNameServiceImple::name())
        return (void *) this;
    else
        return NameServiceImple::castup(className);
}
