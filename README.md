# _list

Copyright (C) 2024 Guillaume Guillet

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

_list is a headers only library for a convenient replacement of current std::list implementation (May 2024)

The goal is to have the same properties of a std::list (fast insert/erase time and vast iterators validity)
but with faster iterations.

## Tests

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

