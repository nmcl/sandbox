/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


#ifndef SVRERRCODE_H_
#define SVRERRCODE_H_

//  @NoRemote, @NoMarshall
enum ServerErrCode
{
    OPER_INVOKED_OK, DISPATCH_ERROR, PARAMETER_ERROR, ALREADY_CONSTRUCTED,
    EXCEPTION_RAISED, UNEXPECTED_EXCEPTION
};

#endif
