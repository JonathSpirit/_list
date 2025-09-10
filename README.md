# _list

Copyright (C) 2025 Guillaume Guillet

<table border="0px">
<tr>
<td>
Licensed under the MIT License
</td>
</tr>
<tr>
<td>
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
</td>
</tr>
</table>

## Description

_list is a headers (.hpp and .inl files) only library for a convenient replacement of current std::list implementation (May 2024)

The goal is to have the same properties of a std::list (fast insert/erase time and vast iterators validity)
but with faster iterations.

## Features

- Creation/Destruction time comparable to std::deque
- Memory fragmentation
- - Fast erase time
- - Better insert time in some cases
- But
- - Can slow down iterations
- - Can be memory consuming
- Overall faster iterations than std::list
- Insertion time comparable to std::list on complex objects
- Insertion at the back/front comparable to std::deque and faster in some cases


- Iterators validity :
- - Erasing elements doesn't invalidate other iterators
- - Inserting through the back/front doesn't invalidate iterators
- - Inserting can invalidate iterators that are on the same block as the insertion or last block if shifting occurs

## How

A _list is composed of blocks of elements. Each block have the size in bits of a fundamental type (TBlockSize) :
- uint8_t : 8 Elements
- uint16_t : 16 Elements
- uint32_t : 32 Elements
- uint64_t : 64 Elements

To know if a block is full or not, a bitset with the same fundamental type is used.
Each bit represent an element in the block. If the bit is set, the element is used, if not, the element is free.

When inserting an element in the middle of the list, this will happen in order :
- Check if we can insert in the current block
- We first check if the element preceding the insertion point is free (using the bitset)
- - If the place is not free, we will have to shift the elements to make space
- - While shifting, we look for a free place in the last block
- - If we find one, no allocation is needed, and we can insert the element after the shift
- - If not, we allocate a new block, and we insert last shifted element at the middle of the new block


## Tests

Every release mode tests have been made with the flags :
- -O1 -DNDEBUG -Wall -Wextra -Wpedantic

And with hardware :
- AMD Ryzen 9 5950X
- 64GB DDR4 3200MHz

### Container creation

- Container created with N elements (with N the number of iterations)

![test_creation](https://github.com/JonathSpirit/_list/blob/master/images/tests/release/test_creation.png?raw=true)

### Container destruction

- Container deleted with N elements (with N the number of iterations)

![test_destruction](https://github.com/JonathSpirit/_list/blob/master/images/tests/release/test_destruction.png?raw=true)

### Container erase

- Container created with N elements
- Getting an iterator at the middle of the container
- Erasing N elements from the middle iterator
- (with N the number of iterations)

![test_erase](https://github.com/JonathSpirit/_list/blob/master/images/tests/release/test_erase.png?raw=true)

### Container insert "uint32_t"

- Container created with N elements
- Getting an iterator at the middle of the container
- Inserting N elements from the middle iterator
- (with N the number of iterations)

![test_insert_int](https://github.com/JonathSpirit/_list/blob/master/images/tests/release/test_insert_int.png?raw=true)

### Container insert "string"

- Container created with N elements
- Getting an iterator at the middle of the container
- Inserting N elements from the middle iterator
- (with N the number of iterations)

![test_insert_str](https://github.com/JonathSpirit/_list/blob/master/images/tests/release/test_insert_str.png?raw=true)

### Container forward iterations

- Container created with N elements
- Iterating through all N elements
- (with N the number of iterations)

![test_iterations](https://github.com/JonathSpirit/_list/blob/master/images/tests/release/test_iterations.png?raw=true)

### Container push_front

- Pushing N elements in an empty container
- (with N the number of iterations)

![test_pushFront](https://github.com/JonathSpirit/_list/blob/master/images/tests/release/test_pushFront.png?raw=true)

### Container push_back

- Pushing N elements in an empty container
- (with N the number of iterations)

![test_push_back](https://github.com/JonathSpirit/_list/blob/master/images/tests/release/test_push_back.png?raw=true)

