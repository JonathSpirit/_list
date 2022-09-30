
template<class T, class TBlockSize>
List<T, TBlockSize>::List()
{
    this->g_startBlock = this->allocateBlock();
    this->g_endBlock = this->g_startBlock;
}
template<class T, class TBlockSize>
List<T, TBlockSize>::~List()
{
    this->clear();
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
}

template<class T, class TBlockSize>
void List<T, TBlockSize>::push_back(const T& data)
{
    auto* place = this->requestFreePlace(InsertionDirections::BACK);
    if (place != nullptr)
    {
        new (place) T(data);
    }
}
template<class T, class TBlockSize>
void List<T, TBlockSize>::push_front(const T& data)
{
    auto* place = this->requestFreePlace(InsertionDirections::FRONT);
    if (place != nullptr)
    {
        new (place) T(data);
    }
}
template<class T, class TBlockSize>
void List<T, TBlockSize>::push(const T& data)
{
    auto* place = this->requestFreePlace();
    if (place != nullptr)
    {
        new (place) T(data);
    }
}

template<class T, class TBlockSize>
typename List<T, TBlockSize>::iterator List<T, TBlockSize>::erase(const iterator& it)
{
    this->freeDataFromBlock(it._block, it._position);

    if (it._block->_occupiedFlags == 0)
    {//no more data, we can free the block (only if this is not the last bloc)
        auto* nextBlock = it._block->_nextBlock;
        auto* lastBlock = it._block->_lastBlock;

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
            this->g_endBlock = lastBlock;
        }

        this->freeBlock(it._block);
        return iterator{nextBlock};
    }
    return iterator{it._block, it._position};
}

template<class T, class TBlockSize>
std::size_t List<T, TBlockSize>::getSize() const
{
    return this->g_blockSize;
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
    Block* lastBlock{nullptr};
    auto** block = &this->g_endBlock;

    while (*block != nullptr)
    {
        lastBlock = *block;
        auto* data = this->requestFreePlaceFromBlock(*block);

        if (data == nullptr)
        {//No more place here
            block = &(*block)->_nextBlock;
        }
        else
        {
            return data;
        }
    }

    (*block) = this->allocateBlock();

    this->g_endBlock = *block;
    this->g_endBlock->_lastBlock = lastBlock;

    return reinterpret_cast<T*>(&this->g_endBlock->_data);
}
template<class T, class TBlockSize>
T* List<T, TBlockSize>::requestFreePlace(InsertionDirections direction)
{
    if (direction == InsertionDirections::FRONT)
    {
        Block* oldStartBlock = this->g_startBlock;
        auto* data = this->requestFreePlaceFromBlock(this->g_startBlock, InsertionDirections::FRONT);

        if (data == nullptr)
        {//Need a new block
            this->g_startBlock->_lastBlock = this->allocateBlock();
            this->g_startBlock = oldStartBlock->_lastBlock;
            this->g_startBlock->_nextBlock = oldStartBlock;

            this->g_startBlock->_occupiedFlags |= static_cast<TBlockSize>(1) << (sizeof(TBlockSize)*8-1);
            return reinterpret_cast<T*>(&this->g_startBlock->_data)+(sizeof(TBlockSize)*8-1);
        }
        return data;
    }
    else
    {
        Block* oldLastBlock = this->g_endBlock;
        auto* data = this->requestFreePlaceFromBlock(this->g_endBlock, InsertionDirections::BACK);

        if (data == nullptr)
        {//Need a new block
            this->g_endBlock->_nextBlock = this->allocateBlock();
            this->g_endBlock = oldLastBlock->_nextBlock;
            this->g_endBlock->_lastBlock = oldLastBlock;

            this->g_endBlock->_occupiedFlags |= 1;
            return reinterpret_cast<T*>(&this->g_endBlock->_data);
        }
        return data;
    }
}
template<class T, class TBlockSize>
T* List<T, TBlockSize>::requestFreePlaceFromBlock(Block* block, [[maybe_unused]] InsertionDirections direction)
{
    if (block->_occupiedFlags != std::numeric_limits<TBlockSize>::max())
    {//There is some place here
        if (direction == InsertionDirections::FRONT)
        {
            TBlockSize mask = 0;
            T* data = reinterpret_cast<T*>(&block->_data);
            for (TBlockSize i=1; i!=0; i<<=1)
            {
                ///         |         |         |
                /// 0000'0000 1111'1111 0000'0000
                mask |= i;
                if (!(block->_occupiedFlags & i) && (block->_occupiedFlags&~mask) == ~mask)
                {
                    ++this->g_dataSize;
                    block->_occupiedFlags |= i;
                    return data;
                }
                ++data;
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
                    return data;
                }
                ++data;
            }
        }
    }
    return nullptr;
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
void List<T, TBlockSize>::freeBlock(Block* block)
{
    T* data = reinterpret_cast<T*>(&block->_data);
    for (TBlockSize i=1; i!=0; i<<=1)
    {
        if (block->_occupiedFlags & i)
        {
            data->~T();
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
List<T, TBlockSize>::iterator::iterator(Block* block, TBlockSize startPosition)
{
    this->_block = block;

    //Go to start position
    this->_data = reinterpret_cast<T*>(&this->_block->_data);
    for (TBlockSize pos=1; pos!=startPosition; pos<<=1)
    {
        ++this->_data;
    }
    this->_position = startPosition;

    if (block != nullptr)
    {
        //Make sure that the position is on a valid value
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

        //No value in this block !
        this->_block = nullptr;
    }
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