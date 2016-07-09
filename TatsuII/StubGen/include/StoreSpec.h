/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: StoreSpec.h,v 1.1 1997/06/09 19:52:17 nmcl Exp $
 */

#ifndef STORESPEC_H_
#define STORESPEC_H_

#include <iostream>

using namespace std;

/*
 *
 * Enumeration type describing storage classes
 *     
 */

enum StorageSpecifier { SC_AUTO = 1, SC_EXTERN, SC_REGISTER,
                        SC_STATIC, SC_FRIEND, SC_TYPEDEF,
                        SC_OVERLOAD, SC_DEFAULT };


extern ostream& operator<< ( ostream&, StorageSpecifier );

#endif
