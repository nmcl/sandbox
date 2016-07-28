/*
 * Copyright (C) 1993, 1994
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Mailer.cc,v 1.5 1994/09/02 14:23:05 ndbi Exp $
 */

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef TIME_H_
#  include <System/time.h>
#endif

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef SYS_TYPES_H_
#include <System/sys/types.h>
#endif

#ifndef SYS_TIME_H_
#include <System/sys/time.h>
#endif

#ifndef ERRNO_H_
#  include <System/errno.h>
#endif

#ifndef CONFIGURE_H_
#include <Config/Configure.h>
#endif

#ifndef MAILER_H_
#  include <Mailer.h>
#endif


Mailer::Mailer(ostream& strm)
	      : ofstream(),
		outStrm(strm),
		fileName(0),
		mailProg(MAILER),
		recipient(ADMIN_EMAIL),
		subject("Arjuna Crash Recovery Message")
{
    // Construct file name for tmp file
    fileName = new char[50];
    pid_t pid = getpid();

    sprintf(fileName, "/tmp/%d.msg", pid);
    
    // Open temp file for composing message
    open(fileName);

    // Append initial text to file
    time_t theTime = ::time(0);
    *this << "This message was automatically at " << ctime(&theTime)
	  << endl;
}

Mailer::~Mailer()
{
}

void Mailer::sendMessage()
{
    char mailCommandLine[256];
    FILE *mailPipe;
    
    sprintf(mailCommandLine, "%s -s \"%s\" %s < %s",
	    mailProg, subject, recipient, fileName);
    
    if((mailPipe = popen(mailCommandLine, "w")) != NULL)
	pclose(mailPipe);
    else
	outStrm << "Mailer: *WARNING* Attempt to send message failed" << endl;
}







