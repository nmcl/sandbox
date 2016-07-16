/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: StoreSpec.h,v 1.1 1997/09/25 15:30:24 nmcl Exp $
 */

#ifndef STORESPEC_H_
#define STORESPEC_H_

/*
 *
 * Enumeration type describing storage classes
 *     
 */

#include <iostream>

using namespace std;


enum StorageSpecifier { SC_AUTO = 1, SC_EXTERN, SC_REGISTER,
                        SC_STATIC, SC_FRIEND, SC_TYPEDEF,
                        SC_OVERLOAD, SC_DEFAULT };


extern ostream& operator<< ( ostream&, StorageSpecifier );

#endif
