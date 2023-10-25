
template<class T, class TBlockSize>
List<T, TBlockSize>::List()
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
void List<T, TBlockSize>::push_back(const T& value)
{
    auto* data = this->requestFreePlace(Positions::BACK)._data;
    if (data == nullptr)
    {
        std::abort();
    }
    new (data) T(value);
}
template<class T, class TBlockSize>
void List<T, TBlockSize>::push_front(const T& value)
{
    auto* data = this->requestFreePlace(Positions::FRONT).first;
    if (data == nullptr)
    {
        std::abort();
    }
    new (data) T(value);
}

template<class T, class TBlockSize>
typename List<T, TBlockSize>::iterator List<T, TBlockSize>::erase(const iterator& pos)
{
    this->freeDataFromBlock(pos._block, pos._position);

    if (pos._block->_occupiedFlags == 0)
    {//no more data, we can free the block (only if this is not the last bloc)
        auto* nextBlock = pos._block->_nextBlock;
        auto* lastBlock = pos._block->_lastBlock;

        if (nextBlock == nullptr && lastBlock == nullptr)
        {//We can't free the last bloc
            return iterator{nullptr};
        }
        else if (nextBlock != nullptr && lastBlock != nullptr)
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

        this->freeBlock(pos._block);
        return iterator{nextBlock};
    }
    return iterator{pos._block, pos._position};
}
template<class T, class TBlockSize>
typename List<T, TBlockSize>::iterator List<T, TBlockSize>::insert(iterator pos, const T& value)
{
    if (pos._block == nullptr)
    {//end
        auto data = this->requestFreePlace(Positions::BACK);
        return iterator{this->g_lastBlock, data.first, data.second};
    }

    TBlockSize newPosition = pos._position>>1;

    if (newPosition != 0)
    {//Check if there is some place to insert
        if ( !(pos._block->_occupiedFlags & newPosition) )
        {//Have the place to insert !
            pos._block->_occupiedFlags |= newPosition;
            new (pos._data-1) T(value);
            ++this->g_dataSize;
            return iterator{pos._block, pos._data-1, newPosition};
        }

        //There is already a value here, we have to shift
        T* data = reinterpret_cast<T*>(&pos._block->_data);
        for (TBlockSize shiftPos=1; shiftPos!=newPosition; shiftPos<<=1)
        {
            if ((shiftPos == 1) && (pos._block->_occupiedFlags & shiftPos))
            {
                //Can we put this value inside the last bloc ?
                if ((pos._block->_lastBlock != nullptr) && !(pos._block->_lastBlock->_occupiedFlags & (static_cast<TBlockSize>(1)<<(sizeof(TBlockSize)*8-1))))
                {
                    new (pos._block->_lastBlock->_data+(sizeof(TBlockSize)*8-1)) T(std::move(*data));
                    data->~T();
                    pos._block->_lastBlock->_occupiedFlags |= static_cast<TBlockSize>(1)<<(sizeof(TBlockSize)*8-1);
                    pos._block->_occupiedFlags &=~ shiftPos;
                }
                else
                {//Sadly not, we have to create a new block
                    auto* newBlock = this->insertNewBlock(pos._block, Positions::FRONT);
                    new (newBlock->_data+(sizeof(TBlockSize)*8-1)) T(std::move(*data));
                    data->~T();
                    newBlock->_occupiedFlags |= static_cast<TBlockSize>(1)<<(sizeof(TBlockSize)*8-1);
                    pos._block->_occupiedFlags &=~ shiftPos;
                }
            }
            else
            {
                if (pos._block->_occupiedFlags & (shiftPos<<1))
                {//we must move the value
                    new (data) T(std::move(*(data+1)));
                    (data+1)->~T();
                    pos._block->_occupiedFlags |= shiftPos;
                }
                else
                {
                    pos._block->_occupiedFlags &=~ shiftPos;
                }
            }

            ++data;
        }

        //Then we can create the new value
        pos._block->_occupiedFlags |= newPosition;
        new (data) T(value);
        ++this->g_dataSize;
        return iterator{pos._block, data, newPosition};
    }
    else
    {
        newPosition = static_cast<TBlockSize>(1)<<(sizeof(TBlockSize)*8-1);

        //Can we put this value inside the last bloc ?
        if ((pos._block->_lastBlock == nullptr) || (pos._block->_lastBlock->_occupiedFlags & (static_cast<TBlockSize>(1)<<(sizeof(TBlockSize)*8-1))))
        {
            //Sadly not, we have to create a new block
            this->insertNewBlock(pos._block, Positions::FRONT);
        }

        T* data = reinterpret_cast<T*>(pos._block->_lastBlock->_data)+(sizeof(TBlockSize)*8-1);
        new (data) T(value);
        pos._block->_lastBlock->_occupiedFlags |= newPosition;
        ++this->g_dataSize;
        return iterator{pos._block->_lastBlock, data, newPosition};
    }
}

