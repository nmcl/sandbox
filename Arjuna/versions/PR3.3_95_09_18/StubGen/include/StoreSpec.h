/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: StoreSpec.h,v 1.1 1993/11/03 14:45:47 nmcl Exp $
 */

#ifndef STORESPEC_H_
#define STORESPEC_H_

/*
 *
 * Enumeration type describing storage classes
 *     
 */


class ostream;

enum StorageSpecifier { SC_AUTO = 1, SC_EXTERN, SC_REGISTER,
                        SC_STATIC, SC_FRIEND, SC_TYPEDEF,
                        SC_OVERLOAD, SC_DEFAULT };


extern ostream& operator<< ( ostream&, StorageSpecifier );

#endif
