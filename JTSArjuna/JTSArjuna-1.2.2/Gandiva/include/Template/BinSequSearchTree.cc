/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BinSequSearchTree.cc,v 1.2 1996/01/02 13:13:02 nsmw Exp $
 */

#ifndef TEMPLATE_BINSEQUSEARCHTREE_CC_
#define TEMPLATE_BINSEQUSEARCHTREE_CC_

#ifndef OS_STDDEF_H_
#   include <os/stddef.h>
#endif

#ifndef OS_IOSTREAM_H_
#   include <os/iostream.h>
#endif

#ifndef TEMPLATE_BINSEQUSEARCHTREE_H_
#   include <Template/BinSequSearchTree.h>
#endif

template <class KeyType,class DataType> struct BinarySequenceSearchTreeElem
{
    KeyType                      _key;
    DataType                     *_data;
    BinarySequenceSearchTreeElem *_lessThan;
    BinarySequenceSearchTreeElem *_greaterThan;
};

template <class KeyType,class DataType> BinarySequenceSearchTree<KeyType,DataType>::BinarySequenceSearchTree()
{
    _root = NULL;
}

template <class KeyType,class DataType> BinarySequenceSearchTree<KeyType,DataType>::~BinarySequenceSearchTree()
{
}

template <class KeyType,class DataType> void BinarySequenceSearchTree<KeyType,DataType>::add(KeyType key, DataType *data)
{
    BinarySequenceSearchTreeElem<KeyType,DataType> *bsstElem;

    bsstElem = new BinarySequenceSearchTreeElem<KeyType,DataType>;

    bsstElem->_key         = key;
    bsstElem->_data        = data;
    bsstElem->_lessThan    = NULL;
    bsstElem->_greaterThan = NULL;

    _add(_root, bsstElem);
}

template <class KeyType,class DataType> void BinarySequenceSearchTree<KeyType,DataType>::_add(BinarySequenceSearchTreeElem<KeyType,DataType> *&root, BinarySequenceSearchTreeElem<KeyType,DataType> *bsstElem)
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
            cerr << "BinarySequenceSearchTree::_add : Duplicate key!" << endl;
    }
}

template <class KeyType,class DataType> void BinarySequenceSearchTree<KeyType,DataType>::remove(KeyType key)
{
    cerr << "BinarySequenceSearchTree::remove : Not Implemented!" << endl;
}

template <class KeyType,class DataType> DataType *BinarySequenceSearchTree<KeyType,DataType>::find(KeyType key)
{
    if (_root == NULL)
        return NULL;
    else
        return _find(_root, key);
}

template <class KeyType,class DataType> DataType *BinarySequenceSearchTree<KeyType,DataType>::_find(BinarySequenceSearchTreeElem<KeyType,DataType> *root, KeyType key)
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

#endif
