/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: TemplateObject.h,v 1.3 1995/02/10 11:57:34 ngdp Exp $
 */

#ifndef TEMPLATEOBJECT_H_
#define TEMPLATEOBJECT_H_

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif


class ostream;
class ReplicaDescriptor;
class GroupData;

extern const char* templateName;
extern const char* wildCardName;
extern const char* globalWildCardName;


/*
 * The Template object indicates the number of replicas and their location
 * in the system - it does not indicate the names of the replicas.
 * Currently, a template governs all replicas in the group, and if too many
 * replica "names" are given, the rest are ignored.
 */

class TemplateObject
{
    friend class ReplicaList;
    friend class ComplexObjectControl;

public:
    TemplateObject ();
    virtual ~TemplateObject ();

    void addTemplate (const char*);
    void deleteTemplate (const char*);

    ReplicaDescriptor* getTemplate () const;
    long sizeofTemplate () const;

    Boolean pack (Buffer&) const;
    Boolean unpack (Buffer&);

    static void deleteAll (TemplateObject*&);

    virtual ostream& print (ostream&) const;

private:
    GroupData* tmplate;
};


extern ostream& operator<< (ostream& strm, const TemplateObject& to);

#endif
