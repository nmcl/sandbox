/*
 * Copyright (C) 1994, 1996, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: SNS.h,v 1.4 1996/02/02 13:52:54 nmcl Exp $
 */

#ifndef IMPLEMENTATION_NAMESERVICE_SNS_H_
#define IMPLEMENTATION_NAMESERVICE_SNS_H_

#ifndef COMMON_CLASSNAME_H_
#  include <Common/ClassName.h>
#endif

#ifndef COMMON_OBJECTNAME_H_
#  include <Common/ObjectName.h>
#endif

#ifndef IMPLEMENTATION_NAMESERVICE_H_
#  include <Implementation/NameService.h>
#endif

#ifndef INTERFACE_NAMESERVICE_H_
#   include <Interface/NameService.h>
#endif

class SNSAttribute;
class SNSAttributeNode;


class SNSNameServiceImple : public NameServiceImple
{
public:
    SNSNameServiceImple& operator=  (const char* name);

    virtual NameServiceImple* clone ();
    
    virtual Boolean getAttribute (const char* objName, const char* attrName,
				  long& value);
    virtual Boolean getAttribute (const char* objName, const char* attrName,
				  unsigned long& value);
    virtual Boolean getAttribute (const char* objName, const char* attrName,
				  char*& value);
    virtual Boolean getAttribute (const char* objName, const char* attrName,
				  ObjectName& value);
    virtual Boolean getAttribute (const char* objName, const char* attrName,
				  ClassName& value);
    virtual Boolean getAttribute (const char* objName, const char* attrName,
				  Uid& value);

    virtual Boolean setAttribute (char*& objName, const char* attrName,
				  long value);
    virtual Boolean setAttribute (char*& objName, const char* attrName,
				  unsigned long value);
    virtual Boolean setAttribute (char*& objName, const char* attrName,
				  const char* value);
    virtual Boolean setAttribute (char*& objName, const char* attrName,
				  const ObjectName& value);
    virtual Boolean setAttribute (char*& objName, const char* attrName,
				  const ClassName& value);
    virtual Boolean setAttribute (char*& objName, const char* attrName,
				  const Uid& value);

    virtual Boolean uniqueAttributeName (const char* objName, char*& attrName);
    virtual Boolean uniqueObjectName    (ObjectName& objName);

    virtual Boolean removeAttribute (char*& objName, const char* attrName);

    virtual Boolean attributeType (const char* objName, const char* attrName,
				   ObjectName::AttributeType& attrType);
    
    virtual Boolean firstAttributeName (const char* objName, char*& attrName);
    
    virtual Boolean nextAttributeName (const char* objName,
				       const char* attrName,
				       char*& nextAttrName);

    virtual ostream& print (ostream& strm) const;

protected:
    SNSNameServiceImple ();
    SNSNameServiceImple (SNSAttributeNode*&);
    virtual ~SNSNameServiceImple ();
    
private:
    SNSAttributeNode* findAttribute (const char* attrName, SNSAttribute& attr);
    
    Boolean saveState (char*&);
    Boolean loadState (const char*);

    SNSAttributeNode *root, *current;
    Boolean          initialized, isWindow;

public:
    virtual Boolean objectName (ObjectName&);
    
    static  const ClassName& name ();
    virtual const ClassName& className () const;

    static Resource* create ();

    virtual Resource* control (const ClassName&);

    virtual void* castup (const ClassName&) const;    
    static  SNSNameServiceImple* castup (Resource*);
};

extern ostream& operator<< (ostream& ostr, const SNSNameServiceImple& s);

#endif
