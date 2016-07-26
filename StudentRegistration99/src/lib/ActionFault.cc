#include <System/string.h>
#include <System/memory.h>

#include <Common/Boolean.h>

#include "local.h"
#include "Requests.h"
#include "Defs.h"

/*
 * This function creates an Action Fault message and prefixes the given message with FATAL if the
 * flag isFatal is set.
 */

void CreateActionFault( char* Line, char* Why, Boolean isFatal )
{
    char *NextString;

    /* Add the ActionFault command code and blank the transfer id and stud number */
    memcpy( &Line[ 16 ], ACTIONFAULT, 2 );
    memset( &Line[ 18 ], '0', 14 );

    /* If isFatal is TRUE then add the prefix */
    if (isFatal)
	NextString = (char *)memccpy( &Line[ 32 ], FATAL, 0, MAXBUFF ) - 1;
    else
	NextString = &Line[ 32 ];

    /* Append the message to the Output Line */
    NextString = (char *)memccpy( NextString, Why, 0, MAXBUFF ) - 1;

    reportf("Action fault: %s\n", &Line[16]);
}

