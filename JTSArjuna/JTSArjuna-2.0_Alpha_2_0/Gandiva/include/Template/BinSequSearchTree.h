/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BinSequSearchTree.h,v 1.2 1996/01/02 13:13:03 nsmw Exp $
 */

#ifndef TEMPLATE_BINSEQUSEARCHTREE_H_
#define TEMPLATE_BINSEQUSEARCHTREE_H_

template <class KeyType,class DataType> struct BinarySequenceSearchTreeElem;

template <class KeyType,class DataType> class BinarySequenceSearchTree
{
public:
    BinarySequenceSearchTree();
    ~BinarySequenceSearchTree();

    void add(KeyType key, DataType *data);
    void remove(KeyType key);

    DataType *find(KeyType key);

private:
    void _add(BinarySequenceSearchTreeElem<KeyType,DataType> *&root, BinarySequenceSearchTreeElem<KeyType,DataType> *bsstElem);
    DataType *_find(BinarySequenceSearchTreeElem<KeyType,DataType> *root, KeyType key);

    BinarySequenceSearchTreeElem<KeyType,DataType> *_root;
};

#ifdef __GNUG__
#   ifndef TEMPLATE_BINSEQUSEARCHTREE_CC_
#       include <Template/BinSequSearchTree.cc>
#   endif
#endif

#endif