template<class T, class TBlockSize>
std::size_t List<T, TBlockSize>::size() const
{
    return this->g_dataSize;
}

template<class T, class TBlockSize>
typename List<T, TBlockSize>::iterator List<T, TBlockSize>::begin()
{
    return iterator{this->g_startBlock};
}
template<class T, class TBlockSize>
typename List<T, TBlockSize>::iterator List<T, TBlockSize>::end()
{
    return iterator{nullptr};
}
template<class T, class TBlockSize>
const typename List<T, TBlockSize>::iterator List<T, TBlockSize>::begin() const
{
    return iterator{this->g_startBlock};
}
template<class T, class TBlockSize>
const typename List<T, TBlockSize>::iterator List<T, TBlockSize>::end() const
{
    return iterator{nullptr};
}

template<class T, class TBlockSize>
T* List<T, TBlockSize>::requestFreePlace()
{
    Block* block{this->g_startBlock};

    while (block->_nextBlock != nullptr)
    {
        auto* data = this->requestFreePlaceFromBlock(block);

        if (data == nullptr)
        {//No more place here
            block = block->_nextBlock;
        }
        else
        {
            return data;
        }
    }

    return this->requestFreePlace(Positions::BACK).first;
}
template<class T, class TBlockSize>
typename List<T, TBlockSize>::DataPlace List<T, TBlockSize>::requestFreePlace(Positions direction)
{
    if (direction == Positions::FRONT)
    {
        auto data = this->requestFreePlaceFromBlock(this->g_startBlock, Positions::FRONT);

        if (data._data == nullptr)
        {//Need a new block
            ++this->g_dataSize;
            auto* newBlock = this->insertNewBlock(this->g_startBlock, Positions::FRONT);

            data._data = reinterpret_cast<T*>(&this->g_startBlock->_data)+(sizeof(TBlockSize)*8-1);
            data._position = static_cast<TBlockSize>(1) << (sizeof(TBlockSize)*8-1);

            newBlock->_occupiedFlags |= data._position;
        }
        return data;
    }
    else
    {
        auto data = this->requestFreePlaceFromBlock(this->g_lastBlock, Positions::BACK);

        if (data._data == nullptr)
        {//Need a new block
            ++this->g_dataSize;
            auto* newBlock = this->insertNewBlock(this->g_lastBlock, Positions::BACK);

            data._data = reinterpret_cast<T*>(&this->g_lastBlock->_data);
            data._position = 1;

            newBlock->_occupiedFlags |= data._position;
        }
        return data;
    }
}
template<class T, class TBlockSize>
typename List<T, TBlockSize>::DataPlace List<T, TBlockSize>::requestFreePlaceFromBlock(Block* block, Positions direction)
{
    if (block->_occupiedFlags != std::numeric_limits<TBlockSize>::max())
    {//There is some place here
        if (direction == Positions::FRONT)
        {
            if (block->_occupiedFlags & 1)
            {
                return {nullptr, 0};
            }

            T* lastData = reinterpret_cast<T*>(&block->_data);

            for (TBlockSize position=2; position != 0; position <<= 1)
            {
                if (block->_occupiedFlags & position)
                {
                    position >>= 1;
                    ++this->g_dataSize;
                    block->_occupiedFlags |= position;
                    return {lastData, position};
                }
                ++lastData;
            }
        }
        else
        {
            TBlockSize mask = 0;
            T* data = reinterpret_cast<T*>(&block->_data);
            for (TBlockSize i=1; i!=0; i<<=1)
            {
                mask |= i;
                if (!(block->_occupiedFlags & i) && !(block->_occupiedFlags&~mask))
                {
                    ++this->g_dataSize;
                    block->_occupiedFlags |= i;
                    return {data, i};
                }
                ++data;
            }
        }
    }
    return {nullptr, 0};
}
template<class T, class TBlockSize>
T* List<T, TBlockSize>::requestFreePlaceFromBlock(Block* block)
{
    if (block->_occupiedFlags != std::numeric_limits<TBlockSize>::max())
    {//There is some place here
        T* data = reinterpret_cast<T*>(&block->_data);
        for (TBlockSize i=1; i!=0; i<<=1)
        {
            if (!(block->_occupiedFlags & i))
            {
                ++this->g_dataSize;
                block->_occupiedFlags |= i;
                return data;
            }
            ++data;
        }
    }
    return nullptr;
}
template<class T, class TBlockSize>
typename List<T, TBlockSize>::Block* List<T, TBlockSize>::allocateBlock()
{
    return new Block{};
}
template<class T, class TBlockSize>
typename List<T, TBlockSize>::Block* List<T, TBlockSize>::insertNewBlock(Block* block, Positions direction)
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
void List<T, TBlockSize>::freeBlock(Block* block)
{
    T* data = reinterpret_cast<T*>(&block->_data);
    for (TBlockSize i=1; i!=0; i<<=1)
    {
        if (block->_occupiedFlags & i)
        {
            data->~T();
            --this->g_dataSize;
        }
        ++data;
    }
    delete block;
}
template<class T, class TBlockSize>
void List<T, TBlockSize>::freeDataFromBlock(Block* block, TBlockSize position)
{
    T* data = reinterpret_cast<T*>(&block->_data);
    for (TBlockSize i=1; i!=0; i<<=1)
    {
        if ((block->_occupiedFlags & i) && (i == position))
        {
            data->~T();
            block->_occupiedFlags &=~ position;
            --this->g_dataSize;
            return;
        }
        ++data;
    }
}

