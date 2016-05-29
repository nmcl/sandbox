/*
 * Copyright (C) 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BloomFilterNameService.cc,v 1.3 1998/07/23 10:47:55 nmcl Exp $
 */

#include <iostream.h>
#include <string.h>

#ifndef BLOOMFILTER_H_
#  include "BloomFilter.h"
#endif

#ifndef BLOOMFILTERNAMESERVICE_H_
#  include "BloomFilterNameService.h"
#endif

class FilterContext
{
public:
    FilterContext ();
    ~FilterContext ();

    BloomFilter* _theFilter;
    char* _ior;
};

FilterContext::FilterContext ()
			     : _theFilter(NULL),
			       _ior(NULL)
{
}

FilterContext::~FilterContext ()
{
    if (_theFilter != NULL)
	delete _theFilter;

    if (_ior != NULL)
	::delete [] _ior;
}


BloomFilterNameService::BloomFilterNameService (long nodes, long vectorSize,
						int numberOfHash)
					       : _filters(0),
						 _size(nodes),
						 _vectorSize(vectorSize),
						 _numberOfHash(numberOfHash)
{
    if (_size > 0)
    {
	_filters = new FilterContext* [_size];

	for (int i = 0; i < _size; i++)
	    _filters[i] = NULL;
    }
}

BloomFilterNameService::~BloomFilterNameService ()
{
}

ostream& BloomFilterNameService::print (ostream& strm) const
{
    if (_filters == NULL)
	strm << "Empty";
    else
    {
	for (int i = 0; i < _size; i++)
	{
	    strm << "Bloom filter: " << i;
	    
	    if (_filters[i] && _filters[i]->_theFilter)
		strm << "\n" << *_filters[i]->_theFilter << endl;
	    else
		strm << "empty." << endl;
	}
    }

    return strm;
}

void BloomFilterNameService::reset ()
{
    if ((_size > 0) && (_filters != NULL))
    {
	long i;
	
	for (i = 0; i < _size; i++)
	    if (_filters[i] != NULL)
		delete _filters[i];

	delete [] _filters;

	_filters = new FilterContext* [_size];

	for (i = 0; i < _size; i++)
	    _filters[i] = NULL;
    }
}

Boolean BloomFilterNameService::addDomain (const char* ior, int& domainNumber)
{
    for (int i = 0; i < _size; i++)
    {
	if (_filters[i] == NULL)
	    _filters[i] = new FilterContext;
	    
	if (_filters[i]->_theFilter == NULL)
	{
	    _filters[i]->_theFilter = new BloomFilter(_vectorSize, _numberOfHash);
	    _filters[i]->_ior = ::new char[::strlen(ior)+1];
	    ::strcpy(_filters[i]->_ior, ior);
	    
	    domainNumber = i;

	    return TRUE;
	}
    }

    return FALSE;
}

Boolean BloomFilterNameService::removeDomain (int domainNumber)
{
    if ((domainNumber < 0) || (domainNumber >= _size))
	return FALSE;
    else
    {
	if (_filters[domainNumber] == NULL)
	    return FALSE;
	
	if (_filters[domainNumber]->_theFilter)
	{
	    delete _filters[domainNumber]->_theFilter;
	    _filters[domainNumber]->_theFilter = NULL;
	}
	
	if (_filters[domainNumber]->_ior)
	{
	    ::delete [] _filters[domainNumber]->_ior;
	    _filters[domainNumber]->_ior = NULL;
	}

	return TRUE;
    }
}

Boolean BloomFilterNameService::insert (const Uid& id, int domainNumber)
{
    if ((domainNumber < 0) || (domainNumber >= _size))
	return FALSE;

    if (_filters[domainNumber] == NULL)
	return FALSE;
	    
    if (_filters[domainNumber]->_theFilter != NULL)
    {
	_filters[domainNumber]->_theFilter->insert(id);
	return TRUE;
    }
    else
	return FALSE;
}

Boolean BloomFilterNameService::present (const Uid& objId) const
{
    for (int i = 0; i < _size; i++)
    {
	if (_filters[i] != NULL)
	{
	    if (_filters[i]->_theFilter->present(objId))
		return TRUE;
	}
    }

    return FALSE;
}

char** BloomFilterNameService::locate (const Uid& objId, int& size) const
{
    char** toReturn = ::new char* [_size];  // assume max size
    long index = 0;
    
    size = 0;

    for (int i = 0; i < _size; i++)
    {
	if (_filters[i] != NULL)
	{
	    if (_filters[i]->_theFilter->present(objId))
	    {
		toReturn[index] = ::new char[::strlen(_filters[i]->_ior)+1];
		::strcpy(toReturn[index], _filters[i]->_ior);
		size++;
	    }
	}
    }

    if (size == 0)
    {
	::delete [] toReturn;
	toReturn = 0;
    }

    return toReturn;
}

ostream& operator<< (ostream& strm, const BloomFilterNameService& b)
{
    return b.print(strm);
}
