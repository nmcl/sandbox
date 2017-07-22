#ifndef NODEDATA_H_
#define NODEDATA_H_

#include <sys/param.h>
#include <iostream.h>

#include <InterViews/color.h>
#include <InterViews/bitmap.h>
#include <InterViews/stencil.h>

enum DependencyLevel
{
    NONE, SLIGHT, MINOR,
    AVERAGE, HIGH, DEFINITE
};

class NodeData
{
public:
    NodeData ();
    NodeData (const char*, DependencyLevel);
    ~NodeData ();
    
    void addToList (NodeData*);
    void getMostDependent (DependencyLevel&) const;

    int modify (const char*, DependencyLevel);
    int numberOfDeps () const;

    DependencyLevel getDependency (const char*) const;
    long getTotalDependency () const;

private:
    static char* getFileName (DependencyLevel) const;

public:
    
    static const Color* getColor (DependencyLevel) const;
    static Stencil* getBitmap (DependencyLevel) const;
    static char* getString (DependencyLevel) const;

    virtual ostream& print (ostream&) const;

    char hostName[MAXHOSTNAMELEN];
    DependencyLevel level;
    
    NodeData* next;
};

extern ostream& operator<< (ostream& strm, const NodeData& nd);

#endif