template<class T, class TBlockSize>
List<T, TBlockSize>::iterator::iterator(Block* block, TBlockSize position, PositionTypes positionType)
{
    this->_block = block;
    if (position == 0 || block == nullptr)
    {
        this->_position = 0;
        return;
    }

    //Make sure that _data is on the position
    this->_data = reinterpret_cast<T*>(&this->_block->_data);
    for (TBlockSize pos=1; pos!=position; pos<<=1)
    {
        ++this->_data;
    }
    this->_position = position;

    //Make sure that the position is on a valid value
    switch (positionType)
    {
    case PositionTypes::POS_START:
        do
        {
            if (this->_block->_occupiedFlags & this->_position)
            {
                return;
            }
            this->_position <<=1;
            ++this->_data;
        }
        while (this->_position != 0);
        this->_block = nullptr; //No valid value in this block !
        break;
    case PositionTypes::POS_END:
        do
        {
            if (this->_block->_occupiedFlags & this->_position)
            {
                return;
            }
            this->_position >>=1;
            --this->_data;
        }
        while (this->_position != 0);
        this->_block = nullptr; //No valid value in this block !
        break;
    case PositionTypes::POS_INPLACE:
        break;
    }
}
template<class T, class TBlockSize>
List<T, TBlockSize>::iterator::iterator(Block* block, T* data, TBlockSize position) :
        _block(block),
        _data(data),
        _position(position)
{
}

template<class T, class TBlockSize>
typename List<T, TBlockSize>::iterator& List<T, TBlockSize>::iterator::operator--()
{
    while (this->_block != nullptr)
    {
        this->_position >>=1;
        if (this->_position == 0)
        {
            this->_position = static_cast<TBlockSize>(1)<<(sizeof(TBlockSize)*8-1);
            this->_block = this->_block->_lastBlock;

            if (this->_block == nullptr)
            {
                continue;
            }

            this->_data = reinterpret_cast<T*>(&this->_block->_data)+(sizeof(TBlockSize)*8-1);
        }
        else
        {
            --this->_data;
        }

        if (this->_block->_occupiedFlags & this->_position)
        {
            return *this;
        }
    }
    return *this;
}
template<class T, class TBlockSize>
typename List<T, TBlockSize>::iterator& List<T, TBlockSize>::iterator::operator++()
{
    while (this->_block != nullptr)
    {
        this->_position <<=1;
        if (this->_position == 0)
        {
            this->_position = 1;
            this->_block = this->_block->_nextBlock;

            if (this->_block == nullptr)
            {
                continue;
            }

            this->_data = reinterpret_cast<T*>(&this->_block->_data);
        }
        else
        {
            ++this->_data;
        }

        if (this->_block->_occupiedFlags & this->_position)
        {
            return *this;
        }
    }
    return *this;
}
template<class T, class TBlockSize>
T& List<T, TBlockSize>::iterator::operator*()
{
    return *this->_data;
}

template<class T, class TBlockSize>
bool List<T, TBlockSize>::iterator::operator==(const iterator& r) const
{
    if (r._block == nullptr)
    {
        return this->_block == nullptr;
    }
    return this->_block == r._block && this->_position == r._position;
}
template<class T, class TBlockSize>
bool List<T, TBlockSize>::iterator::operator!=(const iterator& r) const
{
    return !this->operator==(r);
}