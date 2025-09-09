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

template<class T, class TBlockSize>
constexpr List<T, TBlockSize>::List() :
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
template<class TInputIt>
constexpr List<T, TBlockSize>::List(TInputIt first, TInputIt last) :
        List()
{
    for (auto it=first; it!=last; ++it)
    {
        this->push_back(*it);
    }
}
template<class T, class TBlockSize>
constexpr List<T, TBlockSize>::List(List const& r) :
        List()
{
    for (auto const& value : r)
    {
        this->push_back(value);
    }
}
template<class T, class TBlockSize>
constexpr List<T, TBlockSize>::List(List&& r) noexcept :
        g_startBlock{r.g_startBlock},
        g_lastBlock{r.g_lastBlock},
        g_dataSize{r.g_dataSize},

        g_cacheFrontIndex{r.g_cacheFrontIndex},
        g_cacheBackIndex{r.g_cacheBackIndex}
{
    r.g_startBlock = nullptr;
    r.g_lastBlock = nullptr;
    r.g_dataSize = 0;
    r.g_cacheFrontIndex = gIndexMid-1;
    r.g_cacheBackIndex = gIndexMid;
}
template<class T, class TBlockSize>
constexpr List<T, TBlockSize>::List(std::size_t size) :
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
constexpr List<T, TBlockSize>& List<T, TBlockSize>::operator=(List const& r)
{
    if (this != &r)
    {
        this->clear();
        for (auto const& value : r)
        {
            this->push_back(value);
        }
    }
    return *this;
}
template<class T, class TBlockSize>
constexpr List<T, TBlockSize>& List<T, TBlockSize>::operator=(List&& r) noexcept
{
    if (this != &r)
    {
        this->clear();
        this->g_startBlock = r.g_startBlock;
        this->g_lastBlock = r.g_lastBlock;
        this->g_dataSize = r.g_dataSize;
        this->g_cacheFrontIndex = r.g_cacheFrontIndex;
        this->g_cacheBackIndex = r.g_cacheBackIndex;

        r.g_startBlock = nullptr;
        r.g_lastBlock = nullptr;
        r.g_dataSize = 0;
        r.g_cacheFrontIndex = gIndexMid-1;
        r.g_cacheBackIndex = gIndexMid;
    }
    return *this;
}

template<class T, class TBlockSize>
constexpr void List<T, TBlockSize>::clear()
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
    this->g_lastBlock->_lastBlock = this->g_lastBlock;

    this->g_cacheBackIndex = gIndexMid;
    this->g_cacheFrontIndex = gIndexMid-1;
}

template<class T, class TBlockSize>
template<class U>
constexpr void List<T, TBlockSize>::push_back(U&& value)
{
    auto* data = this->requestFreePlace<Directions::BACK>()._data;
    new (data) T(std::forward<U>(value));
}
template<class T, class TBlockSize>
template<class... TArgs>
constexpr T& List<T, TBlockSize>::emplace_back(TArgs&&... value)
{
    auto* data = this->requestFreePlace<Directions::BACK>()._data;
    new (data) T(std::forward<TArgs>(value)...);
    return *data;
}
template<class T, class TBlockSize>
template<class U>
constexpr void List<T, TBlockSize>::push_front(U&& value)
{
    auto* data = this->requestFreePlace<Directions::FRONT>()._data;
    new (data) T(std::forward<U>(value));
}
template<class T, class TBlockSize>
template<class... TArgs>
constexpr T& List<T, TBlockSize>::emplace_front(TArgs&&... value)
{
    auto* data = this->requestFreePlace<Directions::FRONT>()._data;
    new (data) T(std::forward<TArgs>(value)...);
    return *data;
}

template<class T, class TBlockSize>
constexpr void List<T, TBlockSize>::pop_back()
{
    this->erase(--this->end());
}
template<class T, class TBlockSize>
constexpr void List<T, TBlockSize>::pop_front()
{
    this->erase(this->begin());
}

