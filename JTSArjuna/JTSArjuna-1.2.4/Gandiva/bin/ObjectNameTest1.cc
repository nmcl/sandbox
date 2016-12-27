/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ObjectNameTest1.cc,v 1.3 1997/01/27 17:22:39 nsmw Exp $
 */

#include <os/stdlib.h>
#include <os/iostream.h>

#ifndef COMMON_UID_H_
#  include <Common/Uid.h>
#endif

#ifndef COMMON_OBJECTNAME_H_
#  include <Common/ObjectName.h>
#endif

#ifndef COMMON_CLASSNAME_H_
#  include <Common/ClassName.h>
#endif


int main (int, char**)
{
    ObjectName objName("DummyName:myName");
    char* firstName = (char*) 0;
    long l_value = -1;
    unsigned long ul_value = 0;
    char* c_value = (char*) 0;
    ObjectName on_value(NULL);
    ClassName cn_value(NULL);
    Uid u_value(Uid::invalid());

    if (objName.firstAttributeName(firstName))
    {
	cout << "First attribute name is " << firstName << endl;
	cout << "This is an error!" << endl;
    }
    else
    {
	cout << "firstAttributeName error." << endl;
	cout << "This is the correct result!" << endl;
    }

    if (objName.getAttribute(NULL, l_value))
	cout << "getAttribute::long error" << endl;
    if (objName.getAttribute(NULL, ul_value))
	cout << "getAttribute::unsigned long error" << endl;
    if (objName.getAttribute(NULL, c_value))
	cout << "getAttribute::char* error" << endl;
    if (objName.getAttribute(NULL, on_value))
	cout << "getAttribute::ObjectName error" << endl;
    if (objName.getAttribute(NULL, cn_value))
	cout << "getAttribute::ClassName error" << endl;
    if (objName.getAttribute(NULL, u_value))
	cout << "getAttribute::Uid error" << endl;

    if (objName.setAttribute(NULL, l_value))
	cout << "setAttribute::long error" << endl;
    if (objName.setAttribute(NULL, ul_value))
	cout << "setAttribute::unsigned long error" << endl;    
    if (objName.setAttribute(NULL, c_value))
	cout << "setAttribute::char* error" << endl;
    if (objName.setAttribute(NULL, on_value))
	cout << "setAttribute::ObjectName error" << endl;
    if (objName.setAttribute(NULL, cn_value))
	cout << "setAttribute::ClassName error" << endl;
    if (objName.setAttribute(NULL, u_value))
	cout << "setAttribute::Uid error" << endl;
    
    ObjectName theCopy(NULL);
    
    cout << "\nobjName : " << objName << endl;
    cout << "theCopy : " << theCopy << endl;

    cout << "Copying objName to theCopy." << endl;
    theCopy = objName;

    cout << "theCopy : " << theCopy << endl;

    return 0;
}
