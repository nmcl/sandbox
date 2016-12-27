/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BinSequSearchTreeGen.cc,v 1.2 1996/01/02 13:30:05 nsmw Exp $
 */

#include <os/stddef.h>
#include <os/iostream.h>

#ifndef TEMPLATE_BINSEQUSEARCHTREE_H_
#   include <Template/BinSequSearchTreeGen.h>
#endif

struct BinarySequenceSearchTreeElemGen
{
    KeyType                         _key;
    DataType                        *_data;
    BinarySequenceSearchTreeElemGen *_lessThan;
    BinarySequenceSearchTreeElemGen *_greaterThan;
};

BinarySequenceSearchTreeGen::BinarySequenceSearchTreeGen()
{
    _root = NULL;
}

BinarySequenceSearchTreeGen::~BinarySequenceSearchTreeGen()
{
}

void BinarySequenceSearchTreeGen::add(KeyType key, DataType *data)
{
    BinarySequenceSearchTreeElemGen *bsstElem;

    bsstElem = new BinarySequenceSearchTreeElemGen;

    bsstElem->_key         = key;
    bsstElem->_data        = data;
    bsstElem->_lessThan    = NULL;
    bsstElem->_greaterThan = NULL;

    _add(_root, bsstElem);
}

void BinarySequenceSearchTreeGen::_add(BinarySequenceSearchTreeElemGen *&root, BinarySequenceSearchTreeElemGen *bsstElem)
{
    if (root == NULL)
        root = bsstElem;
    else
    {
        if (! (root->_key == bsstElem->_key))
	{
            if (root->_key > bsstElem->_key)
                _add(root->_lessThan, bsstElem);
            else
                _add(root->_greaterThan, bsstElem);
	}
        else
            cerr << "BinarySequenceSearchTreeGen::_add : Duplicate key!" << endl;
    }
}

void BinarySequenceSearchTreeGen::remove(KeyType key)
{
}

DataType *BinarySequenceSearchTreeGen::find(KeyType key)
{
    if (_root == NULL)
        return NULL;
    else
        return _find(_root, key);
}

void *BinarySequenceSearchTreeGen::_find(BinarySequenceSearchTreeElemGen *root, KeyType key)
{
    if (root == NULL)
        return NULL;
    else
    {
        if (root->_key == key)
            return root->_data;
        else
	{
            if (root->_key > key)
                return _find(root->_lessThan, key);
            else
                return _find(root->_greaterThan, key);
	}
    }
}
