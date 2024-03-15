namespace gg
{

template<class T, class TBlockSize>
List<T, TBlockSize>::List() :
        g_startBlock{nullptr},
        g_lastBlock{nullptr},
        g_dataSize{0},

        g_cacheFrontIndex{gIndexMid-1},
        g_cacheBackIndex{gIndexMid}
{
    this->g_startBlock = this->allocateBlock();
    this->g_lastBlock = this->g_startBlock;
}
template<class T, class TBlockSize>
List<T, TBlockSize>::List(std::size_t size) :
        List()
{
    for (std::size_t i=0; i<size; ++i)
    {
        this->push_back(T{});
    }
}
template<class T, class TBlockSize>
List<T, TBlockSize>::~List()
{
    auto* block = this->g_startBlock;

    while (block != nullptr)
    {
        auto* nextBloc = block->_nextBlock;
        this->freeBlock(block);
        block = nextBloc;
    }
}

template<class T, class TBlockSize>
void List<T, TBlockSize>::clear()
{
    auto* block = this->g_startBlock;

    while (block != nullptr)
    {
        auto* nextBloc = block->_nextBlock;
        this->freeBlock(block);
        block = nextBloc;
    }

    this->g_startBlock = this->allocateBlock();
    this->g_lastBlock = this->g_startBlock;
    this->g_end->_lastBlock = this->g_lastBlock;
}

template<class T, class TBlockSize>
template<class U>
void List<T, TBlockSize>::push_back(U&& value)
{
    auto* data = this->requestFreePlace(Positions::BACK)._data;
    if (data == nullptr)
    {
        std::abort();
    }
    new (data) T(std::forward<U>(value));
}
template<class T, class TBlockSize>
template<class U>
void List<T, TBlockSize>::push_front(U&& value)
{
    auto* data = this->requestFreePlace(Positions::FRONT)._data;
    if (data == nullptr)
    {
        std::abort();
    }
    new (data) T(std::forward<U>(value));
}

template<class T, class TBlockSize>
typename List<T, TBlockSize>::iterator List<T, TBlockSize>::erase(const_iterator const& pos)
{
    //Prepare the next iterator
    iterator iteratorNext = pos;
    ++iteratorNext;

    //Erase the data
    pos._dataLocation._data->~T();
    pos._block->_occupiedFlags &=~ pos._dataLocation._position;
    --this->g_dataSize;

    if (pos._block->_occupiedFlags == 0)
    {//Block can be freed
        auto* nextBlock = pos._block->_nextBlock;
        auto* lastBlock = pos._block->_lastBlock;

        if (nextBlock == nullptr && lastBlock == nullptr)
        {//We can't free the last block
            return iteratorNext;
        }

        if (nextBlock != nullptr && lastBlock != nullptr)
        {//This block can't be a start or end block
            nextBlock->_lastBlock = lastBlock;
            lastBlock->_nextBlock = nextBlock;
        }
        else if (nextBlock != nullptr)
        {
            nextBlock->_lastBlock = nullptr;
            this->g_startBlock = nextBlock;
        }
        else if (lastBlock != nullptr)
        {
            lastBlock->_nextBlock = nullptr;
            this->g_lastBlock = lastBlock;
        }
        delete pos._block;
    }

    return iteratorNext;
}
template<class T, class TBlockSize>
template<class U>
typename List<T, TBlockSize>::iterator List<T, TBlockSize>::insert(const_iterator pos, U&& value)
{
    if (pos._dataLocation._position == 0)
    {//position is the end so we cab just push back
        pos._dataLocation = this->requestFreePlace(Positions::BACK);
        pos._block = this->g_lastBlock;
        if (pos._dataLocation._data == nullptr)
        {
            std::abort();
        }
        new (pos._dataLocation._data) T(std::forward<U>(value));
        return pos;
    }

    TBlockSize positionBefore = pos._dataLocation._position>>1;

    if (positionBefore == 0)
    {//We have to insert into the last block
        if (pos._block->_lastBlock == nullptr || pos._block->_lastBlock->_occupiedFlags == std::numeric_limits<TBlockSize>::max())
        {//Well we can't do that, we have to create a new block
            ++this->g_dataSize;
            pos._block = this->insertNewBlock(pos._block, Positions::FRONT);
            pos._block->_occupiedFlags |= gPositionMid;
            pos._dataLocation._data = reinterpret_cast<T*>(&pos._block->_data)+gIndexMid;
            pos._dataLocation._position = gPositionMid;

            new (pos._dataLocation._data) T(std::forward<U>(value));
            return pos;
        }

        if (pos._block->_lastBlock->_occupiedFlags & gPositionLast)
        {//We have to shift data inside the last block
            shiftBlockToFreeUpLastPosition(pos._block->_lastBlock);

            //Add the new value
            ++this->g_dataSize;
            pos._block->_lastBlock->_occupiedFlags |= gPositionLast;
            pos._dataLocation._data = reinterpret_cast<T*>(&pos._block->_lastBlock->_data)+gIndexLast;
            pos._dataLocation._position = gPositionLast;
            pos._block = pos._block->_lastBlock;

            new (pos._dataLocation._data) T(std::forward<U>(value));
            return pos;
        }

        //We can just insert the value as we have some place at the end of the last block
        ++this->g_dataSize;
        pos._block->_lastBlock->_occupiedFlags |= gPositionLast;
        pos._dataLocation._data = reinterpret_cast<T*>(&pos._block->_lastBlock->_data)+gIndexLast;
        pos._dataLocation._position = gPositionLast;

        new (pos._dataLocation._data) T(std::forward<U>(value));
        return pos;
    }

    //We have to insert into the current block
    if (!(pos._block->_occupiedFlags & positionBefore))
    {//We have the place to insert
        ++this->g_dataSize;
        pos._block->_occupiedFlags |= positionBefore;
        --pos._dataLocation._data;
        pos._dataLocation._position = positionBefore;

        new (pos._dataLocation._data) T(std::forward<U>(value));
        return pos;
    }

    //We have to shift data inside the current block
    //First we have to find empty place
    TBlockSize emptyPlacePosition = positionBefore;
    T* data = pos._dataLocation._data-1;
    do
    {
        if (!(pos._block->_occupiedFlags & emptyPlacePosition))
        {
            break;
        }
        emptyPlacePosition >>= 1;
        --data;
    }
    while (emptyPlacePosition != 0);

    if (emptyPlacePosition != 0)
    {//We have some place to insert
        //Let's shift
        for (TBlockSize iPos = emptyPlacePosition; iPos != pos._dataLocation._position>>1; iPos <<= 1)
        {
            new (data) T(std::move(*(data+1)));
            ++data;
            data->~T();
        }

        ++this->g_dataSize;
        pos._block->_occupiedFlags |= emptyPlacePosition;
        pos._dataLocation._position >>= 1;
        pos._dataLocation._data = data;
        new (data) T(std::forward<U>(value));
        return pos;
    }

    TBlockSize lastBlockInsertPosition = gPositionLast;
    Block* lastBlock = pos._block->_lastBlock;

    //We can check if we can insert into the last block
    if (pos._block->_lastBlock == nullptr || pos._block->_lastBlock->_occupiedFlags == std::numeric_limits<TBlockSize>::max())
    {//Well we can't do that, we have to create a new block
        lastBlock = this->insertNewBlock(pos._block, Positions::FRONT);
        lastBlockInsertPosition = gPositionMid;
    }
    else
    {
        shiftBlockToFreeUpLastPosition(pos._block->_lastBlock);
    }

    //Insert the first value of the block into the last block
    lastBlock->_occupiedFlags |= lastBlockInsertPosition;
    T* lastBlockData = reinterpret_cast<T*>(&lastBlock->_data);
    data = reinterpret_cast<T*>(&pos._block->_data);
    new (lastBlockData) T(std::move(*data));
    data->~T();

    //Now we can shift the rest of the data
    for (TBlockSize iPos = gPositionFirst; iPos != pos._dataLocation._position>>1; iPos <<= 1)
    {
        new (data) T(std::move(*(data+1)));
        ++data;
        data->~T();
    }

    //Insert the new value
    ++this->g_dataSize;
    pos._dataLocation._position >>= 1;
    pos._dataLocation._data = data;
    new (data) T(std::forward<U>(value));
    return pos;
}

template<class T, class TBlockSize>
std::size_t List<T, TBlockSize>::size() const
{
    return this->g_dataSize;
}

template<class T, class TBlockSize>
constexpr typename List<T, TBlockSize>::iterator List<T, TBlockSize>::begin()
{
    DataLocation location{reinterpret_cast<T*>(&this->g_startBlock->_data), gPositionFirst};
    do
    {
        if (this->g_startBlock->_occupiedFlags & location._position)
        {
            break;
        }

        location._position <<= 1;
        ++location._data;
    }
    while (location._position != 0);

    return iterator{this->g_startBlock, location};
}
template<class T, class TBlockSize>
constexpr typename List<T, TBlockSize>::iterator List<T, TBlockSize>::end()
{
    return iterator{this->g_lastBlock};
}
template<class T, class TBlockSize>
constexpr typename List<T, TBlockSize>::const_iterator List<T, TBlockSize>::begin() const
{
    DataLocation location{reinterpret_cast<T*>(&this->g_startBlock->_data), gPositionFirst};
    do
    {
        if (this->g_startBlock->_occupiedFlags & location._position)
        {
            break;
        }

        location._position <<= 1;
        ++location._data;
    }
    while (location._position != 0);

    return const_iterator{this->g_startBlock, location};
}
template<class T, class TBlockSize>
constexpr typename List<T, TBlockSize>::const_iterator List<T, TBlockSize>::cbegin() const
{
    return this->begin();
}
template<class T, class TBlockSize>
constexpr typename List<T, TBlockSize>::const_iterator List<T, TBlockSize>::end() const
{
    return const_iterator{this->g_lastBlock};
}
template<class T, class TBlockSize>
constexpr typename List<T, TBlockSize>::const_iterator List<T, TBlockSize>::cend() const
{
    return this->end();
}

template<class T, class TBlockSize>
constexpr typename List<T, TBlockSize>::DataLocation List<T, TBlockSize>::requestFreePlace(Positions direction)
{
    if (direction == Positions::FRONT)
    {
        if (this->g_cacheFrontIndex > gIndexLast)
        {//We have to allocate a new block as the index is out of range
            this->allocateBlock(Positions::FRONT);
            this->g_cacheFrontIndex = gIndexLast;
        }

        DataLocation data{
            reinterpret_cast<T*>(&this->g_startBlock->_data)+this->g_cacheFrontIndex,
            static_cast<TBlockSize>(static_cast<TBlockSize>(1)<<this->g_cacheFrontIndex)
        };

        this->g_startBlock->_occupiedFlags |= data._position;
        --this->g_cacheFrontIndex;

        ++this->g_dataSize;

        return data;
    }
    else
    {
        if (this->g_cacheBackIndex > gIndexLast)
        {//We have to allocate a new block as the index is out of range
            this->allocateBlock(Positions::BACK);
            this->g_cacheBackIndex = 0;
        }

        DataLocation data{
            reinterpret_cast<T*>(&this->g_lastBlock->_data)+this->g_cacheBackIndex,
            static_cast<TBlockSize>(static_cast<TBlockSize>(1)<<this->g_cacheBackIndex)
        };

        this->g_lastBlock->_occupiedFlags |= data._position;
        ++this->g_cacheBackIndex;

        ++this->g_dataSize;

        return data;
    }
}
template<class T, class TBlockSize>
constexpr void List<T, TBlockSize>::allocateBlock(Positions direction)
{
    if (direction == Positions::FRONT)
    {
        auto* oldBlock = this->g_startBlock;

        this->g_startBlock = this->allocateBlock();

        this->g_startBlock->_nextBlock = oldBlock;
        oldBlock->_lastBlock = this->g_startBlock;
    }
    else
    {
        auto* oldBlock = this->g_lastBlock;

        this->g_lastBlock = this->allocateBlock();

        this->g_lastBlock->_lastBlock = oldBlock;
        oldBlock->_nextBlock = this->g_lastBlock;
    }
}
template<class T, class TBlockSize>
constexpr typename List<T, TBlockSize>::Block* List<T, TBlockSize>::allocateBlock()
{
    return new Block{};
}
template<class T, class TBlockSize>
constexpr typename List<T, TBlockSize>::Block* List<T, TBlockSize>::insertNewBlock(Block* block, Positions direction)
{
    if (direction == Positions::FRONT)
    {
        auto* oldBlock = block->_lastBlock;

        block->_lastBlock = this->allocateBlock();

        block->_lastBlock->_nextBlock = block;
        block->_lastBlock->_lastBlock = oldBlock;

        if (oldBlock == nullptr)
        {
            this->g_startBlock = block->_lastBlock;
        }
        return block->_lastBlock;
    }
    else
    {
        auto* oldBlock = block->_nextBlock;

        block->_nextBlock = this->allocateBlock();

        block->_nextBlock->_nextBlock = oldBlock;
        block->_nextBlock->_lastBlock = block;

        if (oldBlock == nullptr)
        {
            this->g_lastBlock = block->_nextBlock;
        }
        return block->_nextBlock;
    }
}
template<class T, class TBlockSize>
constexpr void List<T, TBlockSize>::shiftBlockToFreeUpLastPosition(Block* block)
{
    //Find the first empty place from the end
    TBlockSize emptyPlacePosition = gPositionLast;
    T* data = reinterpret_cast<T*>(&block->_data) + gIndexLast;
    do
    {
        emptyPlacePosition >>= 1;
        --data;
    }
    while (block->_occupiedFlags & emptyPlacePosition);
    block->_occupiedFlags |= emptyPlacePosition;
    block->_occupiedFlags &=~ gPositionLast;

    //Let's shift
    for (TBlockSize iPos = emptyPlacePosition; iPos != gPositionLast; iPos <<= 1)
    {
        new (data) T(std::move(*(data+1)));
        ++data;
        data->~T();
    }
}

template<class T, class TBlockSize>
constexpr void List<T, TBlockSize>::freeBlock(Block* block)
{
    T* data = reinterpret_cast<T*>(&block->_data);
    for (TBlockSize iPos=gPositionFirst; iPos!=0; iPos<<=1)
    {
        if (block->_occupiedFlags & iPos)
        {
            data->~T();
            --this->g_dataSize;
        }
        ++data;
    }
    delete block;
}

//base_iterator

template<class T, class TBlockSize>
constexpr List<T, TBlockSize>::base_iterator::base_iterator(Block* block, DataLocation const& dataLocation) :
        _block(block),
        _dataLocation(dataLocation)
{}
template<class T, class TBlockSize>
constexpr List<T, TBlockSize>::base_iterator::base_iterator(Block* block) :
        _block(block)
{}

template<class T, class TBlockSize>
constexpr void List<T, TBlockSize>::base_iterator::operator--()
{
    if (this->_dataLocation._position == 0)
    {
        this->_dataLocation._position = gPositionLast;
        this->_dataLocation._data = reinterpret_cast<T*>(&this->_block->_data)+gIndexLast;

        do
        {
            if (this->_block->_occupiedFlags & this->_dataLocation._position)
            {
                return;
            }
            this->_dataLocation._position >>=1;
            --this->_dataLocation._data;
        }
        while (this->_dataLocation._position != 0);
    }

    do
    {
        this->_dataLocation._position >>=1;

        //Checking the position
        if (this->_dataLocation._position == 0)
        {
            if (this->_block->_lastBlock == nullptr)
            {
                return;
            }
            this->_block = this->_block->_lastBlock;
            this->_dataLocation._position = gPositionLast;
            this->_dataLocation._data = reinterpret_cast<T*>(&this->_block->_data)+gIndexLast;
        }
        else
        {
            --this->_dataLocation._data;
        }
    }
    while (!(this->_block->_occupiedFlags & this->_dataLocation._position));
}
template<class T, class TBlockSize>
constexpr void List<T, TBlockSize>::base_iterator::operator++()
{
    if (this->_dataLocation._position == 0)
    {//Can't go further (end)
        return;
    }

    do
    {
        this->_dataLocation._position <<=1;

        //Checking the position
        if (this->_dataLocation._position == 0)
        {
            if (this->_block->_nextBlock == nullptr)
            {//Can't go further (end)
                return;
            }
            this->_block = this->_block->_nextBlock;
            this->_dataLocation._position = gPositionFirst;
            this->_dataLocation._data = reinterpret_cast<T*>(&this->_block->_data);
        }
        else
        {
            ++this->_dataLocation._data;
        }
    }
    while (!(this->_block->_occupiedFlags & this->_dataLocation._position));
}

template<class T, class TBlockSize>
constexpr bool List<T, TBlockSize>::base_iterator::operator==(const base_iterator& r) const
{
    return this->_block == r._block && this->_dataLocation._position == r._dataLocation._position;
}
template<class T, class TBlockSize>
constexpr bool List<T, TBlockSize>::base_iterator::operator!=(const base_iterator& r) const
{
    return !this->operator==(r);
}

//iterator

template<class T, class TBlockSize>
constexpr T& List<T, TBlockSize>::iterator::operator*() const
{
    return *this->_dataLocation._data;
}

//const_iterator

template<class T, class TBlockSize>
constexpr T const& List<T, TBlockSize>::const_iterator::operator*() const
{
    return *this->_dataLocation._data;
}

} //namespace gg