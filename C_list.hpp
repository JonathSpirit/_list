/*
 * MIT License
 * Copyright (c) 2024 Guillaume Guillet
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include <cstdlib>
#include <cstdint>
#include <limits>
#include <utility>
#include <type_traits>
#include <iterator>

namespace gg
{

template<class T, class TBlockSize=uint16_t>
class List
{
    static_assert(std::is_fundamental_v<TBlockSize> && std::is_unsigned_v<TBlockSize>, "TBlockSize must be fundamental and unsigned type !");

    enum class Directions
    {
        FRONT,
        BACK
    };

    struct DataLocation
    {
        T* _data{nullptr};
        TBlockSize _position{0};
    };
    struct Block
    {
        Block* _lastBlock{nullptr};
        uint8_t _data[sizeof(T)*sizeof(TBlockSize)*8]{};
        Block* _nextBlock{nullptr};
        TBlockSize _occupiedFlags{0};
    };
    using BlockIndex = unsigned short;

    constexpr static BlockIndex gIndexLast = sizeof(TBlockSize) * 8 - 1;
    constexpr static BlockIndex gIndexMid = sizeof(TBlockSize) * 8 / 2;
    constexpr static TBlockSize gPositionFirst = 1;
    constexpr static TBlockSize gPositionLast = static_cast<TBlockSize>(1) << gIndexLast;
    constexpr static TBlockSize gPositionMid = static_cast<TBlockSize>(1) << gIndexMid;

    class base_iterator
    {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = T;
        using pointer           = value_type*;
        using const_pointer     = value_type const*;
        using reference         = value_type&;
        using const_reference   = value_type const&;

        constexpr explicit base_iterator(Block* block, DataLocation const& dataLocation);
        constexpr explicit base_iterator(Block* block);
        constexpr base_iterator() = default;

        constexpr void operator--();
        constexpr void operator++();

        [[nodiscard]] constexpr bool operator==(const base_iterator& r) const;
        [[nodiscard]] constexpr bool operator!=(const base_iterator& r) const;

    private:
        Block* _block{nullptr};
        DataLocation _dataLocation;
        friend List;
    };

public:
    using value_type = T;

    class const_iterator;

    class iterator : public base_iterator
    {
    public:
        using base_iterator::base_iterator;

        constexpr iterator& operator--() {base_iterator::operator--(); return *this;}
        constexpr iterator& operator++() {base_iterator::operator++(); return *this;}

        constexpr iterator operator--(int) {auto copy=*this; base_iterator::operator--(); return copy;}
        constexpr iterator operator++(int) {auto copy=*this; base_iterator::operator++(); return copy;}

        [[nodiscard]] constexpr typename base_iterator::reference operator*() const;
        [[nodiscard]] constexpr typename base_iterator::pointer operator->() const;

    private:
        constexpr iterator(const_iterator const& it) : base_iterator(it) {}
        friend List;
    };

    class const_iterator : public base_iterator
    {
    public:
        using base_iterator::base_iterator;

        constexpr const_iterator(iterator const& it) : base_iterator(it) {}

        constexpr const_iterator& operator--() {base_iterator::operator--(); return *this;}
        constexpr const_iterator& operator++() {base_iterator::operator++(); return *this;}

        constexpr const_iterator operator--(int) {auto copy=*this; base_iterator::operator--(); return copy;}
        constexpr const_iterator operator++(int) {auto copy=*this; base_iterator::operator++(); return copy;}

        [[nodiscard]] constexpr typename base_iterator::const_reference operator*() const;
        [[nodiscard]] constexpr typename base_iterator::const_pointer operator->() const;

    private:
        friend List;
    };

    constexpr List();
    template<class TInputIt>
    constexpr List(TInputIt first, TInputIt last);
    constexpr List(List const& r);
    constexpr List(List&& r) noexcept;
    constexpr explicit List(std::size_t size);
    ~List();

    constexpr List& operator=(List const& r);
    constexpr List& operator=(List&& r) noexcept;

    constexpr void clear();

    template<class U>
    constexpr void push_back(U&& value);
    template<class... TArgs>
    constexpr T& emplace_back(TArgs&&... value);
    template<class U>
    constexpr void push_front(U&& value);
    template<class... TArgs>
    constexpr T& emplace_front(TArgs&&... value);

    constexpr void pop_back();
    constexpr void pop_front();

    constexpr iterator erase(const_iterator const& pos);
    template<class U>
    constexpr iterator insert(const_iterator pos, U&& value);

    [[nodiscard]] constexpr std::size_t size() const noexcept;
    [[nodiscard]] constexpr bool empty() const noexcept;

    [[nodiscard]] constexpr iterator begin();
    [[nodiscard]] constexpr iterator end();

    [[nodiscard]] constexpr const_iterator begin() const;
    [[nodiscard]] constexpr const_iterator cbegin() const;
    [[nodiscard]] constexpr const_iterator end() const;
    [[nodiscard]] constexpr const_iterator cend() const;

    [[nodiscard]] constexpr T& front();
    [[nodiscard]] constexpr T const& front() const;
    [[nodiscard]] constexpr T& back();
    [[nodiscard]] constexpr T const& back() const;

    constexpr void splice(const_iterator pos, List& other, const_iterator it);
    constexpr void splice(const_iterator pos, List&& other, const_iterator it);

private:
    template<Directions TDirection>
    constexpr DataLocation requestFreePlace();
    template<Directions TDirection>
    constexpr void allocateBlock();
    constexpr static Block* allocateBlock();
    template<Directions TDirection>
    constexpr Block* insertNewBlock(Block* block);
    constexpr static void shiftBlockToFreeUpLastPosition(Block* block);

    constexpr void freeBlock(Block* block);

    Block* g_startBlock;
    Block* g_lastBlock;
    std::size_t g_dataSize;

    BlockIndex g_cacheFrontIndex;
    BlockIndex g_cacheBackIndex;
};

#include "C_list.inl"

}//end gg
