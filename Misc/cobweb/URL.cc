#include "URL.h"
#include <iostream.h>
#include <string.h>

URL::URL ()
	 : accessLength(0), access(0),
	   hostLength(0), host(0),
	   absoluteLength(0), absolute(0),
	   relativeLength(0), relative(0),
	   anchorLength(0), anchor(0)
{
}

URL::~URL()
{
    if (access)
    {
	delete(access);
	access = 0;
	accessLength = 0;
    }
    
    if (host)
    {
	delete(host);
	host = 0;
	hostLength = 0;
    }
    
    if (absolute)
    {
	delete(absolute);
	absolute = 0;
	absoluteLength = 0;
    }
    
    if (relative)
    {
	delete(relative);
	relative = 0;
	relativeLength = 0;
    }
    
    if (anchor)
    {
	delete(anchor);
	anchor = 0;
	anchorLength = 0;
    }
}

void URL::parse(const char *urlToParse)
{
    int length = strlen(urlToParse);
    char *urlString = new char[length + 1];
    memset(urlString, '\0', length + 1);
    memcpy(urlString, urlToParse, length);
    
    char *p;
    char *after_access = urlString;

    // Access protocol
    for (p = urlString; *p; p++)
    {
	if (*p == ':')
	{
	    *p = 0;
	    accessLength = strlen(urlString);
	    access = new char[accessLength + 1];
	    memset(access, '\0', accessLength + 1);
	    memcpy(access, urlString, accessLength);
	    after_access = p+1;
	}
	
	if (*p == '/')
	    break;

	if (*p == '#')
	    break;
    }

    // Anchor
    for (p = urlString + length - 1; p >= urlString; p--)
    {
	if (*p == '#')
	{
	    anchorLength = strlen(p+1);
	    if (anchorLength > 0)
	    {
		anchor = new char[anchorLength + 1];
		memset(anchor, '\0', anchorLength + 1);
		memcpy(anchor, p+1, anchorLength);
		*p = 0;
	    }
	}
    }
    

    // Host
    p = after_access;
    if (*p == '/')
    {
	if (p[1]=='/')
	{
	    // We detected "//" so we have the start of a host name
	    char *hostStart = p+2;
	    char *hostEnd = 0;

	    char *hostTerminator = strchr(hostStart, '/');

	    if (!hostTerminator)
	    {
		// we have a url like "http://arjuna"
		hostEnd = hostStart + strlen(hostStart);
	    }
	    else
	    {
		// we have a url like "http://arjuna/" or http://arjuna/blah"
		hostEnd = hostTerminator;
		absoluteLength = strlen(hostTerminator + 1);

		if (absoluteLength)
		{
		    absolute = new char[absoluteLength + 1];
		    memset(absolute, '\0', absoluteLength + 1);
		    memcpy(absolute, hostTerminator + 1, absoluteLength);
		}
	    }

	    hostLength = hostEnd - hostStart;
	    
	    if (hostLength)
	    {
		host = new char[hostLength + 1];
		memset(host, '\0', hostLength + 1);
		memcpy(host, hostStart, hostLength);
	    }
	    
	}
	else
	{
	    // just path, no host
	    absoluteLength = strlen(p+1);
	    if (absoluteLength)
	    {
		absolute = new char[absoluteLength + 1];
		memset(absolute, '\0', absoluteLength + 1);
		memcpy(absolute, p + 1, absoluteLength);
	    }
	}
    }
    else
    {
	if (after_access)
	{
	    // we do have something after the access prot
	    relativeLength = strlen(after_access);
	    if (relativeLength)
	    {
		relative = new char[relativeLength + 1];
		memset(relative, '\0', relativeLength + 1);
		memcpy(relative, after_access, relativeLength);
	    }
	}
    }


if (urlString)
    {
	delete(urlString);
	urlString = 0;
    }
}

ostream& URL::print (ostream& strm) const
{
     if (access)
	strm << access << ":";

    if (host)
	strm << "//" << host;

    if (absolute)
	strm << "/" << absolute;

    if (relative)
	strm << relative;

    if (anchor)
	strm << "#" << anchor;

     return strm;
     
}

ostream& URL::printInternal (ostream& strm) const
{
    strm << "\nURL State:" << endl;

    if (access)
	strm << "\taccess: " << accessLength << " <" << access << ">" << endl;

    if (host)
	strm << "\thost: " << hostLength << " <" << host << ">" << endl;

    if (absolute)
	strm << "\tabsolute: " << absoluteLength << " <" << absolute << ">"
	     << endl;

    if (relative)
	strm << "\trelative: " << relativeLength << " <" << relative << ">"
	     << endl;

    if (anchor)
	strm << "\tanchor: " << anchorLength << " <" << anchor << ">" << endl;

    return strm;
}

ostream& operator<< (ostream& strm, const URL& u)
{
    return u.print(strm);
}
