/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: TemplateObject.h,v
 */

#ifndef TEMPLATEOBJECT_H_
#define TEMPLATEOBJECT_H_

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif


class ReplicaDescriptor;
class GroupData;
class ostream;


extern const char* templateName;
extern const char* wildCardName;
extern const char* globalWildCardName;


class TemplateObject
{
    friend class ReplicaList;

public:
    TemplateObject ();
    virtual ~TemplateObject ();

    void addTemplate (const char*);
    void deleteTemplate (const char*);

    ReplicaDescriptor* getTemplate ();

    Boolean pack (Buffer&) const;
    Boolean unpack (Buffer&);

    static void deleteAll (TemplateObject*&);

    virtual ostream& print (ostream&) const;

private:
    GroupData* tmplate;
};


extern ostream& operator<< (ostream& strm, const TemplateObject& to);

#endif
