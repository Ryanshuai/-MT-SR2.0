#pragma once

#include <mtface/Common/MTDefine.h>
//#ifdef(__linux__)
#include "stddef.h"
//#endif
namespace mtface
{
    // 将vector内存操作封装到内部
    template < class T >
    class MEITU_EXPORT MTVector
    {
    public:
        typedef T                       value_type;
        typedef T&                      reference;
        typedef const T&                const_reference;
        typedef size_t                  size_type;
        typedef ptrdiff_t               difference_type;
        typedef T*                      pointer;
        typedef const T*                const_pointer;
        
    public:
        
        MTVector();
        explicit MTVector(size_type n);
        MTVector(size_type n, const value_type& value);
        MTVector(const MTVector& x);
        MTVector(MTVector&& x);
        ~MTVector();
        
        MTVector& operator=(const MTVector& x);
        MTVector& operator=(MTVector&& x);
        void assign(size_type n, const value_type& u);
        
        size_type size() const;
        size_type max_size() const;
        size_type capacity() const;
        bool empty() const;
        void reserve(size_type n);
        void shrink_to_fit();
        
        reference       operator[](size_type n);
        const_reference operator[](size_type n) const;
        reference       at(size_type n);
        const_reference at(size_type n) const;
        
        reference       front();
        const_reference front() const;
        reference       back();
        const_reference back() const;
        
        void pop_back();
        void push_back(const_reference __x);
        void push_back(value_type&& __x);
        
        value_type*       data();
        const value_type* data() const;
        
        void clear();
        
        void resize(size_type sz);
        void resize(size_type sz, const value_type& c);
        
        void swap(MTVector& x);
        
    private:
        void* val;
    };
}
