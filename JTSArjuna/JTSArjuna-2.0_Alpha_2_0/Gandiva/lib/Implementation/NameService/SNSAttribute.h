/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: SNSAttribute.h,v 1.3 1997/06/02 10:28:24 nmcl Exp $
 */

#ifndef IMPLEMENTATION_NAMESERVICE_SNSATTRIBUTE_H_
#define IMPLEMENTATION_NAMESERVICE_SNSATTRIBUTE_H_

#ifndef COMMON_BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef COMMON_UID_H_
#  include <Common/Uid.h>
#endif

#ifndef COMMON_CLASSNAME_H_
#  include <Common/ClassName.h>
#endif

#ifndef COMMON_OBJECTNAME_H_
#  include <Common/ObjectName.h>
#endif

class istrstream;
class ostrstream;

/*
 * An attribute can be used in either input or output
 * mode, but not both simultaneously.
 */

class SNSAttribute
{
public:
    SNSAttribute ();    
    SNSAttribute (const char* attrName);
    SNSAttribute (const SNSAttribute&);
    virtual ~SNSAttribute ();

    SNSAttribute& operator=  (const char*);
    SNSAttribute& operator=  (const SNSAttribute&);
    Boolean       operator== (const SNSAttribute&) const;
    Boolean       operator!= (const SNSAttribute&) const;

    SNSAttribute& operator>> (long&);
    SNSAttribute& operator>> (unsigned long&);
    SNSAttribute& operator>> (short&);
    SNSAttribute& operator>> (unsigned short&);
    SNSAttribute& operator>> (float&);
    SNSAttribute& operator>> (char*);
    SNSAttribute& operator>> (ClassName&);
    SNSAttribute& operator>> (ObjectName&);
    SNSAttribute& operator>> (Uid&);

    SNSAttribute& operator<< (long);
    SNSAttribute& operator<< (unsigned long);
    SNSAttribute& operator<< (short);
    SNSAttribute& operator<< (unsigned short);
    SNSAttribute& operator<< (float);
    SNSAttribute& operator<< (const char*);
    SNSAttribute& operator<< (const ClassName&);
    SNSAttribute& operator<< (const ObjectName&);
    SNSAttribute& operator<< (const Uid&);

    Boolean operator! () const;
    
    ostream& print (ostream& ostr) const;

    Boolean type (ObjectName::AttributeType&) const;

    char*  stringForm () const;
    size_t length () const;
    
private:
    void copyAttribute (const char*);
    void setOutput ();

    char*       _attribute;
    istrstream* _iparser;
    ostrstream* _oparser;
};

extern ostream& operator<< (ostream& ostr, const SNSAttribute& attr);

#endif
