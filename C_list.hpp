#pragma once

#include <cstdlib>
#include <cstdint>
#include <limits>
#include <utility>
#include <type_traits>
#include <iterator>

namespace gg
{

template<class T, class TBlockSize>
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

    constexpr static unsigned short gIndexLast = sizeof(TBlockSize) * 8 - 1;
    constexpr static unsigned short gIndexMid = sizeof(TBlockSize) * 8 / 2;
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

        [[nodiscard]] constexpr typename base_iterator::const_reference operator*() const;
        [[nodiscard]] constexpr typename base_iterator::const_pointer operator->() const;

    private:
        friend List;
    };

    constexpr List();
    constexpr explicit List(std::size_t size);
    ~List();

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

    unsigned short g_cacheFrontIndex;
    unsigned short g_cacheBackIndex;
};

#include "C_list.inl"

}//end gg
