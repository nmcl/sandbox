/*
 * Copyright (c) 1987, 1988, 1989, 1990, 1991 Stanford University
 * Copyright (c) 1991 Silicon Graphics, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and 
 * its documentation for any purpose is hereby granted without fee, provided
 * that (i) the above copyright notices and this permission notice appear in
 * all copies of the software and related documentation, and (ii) the names of
 * Stanford and Silicon Graphics may not be used in any advertising or
 * publicity relating to the software without the specific, prior written
 * permission of Stanford and Silicon Graphics.
 * 
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  
 *
 * IN NO EVENT SHALL STANFORD OR SILICON GRAPHICS BE LIABLE FOR
 * ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,
 * OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF 
 * LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE 
 * OF THIS SOFTWARE.
 */

/*
 * Generic list implemented as dynamic array
 */

#ifndef list_h
#define list_h

#include <InterViews/boolean.h>
#include <generic.h>

#define declareList(List,T) \
class List { \
public: \
    List(int size = 0); \
    ~List(); \
\
    int count(); \
    T& item(int index); \
    T* array(int index, int count); \
\
    void prepend(T&); \
    void append(T&); \
    void insert(int index, T&); \
    void remove(int index); \
    void remove_all(); \
private: \
    T* items_; \
    int size_; \
    int count_; \
    int free_; \
}; \
\
inline int List::count() { return count_; } \
\
inline T& List::item(int index) { \
    register int i = index < free_ ? index : index + size_ - count_; \
    return items_[i]; \
} \
\
inline void List::append(T& item) { insert(count_, item); } \
inline void List::prepend(T& item) { insert(0, item); }

/*
 * List implementation
 */

extern int best_new_count(int count, int size);

#define implementList(List,T) \
List::List(int size) { \
    if (size > 0) { \
        size_ = best_new_count(size, sizeof(T)); \
        items_ = new T[size_]; \
    } else { \
        size_ = 0; \
        items_ = 0; \
    } \
    count_ = 0; \
    free_ = 0; \
} \
\
List::~List() { \
    delete [] items_; \
} \
\
T* List::array(int index, int count) { \
    if (index + count <= free_) { \
        return items_ + index; \
    } else if (index >= free_) { \
        return items_ + index + size_ - count_; \
    } else { \
        int i; \
        for (i = 0; i < index + count - free_; ++i) { \
            items_[free_ + i] = items_[free_ + size_ - count_ + i]; \
        } \
        free_ = index + count; \
    } \
    return items_ + index; \
} \
\
void List::insert(int index, T& item) { \
    if (count_ == size_) { \
        int size = best_new_count(size_ + 1, sizeof(T)); \
        T* items = new T[size]; \
        if (items_ != 0) { \
            register int i; \
            for (i = 0; i < free_; ++i) { \
                items[i] = items_[i]; \
            } \
            for (i = 0; i < count_ - free_; ++i) { \
                items[free_ + size - count_ + i] = \
                    items_[free_ + size_ - count_ + i]; \
            } \
            delete [] items_; \
        } \
        items_ = items; \
        size_ = size; \
    } \
    if (index >= 0 && index <= count_) { \
	if (index < free_) { \
            for (register int i = free_ - index - 1; i >= 0; --i) { \
                items_[index + size_ - count_ + i] = items_[index + i]; \
            } \
        } else if (index > free_) { \
            for (register int i = 0; i < index - free_; ++i) { \
                items_[free_ + i] = items_[free_ + size_ - count_ + i]; \
            } \
        } \
        free_ = index + 1; \
        count_ += 1; \
        items_[index] = item; \
    } \
} \
\
void List::remove(int index) { \
    if (index >= 0 && index <= count_) { \
        if (index < free_) { \
            for (register int i = free_ - index - 2; i >= 0; --i) { \
                items_[size_ - count_ + index + 1 + i] = \
		    items_[index + 1 + i]; \
            } \
        } else if (index > free_) { \
            for (register int i = 0; i < index - free_; ++i) { \
                items_[free_ + i] = items_[free_ + size_ - count_ + i]; \
            } \
        } \
        free_ = index; \
        count_ -= 1; \
    } \
} \
\
void List::remove_all() { \
    count_ = 0; \
    free_ = 0; \
}

#endif
