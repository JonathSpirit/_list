#ifndef C_LIST_HPP_INCLUDED_
#define C_LIST_HPP_INCLUDED_

#include <cstdint>
#include <limits>

namespace gg
{

template<class T, class TBlockSize>
class List
{
    static_assert(std::is_fundamental<TBlockSize>::value, "TBlockSize must be a fundamental type !");
public:
    struct Block
    {
        Block* _lastBlock{nullptr};
        uint8_t _data[sizeof(T)*sizeof(TBlockSize)*8]{0};
        Block* _nextBlock{nullptr};
        TBlockSize _occupiedFlags{0};
    };
    enum class InsertionDirections : uint8_t
    {
        FRONT,
        BACK
    };

    class iterator
    {
    public:
        enum class PositionTypes : uint8_t
        {
            POS_START,
            POS_END,
            POS_INPLACE
        };

        explicit iterator(Block* block, TBlockSize position=1, PositionTypes positionType=PositionTypes::POS_START);

        iterator& operator--();
        iterator& operator++();
        T& operator*();

        bool operator==(const iterator& r) const;
        bool operator!=(const iterator& r) const;

    private:
        typename List<T, TBlockSize>::Block* _block;
        T* _data;
        TBlockSize _position;
        friend List;
    };

    List();
    ~List();

    void clear();

    void push_back(const T& value);
    void push_front(const T& value);
    void push(const T& value);

    iterator erase(const iterator& pos);
    iterator insert(iterator pos, const T& value);

    [[nodiscard]] std::size_t getSize() const;

    [[nodiscard]] iterator begin();
    [[nodiscard]] iterator end();

    [[nodiscard]] const iterator begin() const;
    [[nodiscard]] const iterator end() const;

private:
    T* requestFreePlace();
    T* requestFreePlace(InsertionDirections direction);
    T* requestFreePlaceFromBlock(Block* block, InsertionDirections direction);
    T* requestFreePlaceFromBlock(Block* block);
    Block* allocateBlock();
    Block* insertNewBlock(Block* block, InsertionDirections direction);
    void freeBlock(Block* block);
    void freeDataFromBlock(Block* block, TBlockSize position);

    Block* g_startBlock{nullptr};
    Block* g_lastBlock{nullptr};
    std::size_t g_dataSize{0};
};

#include "C_list.inl"

}//end gg

#endif //C_LIST_HPP_INCLUDED_