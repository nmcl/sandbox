/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: SNSAttributeNode.h,v 1.2 1997/05/20 16:13:36 nmcl Exp $
 */

#ifndef IMPLEMENTATION_NAMESERVICE_SNSATTRIBUTENODE_H_
#define IMPLEMENTATION_NAMESERVICE_SNSATTRIBUTENODE_H_

#ifndef COMMON_BOOLEAN_H_
#   include <Common/Boolean.h>
#endif

#ifndef CORE_RESOURCE_H_
#  include <Core/Resource.h>
#endif

#ifndef COMMON_CLASSNAME_H_
#  include <Common/ClassName.h>
#endif

#ifndef IMPLEMENTATION_NAMESERVICE_SNSATTRIBUTE_H_
#  include <Implementation/NameService/SNSAttribute.h>
#endif


class SNSAttributeNode
{
    friend class SNSNameServiceImple;
    
public:
    SNSAttributeNode  (const char* formatString, int& index);
    SNSAttributeNode  (const char* name, const SNSAttribute& value);
    ~SNSAttributeNode ();

    const char*      name  () const;
    const SNSAttribute& value () const;

    void setName  (const char*);
    void setValue (const SNSAttribute&);
    
    void              addAttribute (const char* name, const SNSAttribute& val);
    SNSAttributeNode* addAttributeLevel (const char* name, const SNSAttribute& val);
    
    SNSAttributeNode* nextAttributeLevel () const;
    SNSAttributeNode* nextAttribute () const;

    ostream& print (ostream& strm, int = 0) const;

    char* pack (char*) const;

protected:
    SNSAttributeNode (const SNSAttribute& toCopy);
    SNSAttributeNode (const SNSAttributeNode& toCopy);
    
private:
    int parse (const char*);
    
    void set (const char*, int);
    void add (SNSAttributeNode*);

    char*         _name;
    SNSAttribute* _value;

    SNSAttributeNode* sameLevel;
    SNSAttributeNode* nextLevel;
};

extern ostream& operator<< (ostream& ostr, const SNSAttributeNode& attrNode);

#endif
