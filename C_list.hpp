#ifndef C_LIST_HPP_INCLUDED_
#define C_LIST_HPP_INCLUDED_

#include <cstdlib>
#include <cstdint>
#include <limits>
#include <utility>
#include <type_traits>

namespace gg
{

template<class T, class TBlockSize>
class List
{
    static_assert(std::is_fundamental_v<TBlockSize> && std::is_unsigned_v<TBlockSize>, "TBlockSize must be fundamental and unsigned type !");
public:
    struct Block
    {
        Block* _lastBlock{nullptr};
        uint8_t _data[sizeof(T)*sizeof(TBlockSize)*8]{};
        Block* _nextBlock{nullptr};
        TBlockSize _occupiedFlags{0};
    };
    enum class Positions
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
        iterator(Block* block, T* data, TBlockSize position);
        iterator() = default;

        iterator& operator--();
        iterator& operator++();
        T& operator*();

        bool operator==(const iterator& r) const;
        bool operator!=(const iterator& r) const;

    private:
        typename List<T, TBlockSize>::Block* _block{nullptr};
        T* _data{nullptr};
        TBlockSize _position{0};
        friend List;
    };

    List();
    explicit List(std::size_t size);
    ~List();

    void clear();

    void push_back(const T& value);
    void push_front(const T& value);

    iterator erase(const iterator& pos);
    iterator insert(iterator pos, const T& value);

    [[nodiscard]] std::size_t size() const;

    [[nodiscard]] iterator begin();
    [[nodiscard]] iterator end();

    [[nodiscard]] const iterator begin() const;
    [[nodiscard]] const iterator end() const;

private:
    struct DataPlace
    {
        T* _data{nullptr};
        TBlockSize _position{0};
    };

    T* requestFreePlace();
    DataPlace requestFreePlace(Positions direction);
    DataPlace requestFreePlaceFromBlock(Block* block, Positions direction);
    T* requestFreePlaceFromBlock(Block* block);
    inline Block* allocateBlock();
    Block* insertNewBlock(Block* block, Positions direction);
    void freeBlock(Block* block);
    void freeDataFromBlock(Block* block, TBlockSize position);

    Block* g_startBlock{nullptr};
    Block* g_lastBlock{nullptr};
    std::size_t g_dataSize{0};
};

#include "C_list.inl"

}//end gg

#endif //C_LIST_HPP_INCLUDED_