#include <System/stdio.h>
#include <System/unistd.h>
#include <System/errno.h>

#include "local.h"
#include "Requests.h"
#include "Defs.h"

Boolean IsRegistration()
{
    return (access( REGFILE, F_OK ) == 0 ? TRUE : FALSE );
}

