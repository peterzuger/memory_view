/**
 * @file   memory_view/include/memory_view.hpp
 * @author Peter Züger
 * @date   28.05.2019
 * @brief  python memoryview in C++17
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Peter Züger
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef MEMORY_VIEW_HPP
#define MEMORY_VIEW_HPP

#include <algorithm>
#include <iterator>
#include <limits>
#include <stdexcept>
#include <exception>

namespace memory_view{
    namespace impl{
        [[noreturn]] inline void throw_out_of_range(const char* s){
#if defined(__cpp_exceptions)
            throw std::out_of_range(s);
#else
            (void)s;
            std::terminate();
#endif /* defined(__cpp_exceptions) */
        }
    }

    template<typename T>
    class memory_view{
        const T*    _data;
        std::size_t _size;

    public:
        // types:
        using value_type             = T;
        using size_type              = std::size_t;
        using difference_type        = std::ptrdiff_t;
        using pointer                = value_type*;
        using const_pointer          = const value_type*;
        using reference              = value_type&;
        using const_reference        = const value_type&;
        using iterator               = pointer;
        using const_iterator         = const_pointer;
        using reverse_iterator       = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        static const size_type npos  = -1;

        // copy constructor
        constexpr memory_view(const memory_view& other):
            _data{other.data()},
            _size{other.size()}{}

        // move constructor
        constexpr memory_view(memory_view&& other):
            _data{other.data()},
            _size{other.size()}{
                other._data = nullptr;
                other._size = 0;
            }

        // construct from compiletime array type
        template<std::size_t N>
        constexpr memory_view(const value_type arr[N]):
            _data{arr},
            _size{N}{}

        // construct from pointer and size
        constexpr memory_view(const_pointer begin, size_type size):
            _data{begin},
            _size{size}{}

        // construct from begin and end pointer
        constexpr memory_view(const_pointer begin, const_pointer end):
            _data{begin},
            _size{end - begin}{}

        // construct from address and size
        explicit constexpr memory_view(size_type address, size_t size):
            _data{reinterpret_cast<const_pointer>(address)},
            _size{size}{}

        // copy assignment
        memory_view& operator=(const memory_view& other)noexcept{
            _data = other._data;
            _size = other._size;
            return *this;
        }

        // move assignment
        memory_view& operator=(memory_view&& other)noexcept{
            _data = other._data;
            _size = other._size;
            other._data = nullptr;
            other._size = 0;
            return *this;
        }

        void swap(memory_view& other)noexcept{
            using std::swap;
            swap(_data, other._data);
            swap(_size, other._size);
        }

        // iterators:
        constexpr iterator begin()noexcept{
            return iterator(data());
        }
        constexpr const_iterator begin()const noexcept{
            return const_iterator(data());
        }
        constexpr iterator end()noexcept{
            return iterator(data()+size());
        }
        constexpr const_iterator end()const noexcept{
            return const_iterator(data()+size());
        }

        // reverse iterators
        constexpr reverse_iterator rbegin()noexcept{
            return reverse_iterator(end());
        }
        constexpr const_reverse_iterator rbegin()const noexcept{
            return const_reverse_iterator(end());
        }
        constexpr reverse_iterator rend()noexcept{
            return reverse_iterator(begin());
        }
        constexpr const_reverse_iterator rend()const noexcept{
            return const_reverse_iterator(begin());
        }

        // const iterators
        constexpr const_iterator cbegin()const noexcept{
            return begin();
        }
        constexpr const_iterator cend()const noexcept{
            return end();
        }
        constexpr const_reverse_iterator crbegin()const noexcept{
            return rbegin();
        }
        constexpr const_reverse_iterator crend()const noexcept{
            return rend();
        }

        // capacity:
        constexpr bool empty()const noexcept{
            return size() == 0;
        }
        constexpr size_type size()const noexcept{
            return size;
        }
        constexpr size_type max_size()const noexcept{
            return std::numeric_limits<std::size_t>::max();
        }

        // element access:
        constexpr reference operator[](size_type n)noexcept{
            return _data[n];
        }
        constexpr const_reference operator[](size_type n)const noexcept{
            return _data[n];
        }
        constexpr reference at(size_type n){
            if(n >= size())
                impl::throw_out_of_range("memory_view::at");
            return _data[n];
        }
        constexpr const_reference at(size_type n)const{
            if(n >= size())
                impl::throw_out_of_range("memory_view::at");
            return _data[n];
        }

        constexpr reference front()noexcept{
            return _data[0];
        }
        constexpr const_reference front()const noexcept{
            return _data[0];
        }
        constexpr reference back()noexcept{
            return _data[size()-1];
        }
        constexpr const_reference back()const noexcept{
            return _data[size()-1];
        }

        constexpr pointer data()noexcept{
            return _data;
        }
        constexpr const_pointer data()const noexcept{
            return _data;
        }

        constexpr void remove_prefix(size_type n)noexcept{
            _data += n;
        }
        constexpr void remove_suffix(size_type n)noexcept{
            _size -= n;
        }

        constexpr memory_view view(size_type pos = 0, size_type count = npos)const{
            if(pos >= size())
                impl::throw_out_of_range("memory_view::view");
            return memory_view(data() + pos, std::min(count, size() - pos));
        }
    };

    template<class T>
    constexpr bool operator==(memory_view<T> lhs, memory_view<T> rhs)noexcept{
        if(!(lhs.size() == rhs.size()))
            return false;
        for(std::size_t i = 0; i < lhs.size(); i++)
            if(!(lhs[i] == rhs[i]))
                return false;
        return true;
    }
    template<class T>
    constexpr bool operator!=(memory_view<T> lhs, memory_view<T> rhs)noexcept{
        return !(lhs == rhs);
    }

    template<class T>
    constexpr bool operator< (memory_view<T> lhs, memory_view<T> rhs)noexcept{
        return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin, rhs.end());
    }
    template<class T>
    constexpr bool operator> (memory_view<T> lhs, memory_view<T> rhs)noexcept{
        return rhs < lhs;
    }
    template<class T>
    constexpr bool operator<=(memory_view<T> lhs, memory_view<T> rhs)noexcept{
        return !(rhs < lhs);
    }
    template<class T>
    constexpr bool operator>=(memory_view<T> lhs, memory_view<T> rhs)noexcept{
        return !(lhs < rhs);
    }
}

#endif /* MEMORY_VIEW_HPP */
