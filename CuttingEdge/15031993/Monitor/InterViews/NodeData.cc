#ifndef NODEDATA_H_
#include "NodeData.h"
#endif

#include <memory.h>
#include <string.h>

#include <InterViews/session.h>
#include <InterViews/display.h>

NodeData::NodeData ()
                    : level(NONE),
		      next(0)
{
    ::memset(hostName, '\0', MAXHOSTNAMELEN);
}

NodeData::NodeData (const char* name, DependencyLevel temp)
                                                          : next(0)
{
    ::memset(hostName, '\0', MAXHOSTNAMELEN);
    ::memcpy(hostName, name, strlen(name));
    level = temp;
}

NodeData::~NodeData () {}

void NodeData::addToList (NodeData* toAdd)
{
    if (toAdd == this)
	return;
    
    if (next)
	next->addToList(toAdd);
    else
	next = toAdd;
}

int NodeData::modify (const char* name, DependencyLevel temp)
{
    int result = 0;
    
    if (strcmp(hostName, name) == 0)
    {
	level = temp;
	result = 1;
    }
    else
	if (next)
	    result = next->modify(name, temp);
    
    if (!result)
    {
	NodeData* p = new NodeData(name, temp);
	addToList(p);
	result = 1;
    }
    
    return result;
}

void NodeData::getMostDependent (DependencyLevel& temp) const
{
    if (level > temp)
	temp = level;
    
    if (next)
	next->getMostDependent(temp);
}

long NodeData::getTotalDependency () const
{
    if (next)
	return level + next->getTotalDependency();
    else
	return level;
}

DependencyLevel NodeData::getDependency (const char* name) const
{
    DependencyLevel temp = NONE;
    
    if (strcmp(hostName, name) == 0)
	return level;
    else
    {
	if (next)
	    return next->getDependency(name);
	else
	    return temp;
    }
}

int NodeData::numberOfDeps () const
{
    int number = 0;
    const NodeData* ptr = this;
    
    while (ptr)
    {
	number++;
	ptr = ptr->next;
    }
    
    return number;
}

const Color* NodeData::getColor (DependencyLevel temp) const
{
    float r = ((float)temp/(float)DEFINITE)*1.0, g = 0.0, b = 1.0-r;
    return new Color(r, g, b);
}

char* NodeData::getFileName (DependencyLevel temp) const
{
    char* fileName = 0;

    switch (temp)
    {
    case NONE:
	fileName = "none";
	break;
    case SLIGHT:
	fileName = "slight";
	break;
    case MINOR:
	fileName = "minor";
	break;
    case AVERAGE:
	fileName = "average";
	break;
    case HIGH:
	fileName = "high";
	break;
    case DEFINITE:
	fileName = "definite";
	break;
    }

    return fileName;
}

Stencil* NodeData::getBitmap (DependencyLevel temp) const
{
    Bitmap *bm = Bitmap::open(NodeData::getFileName(temp));
    Stencil *pict = new Stencil(bm, NodeData::getColor(temp));
    
    return pict;
}

char* NodeData::getString (DependencyLevel temp) const
{
    return NodeData::getFileName(temp);
}

ostream& NodeData::print (ostream& strm) const
{
    strm << "node : " << hostName << endl;

    strm << "dependency : ";
    strm << NodeData::getFileName(level);

    if (next)
	strm << *next;
    else
	strm << "\nEnd of list" << endl;
    
    return strm;
}

ostream& operator<< (ostream& strm, const NodeData& nd)
{   
    return nd.print(strm);
}
