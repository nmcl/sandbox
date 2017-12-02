/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: typetokey.cc,v 1.3 1994/09/20 11:59:56 nmcl Exp $
 */


#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef UTILITY_H_
#  include <Common/Utility.h>
#endif

int main (int argc, char** argv)
{

    for (int i = 1; i < argc; i++)
		cout << argv[i] << ": " << typeToKey(argv[i]) << endl;

	return 0;
}