template<class T, class TBlockSize>
constexpr typename List<T, TBlockSize>::iterator List<T, TBlockSize>::erase(const_iterator const& pos)
{
    if (pos._dataLocation._data == nullptr || pos._dataLocation._position == 0)
    {//Nothing to erase
        return this->end();
    }

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
            this->g_cacheBackIndex = gIndexMid;
            this->g_cacheFrontIndex = gIndexMid-1;
            return iteratorNext;
        }

        if (nextBlock != nullptr && lastBlock != nullptr)
        {//This block can't be a start or end block
            nextBlock->_lastBlock = lastBlock;
            lastBlock->_nextBlock = nextBlock;
        }
        else if (lastBlock == nullptr)
        {
            nextBlock->_lastBlock = nullptr;
            this->g_startBlock = nextBlock;

            //Redo the front cache index
            this->g_cacheFrontIndex = 0;
            while (this->g_cacheFrontIndex != gIndexLast)
            {
                if (nextBlock->_occupiedFlags & (static_cast<TBlockSize>(1)<<this->g_cacheFrontIndex))
                {
                    --this->g_cacheFrontIndex;
                    break;
                }
                ++this->g_cacheFrontIndex;
            }
        }
        else if (nextBlock == nullptr)
        {
            lastBlock->_nextBlock = nullptr;
            this->g_lastBlock = lastBlock;

            //Redo the back cache index
            this->g_cacheBackIndex = gIndexLast;
            while (this->g_cacheBackIndex != 0)
            {
                if (lastBlock->_occupiedFlags & (static_cast<TBlockSize>(1)<<this->g_cacheBackIndex))
                {
                    ++this->g_cacheBackIndex;
                    break;
                }
                --this->g_cacheBackIndex;
            }
        }
        delete pos._block;
    }
    else
    {
        //Redo the cache indexes
        if (pos._block->_nextBlock == nullptr && (pos._dataLocation._position != static_cast<TBlockSize>(1)<<this->g_cacheBackIndex))
        {
            while (this->g_cacheBackIndex != 0)
            {
                if (pos._block->_occupiedFlags & (static_cast<TBlockSize>(1)<<this->g_cacheBackIndex))
                {
                    ++this->g_cacheBackIndex;
                    break;
                }
                --this->g_cacheBackIndex;
            }
        }
        else if (pos._block->_lastBlock == nullptr && (pos._dataLocation._position != static_cast<TBlockSize>(1)<<this->g_cacheFrontIndex))
        {
            while (this->g_cacheFrontIndex != gIndexLast)
            {
                if (pos._block->_occupiedFlags & (static_cast<TBlockSize>(1)<<this->g_cacheFrontIndex))
                {
                    --this->g_cacheFrontIndex;
                    break;
                }
                ++this->g_cacheFrontIndex;
            }
        }
    }

    return iteratorNext;
}
template<class T, class TBlockSize>
template<class U>
constexpr typename List<T, TBlockSize>::iterator List<T, TBlockSize>::insert(const_iterator pos, U&& value)
{
    if (pos._dataLocation._position == 0)
    {//position is the end so we can just push back
        pos._dataLocation = this->requestFreePlace<Directions::BACK>();
        pos._block = this->g_lastBlock;
        new (pos._dataLocation._data) T(std::forward<U>(value));
        return pos;
    }

    //Getting the mask for all bits before the position
    //ex. position = 0000'1000
    //    maskBefore = 0000'0111
    TBlockSize maskBefore = pos._dataLocation._position-1;

    if ((pos._block->_occupiedFlags & maskBefore) == maskBefore)
    {//We can't insert into the current block as it's full, we have to create a new block or shift last block
        auto lastBlockInsertIndex = gIndexMid;
        Block* lastBlock = pos._block->_lastBlock;

        //We can check if we can insert into the last block
        if (lastBlock == nullptr || lastBlock->_occupiedFlags == std::numeric_limits<TBlockSize>::max())
        {//Well we can't do that, we have to create a new block
            lastBlock = this->insertNewBlock<Directions::FRONT>(pos._block);
        }
        else
        {
            lastBlockInsertIndex = shiftBlockToFreeUpSpace(lastBlock);
        }

        if (maskBefore == 0)
        {//We have to insert into the last block
            ++this->g_dataSize;
            pos._block = lastBlock;
            pos._dataLocation._data = reinterpret_cast<T*>(&lastBlock->_data) + lastBlockInsertIndex;
            pos._dataLocation._position = gPositionFirst << lastBlockInsertIndex;
            lastBlock->_occupiedFlags |= pos._dataLocation._position;
            new (pos._dataLocation._data) T(std::forward<U>(value));
            return pos;
        }

        //Insert the first value of the block into the last block
        lastBlock->_occupiedFlags |= gPositionFirst << lastBlockInsertIndex;
        T* lastBlockData = reinterpret_cast<T*>(&lastBlock->_data) + lastBlockInsertIndex;
        T* data = reinterpret_cast<T*>(&pos._block->_data);
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

    if (!(pos._block->_occupiedFlags & pos._dataLocation._position>>1))
    {//We have the place to insert it just before the current position
        ++this->g_dataSize;
        pos._dataLocation._position >>= 1;
        pos._block->_occupiedFlags |= pos._dataLocation._position;
        --pos._dataLocation._data;

        new (pos._dataLocation._data) T(std::forward<U>(value));
        return pos;
    }

    TBlockSize notOccupiedBefore = ~pos._block->_occupiedFlags & maskBefore;

    //We have to shift data inside the current block
    TBlockSize position = gPositionFirst;
    T* data = reinterpret_cast<T*>(pos._block->_data);
    do
    {
        notOccupiedBefore &=~ position;

        if (notOccupiedBefore == 0)
        {//We are in the last not occupied place
            pos._block->_occupiedFlags |= position;
            new (data) T(std::move(*(data+1)));
            ++data;
            data->~T();
        }
        else
        {
            ++data;
        }

        position <<= 1;
    }
    while (position != pos._dataLocation._position>>1);

    ++this->g_dataSize;
    pos._dataLocation._position = position;
    pos._dataLocation._data = data;
    new (data) T(std::forward<U>(value));
    return pos;
}

template<class T, class TBlockSize>
constexpr std::size_t List<T, TBlockSize>::size() const noexcept
{
    return this->g_dataSize;
}
template<class T, class TBlockSize>
constexpr bool List<T, TBlockSize>::empty() const noexcept
{
    return this->g_dataSize == 0;
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
constexpr T& List<T, TBlockSize>::front()
{
    return *this->begin();
}
template<class T, class TBlockSize>
constexpr T const& List<T, TBlockSize>::front() const
{
    return *this->begin();
}
template<class T, class TBlockSize>
constexpr T& List<T, TBlockSize>::back()
{
    return *--this->end();
}
template<class T, class TBlockSize>
constexpr T const& List<T, TBlockSize>::back() const
{
    return *--this->end();
}

template<class T, class TBlockSize>
constexpr void List<T, TBlockSize>::splice(const_iterator pos, List& other)
{
    for (auto& data : other)
    {
        this->insert(pos, std::move(data));
    }
    other.clear();
}
template<class T, class TBlockSize>
constexpr void List<T, TBlockSize>::splice(const_iterator pos, List&& other)
{
    for (auto& data : other)
    {
        this->insert(pos, std::move(data));
    }
    other.clear();
}
template<class T, class TBlockSize>
constexpr void List<T, TBlockSize>::splice(const_iterator pos, List& other, const_iterator it)
{
    this->insert(pos, std::move(const_cast<T&>(*it)));
    other.erase(it);
}
template<class T, class TBlockSize>
constexpr void List<T, TBlockSize>::splice(const_iterator pos, List&& other, const_iterator it)
{
    this->insert(pos, std::move(const_cast<T&>(*it)));
    other.erase(it);
}

template<class T, class TBlockSize>
template<typename List<T, TBlockSize>::Directions TDirection>
constexpr typename List<T, TBlockSize>::DataLocation List<T, TBlockSize>::requestFreePlace()
{
    if constexpr (TDirection == Directions::FRONT)
    {
        if (this->g_cacheFrontIndex > gIndexLast)
        {//We have to allocate a new block as the index is out of range
            this->allocateBlock<Directions::FRONT>();
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
            this->allocateBlock<Directions::BACK>();
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
template<typename List<T, TBlockSize>::Directions TDirection>
constexpr void List<T, TBlockSize>::allocateBlock()
{
    if constexpr (TDirection == Directions::FRONT)
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
template<typename List<T, TBlockSize>::Directions TDirection>
constexpr typename List<T, TBlockSize>::Block* List<T, TBlockSize>::insertNewBlock(Block* block)
{
    if constexpr (TDirection == Directions::FRONT)
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
constexpr typename List<T, TBlockSize>::BlockIndex List<T, TBlockSize>::shiftBlockToFreeUpSpace(Block* block)
{
    TBlockSize emptyPlacePosition = gPositionFirst;
    BlockIndex emptyIndex = 0;

    TBlockSize dataPlacePosition = emptyPlacePosition;
    T* data = reinterpret_cast<T*>(&block->_data);
    do
    {
        if (!(block->_occupiedFlags & emptyPlacePosition))
        {
            //Find the first data to move
            do
            {
                dataPlacePosition <<= 1;
                ++data;
            }
            while ((block->_occupiedFlags & dataPlacePosition) == 0);

            //Move the data
            new (reinterpret_cast<T*>(&block->_data) + emptyIndex) T(std::move(*data));
            block->_occupiedFlags |= emptyPlacePosition;
            block->_occupiedFlags &=~ dataPlacePosition;
            data->~T();
        }

        emptyPlacePosition <<= 1;
        ++emptyIndex;

        dataPlacePosition = emptyPlacePosition;
        data = reinterpret_cast<T*>(&block->_data) + emptyIndex;
    }
    while ((block->_occupiedFlags & ~(emptyPlacePosition-1)) != 0); //Until the block is fully shifted to the "right"

    return emptyIndex;
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
constexpr typename List<T, TBlockSize>::base_iterator::reference List<T, TBlockSize>::iterator::operator*() const
{
    return *this->_dataLocation._data;
}
template<class T, class TBlockSize>
constexpr typename List<T, TBlockSize>::base_iterator::pointer List<T, TBlockSize>::iterator::operator->() const
{
    return this->_dataLocation._data;
}

//const_iterator

template<class T, class TBlockSize>
constexpr typename List<T, TBlockSize>::base_iterator::const_reference List<T, TBlockSize>::const_iterator::operator*() const
{
    return *this->_dataLocation._data;
}
template<class T, class TBlockSize>
constexpr typename List<T, TBlockSize>::base_iterator::const_pointer List<T, TBlockSize>::const_iterator::operator->() const
{
    return this->_dataLocation._data;
}