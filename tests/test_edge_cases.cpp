/*
 * Copyright 2024 Guillaume Guillet
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "doctest/doctest.h"
#include "C_list.hpp"
#include <string>
#include <vector>
#include <memory>
#include <cmath>

// Test helper classes for different scenarios
class NonCopyable
{
public:
    int value;
    
    NonCopyable(int v) : value(v) {}
    
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
    
    NonCopyable(NonCopyable&& other) noexcept : value(other.value)
    {
        other.value = 0;
    }
    
    NonCopyable& operator=(NonCopyable&& other) noexcept
    {
        if (this != &other)
        {
            value = other.value;
            other.value = 0;
        }
        return *this;
    }
};

class CountingObject
{
public:
    static int constructorCount;
    static int destructorCount;
    static int copyCount;
    static int moveCount;
    
    int id;
    
    CountingObject(int i) : id(i)
    {
        ++constructorCount;
    }
    
    CountingObject(const CountingObject& other) : id(other.id)
    {
        ++copyCount;
    }
    
    CountingObject(CountingObject&& other) noexcept : id(other.id)
    {
        other.id = -1;
        ++moveCount;
    }
    
    CountingObject& operator=(const CountingObject& other)
    {
        if (this != &other)
        {
            id = other.id;
            ++copyCount;
        }
        return *this;
    }
    
    CountingObject& operator=(CountingObject&& other) noexcept
    {
        if (this != &other)
        {
            id = other.id;
            other.id = -1;
            ++moveCount;
        }
        return *this;
    }
    
    ~CountingObject()
    {
        ++destructorCount;
    }
    
    static void reset()
    {
        constructorCount = 0;
        destructorCount = 0;
        copyCount = 0;
        moveCount = 0;
    }
};

int CountingObject::constructorCount = 0;
int CountingObject::destructorCount = 0;
int CountingObject::copyCount = 0;
int CountingObject::moveCount = 0;

TEST_CASE("testing different data types")
{
    SUBCASE("list with floating point numbers")
    {
        gg::List<double> list;
        
        list.push_back(3.14159);
        list.push_back(2.71828);
        list.push_back(1.41421);
        
        CHECK(list.size() == 3);
        
        auto it = list.begin();
        CHECK(*it == doctest::Approx(3.14159));
        ++it;
        CHECK(*it == doctest::Approx(2.71828));
        ++it;
        CHECK(*it == doctest::Approx(1.41421));
        
        // Test modification
        list.front() = 3.0;
        list.back() = 1.5;
        
        CHECK(list.front() == doctest::Approx(3.0));
        CHECK(list.back() == doctest::Approx(1.5));
    }

    SUBCASE("list with characters")
    {
        gg::List<char> list;
        
        std::string word = "hello";
        for (char c : word)
        {
            list.push_back(c);
        }
        
        CHECK(list.size() == 5);
        
        // Reconstruct string
        std::string result;
        for (char c : list)
        {
            result += c;
        }
        
        CHECK(result == "hello");
        
        // Test reverse iteration
        std::string reverse;
        auto it = list.end();
        while (it != list.begin())
        {
            --it;
            reverse += *it;
        }
        
        CHECK(reverse == "olleh");
    }

    SUBCASE("list with pointers")
    {
        gg::List<int*> list;
        
        int a = 10, b = 20, c = 30;
        
        list.push_back(&a);
        list.push_back(&b);
        list.push_back(&c);
        
        CHECK(list.size() == 3);
        
        auto it = list.begin();
        CHECK(**it == 10);
        ++it;
        CHECK(**it == 20);
        ++it;
        CHECK(**it == 30);
        
        // Modify through pointers
        **list.begin() = 100;
        CHECK(a == 100);
        CHECK(**list.begin() == 100);
    }

    SUBCASE("list with std::unique_ptr")
    {
        gg::List<std::unique_ptr<int>> list;
        
        list.push_back(std::make_unique<int>(10));
        list.push_back(std::make_unique<int>(20));
        list.push_back(std::make_unique<int>(30));
        
        CHECK(list.size() == 3);
        
        auto it = list.begin();
        CHECK(**it == 10);
        ++it;
        CHECK(**it == 20);
        ++it;
        CHECK(**it == 30);
        
        // Test move semantics
        auto moved = std::move(*list.begin());
        CHECK(*moved == 10);
        
        // Test emplace with unique_ptr
        list.emplace_back(std::make_unique<int>(40));
        CHECK(list.size() == 4);
    }

    SUBCASE("list with vectors")
    {
        gg::List<std::vector<int>> list;
        
        list.push_back(std::vector<int>{1, 2, 3});
        list.push_back(std::vector<int>{4, 5});
        list.push_back(std::vector<int>{6, 7, 8, 9});
        
        CHECK(list.size() == 3);
        
        auto it = list.begin();
        CHECK(it->size() == 3);
        CHECK((*it)[0] == 1);
        CHECK((*it)[2] == 3);
        
        ++it;
        CHECK(it->size() == 2);
        CHECK((*it)[0] == 4);
        CHECK((*it)[1] == 5);
        
        ++it;
        CHECK(it->size() == 4);
        CHECK((*it)[3] == 9);
    }

    SUBCASE("list with custom structures")
    {
        struct Point
        {
            double x, y;
            
            Point(double x_val, double y_val) : x(x_val), y(y_val) {}
            
            double distance() const
            {
                return std::sqrt(x * x + y * y);
            }
            
            bool operator==(const Point& other) const
            {
                return std::abs(x - other.x) < 1e-9 && std::abs(y - other.y) < 1e-9;
            }
        };
        
        gg::List<Point> list;
        
        list.emplace_back(3.0, 4.0);
        list.emplace_back(1.0, 1.0);
        list.emplace_back(0.0, 0.0);
        
        CHECK(list.size() == 3);
        
        auto it = list.begin();
        CHECK(it->distance() == doctest::Approx(5.0));
        ++it;
        CHECK(it->distance() == doctest::Approx(std::sqrt(2.0)));
        ++it;
        CHECK(it->distance() == doctest::Approx(0.0));
        
        // Test modification
        list.front().x = 6.0;
        list.front().y = 8.0;
        CHECK(list.front().distance() == doctest::Approx(10.0));
    }

    SUBCASE("list with move-only objects")
    {
        gg::List<NonCopyable> list;
        
        list.emplace_back(10);
        list.emplace_back(20);
        list.emplace_back(30);
        
        CHECK(list.size() == 3);
        
        auto it = list.begin();
        CHECK(it->value == 10);
        ++it;
        CHECK(it->value == 20);
        ++it;
        CHECK(it->value == 30);
        
        // Test moving elements
        NonCopyable obj(40);
        list.push_back(std::move(obj));
        CHECK(list.size() == 4);
        CHECK(obj.value == 0);  // Should be moved from
        
        auto lastIt = list.end();
        --lastIt;
        CHECK(lastIt->value == 40);
    }
}

TEST_CASE("testing different template parameters")
{
    SUBCASE("list with uint8_t block size")
    {
        gg::List<int, uint8_t> list;
        
        // uint8_t gives us 8 bits, so block can hold up to 8 elements
        for (int i = 0; i < 20; ++i)  // Force multiple blocks
        {
            list.push_back(i);
        }
        
        CHECK(list.size() == 20);
        
        int expected = 0;
        for (const auto& elem : list)
        {
            CHECK(elem == expected);
            ++expected;
        }
        
        // Test all operations work with smaller block size
        list.pop_front();
        list.pop_back();
        CHECK(list.size() == 18);
        CHECK(list.front() == 1);
        CHECK(list.back() == 18);
    }

    SUBCASE("list with uint16_t block size")
    {
        gg::List<int, uint16_t> list;
        
        // uint16_t gives us 16 bits, so block can hold up to 16 elements
        for (int i = 0; i < 50; ++i)  // Force multiple blocks
        {
            list.push_back(i);
        }
        
        CHECK(list.size() == 50);
        
        // Test operations
        list.insert(list.begin(), -1);
        CHECK(list.size() == 51);
        CHECK(list.front() == -1);
        
        auto it = list.begin();
        ++it;
        CHECK(*it == 0);
    }

    SUBCASE("list with uint32_t block size")
    {
        gg::List<int, uint32_t> list;
        
        // uint32_t gives us 32 bits, so block can hold up to 32 elements
        for (int i = 0; i < 100; ++i)  // Force multiple blocks
        {
            list.push_back(i);
        }
        
        CHECK(list.size() == 100);
        
        // Test erase operations
        auto it = list.begin();
        for (int i = 0; i < 10; ++i)
        {
            ++it;
        }
        it = list.erase(it);  // Erase element 10
        
        CHECK(list.size() == 99);
        CHECK(*it == 11);  // Should point to next element
    }

    SUBCASE("list with uint64_t block size")
    {
        gg::List<int, uint64_t> list;
        
        // uint64_t gives us 64 bits, so block can hold up to 64 elements
        for (int i = 0; i < 200; ++i)  // Force multiple blocks
        {
            list.push_back(i);
        }
        
        CHECK(list.size() == 200);
        
        // Test splice operations
        gg::List<int, uint64_t> other;
        for (int i = 1000; i < 1010; ++i)
        {
            other.push_back(i);
        }
        
        list.splice(list.end(), other);
        CHECK(list.size() == 210);
        CHECK(other.empty());
        
        // Verify last elements
        auto it = list.end();
        --it;
        CHECK(*it == 1009);
    }
}

TEST_CASE("testing memory management and object lifecycle")
{
    SUBCASE("constructor and destructor counting")
    {
        CountingObject::reset();
        
        {
            gg::List<CountingObject> list;
            
            list.emplace_back(1);
            list.emplace_back(2);
            list.emplace_back(3);
            
            CHECK(CountingObject::constructorCount == 3);
            CHECK(CountingObject::destructorCount == 0);
            
            list.pop_back();
            CHECK(CountingObject::destructorCount == 1);
            
            list.clear();
            CHECK(CountingObject::destructorCount == 3);
            
        }  // List destructor called here
        
        // All objects should be destroyed
        CHECK(CountingObject::constructorCount == CountingObject::destructorCount);
    }

    SUBCASE("copy vs move semantics tracking")
    {
        CountingObject::reset();
        
        gg::List<CountingObject> list;
        
        // Test copy construction
        CountingObject obj1(10);
        list.push_back(obj1);
        CHECK(CountingObject::copyCount > 0);
        
        // Reset counters for move test
        int copyCountAfterCopy = CountingObject::copyCount;
        
        // Test move construction
        CountingObject obj2(20);
        list.push_back(std::move(obj2));
        CHECK(CountingObject::moveCount > 0);
        CHECK(CountingObject::copyCount == copyCountAfterCopy);  // No additional copies
        
        // Test emplace (direct construction)
        int constructorCountBefore = CountingObject::constructorCount;
        list.emplace_back(30);
        CHECK(CountingObject::constructorCount == constructorCountBefore + 1);
    }

    SUBCASE("exception safety during construction")
    {
        // This test ensures basic exception safety principles
        gg::List<std::string> list;
        
        list.push_back("safe1");
        list.push_back("safe2");
        
        CHECK(list.size() == 2);
        
        // Test that the list continues to work after potential exception scenarios
        list.emplace_back("safe3");
        CHECK(list.size() == 3);
        CHECK(list.front() == "safe1");
        CHECK(list.back() == "safe3");
    }
}

TEST_CASE("testing large scale operations and performance characteristics")
{
    SUBCASE("large list operations")
    {
        gg::List<int> list;
        
        // Insert many elements
        const int COUNT = 10000;
        for (int i = 0; i < COUNT; ++i)
        {
            if (i % 2 == 0)
            {
                list.push_back(i);
            }
            else
            {
                list.push_front(-i);
            }
        }
        
        CHECK(list.size() == COUNT);
        
        // Test front and back access (should be O(1))
        CHECK(list.front() == -(COUNT - 1));  // Last negative number added
        CHECK(list.back() == (COUNT - 2));    // Last even number added
        
        // Test size operations
        CHECK_FALSE(list.empty());
        
        // Remove many elements
        for (int i = 0; i < COUNT / 2; ++i)
        {
            if (i % 2 == 0)
            {
                list.pop_front();
            }
            else
            {
                list.pop_back();
            }
        }
        
        CHECK(list.size() == COUNT / 2);
    }

    SUBCASE("iterator traversal on large list")
    {
        gg::List<int> list;
        
        const int COUNT = 5000;
        for (int i = 0; i < COUNT; ++i)
        {
            list.push_back(i);
        }
        
        // Forward traversal
        int count = 0;
        int expected = 0;
        for (auto it = list.begin(); it != list.end(); ++it)
        {
            CHECK(*it == expected);
            ++expected;
            ++count;
        }
        CHECK(count == COUNT);
        
        // Backward traversal
        count = 0;
        expected = COUNT - 1;
        auto it = list.end();
        while (it != list.begin())
        {
            --it;
            CHECK(*it == expected);
            --expected;
            ++count;
        }
        CHECK(count == COUNT);
    }

    SUBCASE("multiple lists interaction")
    {
        std::vector<gg::List<int>> lists(10);
        
        // Fill each list with different ranges
        for (int i = 0; i < 10; ++i)
        {
            for (int j = i * 100; j < (i + 1) * 100; ++j)
            {
                lists[i].push_back(j);
            }
        }
        
        // Verify each list
        for (int i = 0; i < 10; ++i)
        {
            CHECK(lists[i].size() == 100);
            CHECK(lists[i].front() == i * 100);
            CHECK(lists[i].back() == (i + 1) * 100 - 1);
        }
        
        // Merge all lists into the first one
        for (int i = 1; i < 10; ++i)
        {
            lists[0].splice(lists[0].end(), lists[i]);
        }
        
        CHECK(lists[0].size() == 1000);
        for (int i = 1; i < 10; ++i)
        {
            CHECK(lists[i].empty());
        }
        
        // Verify merged list contains all elements in order
        int expected = 0;
        for (const auto& elem : lists[0])
        {
            CHECK(elem == expected);
            ++expected;
        }
    }
}

TEST_CASE("testing edge cases and boundary conditions")
{
    SUBCASE("empty list operations safety")
    {
        gg::List<int> list;
        
        // These operations should be safe on empty list
        CHECK(list.begin() == list.end());
        CHECK(list.cbegin() == list.cend());
        
        // Erase on empty list should be safe
        auto it = list.erase(list.begin());
        CHECK(it == list.end());
        
        it = list.erase(list.end());
        CHECK(it == list.end());
        
        // Clear on empty list should be safe
        list.clear();
        CHECK(list.empty());
        CHECK(list.size() == 0);
        
        // Splice empty into empty should be safe
        gg::List<int> other;
        list.splice(list.begin(), other);
        CHECK(list.empty());
        CHECK(other.empty());
    }

    SUBCASE("single element list comprehensive tests")
    {
        gg::List<int> list;
        list.push_back(42);
        
        // Basic state
        CHECK(list.size() == 1);
        CHECK_FALSE(list.empty());
        CHECK(list.front() == 42);
        CHECK(list.back() == 42);
        
        // Iterator tests
        CHECK(list.begin() != list.end());
        CHECK(*list.begin() == 42);
        
        auto it = list.begin();
        ++it;
        CHECK(it == list.end());
        
        it = list.end();
        --it;
        CHECK(it == list.begin());
        CHECK(*it == 42);
        
        // Modification tests
        list.front() = 100;
        CHECK(list.front() == 100);
        CHECK(list.back() == 100);
        CHECK(*list.begin() == 100);
        
        // Insert tests
        list.insert(list.begin(), 50);
        CHECK(list.size() == 2);
        CHECK(list.front() == 50);
        CHECK(list.back() == 100);
        
        // Reset to single element
        list.clear();
        list.push_back(42);
        
        list.insert(list.end(), 50);
        CHECK(list.size() == 2);
        CHECK(list.front() == 42);
        CHECK(list.back() == 50);
    }

    SUBCASE("boundary iterator operations")
    {
        gg::List<int> list;
        list.push_back(1);
        list.push_back(2);
        list.push_back(3);
        
        // Test incrementing end iterator (undefined behavior, but shouldn't crash)
        auto it = list.end();
        // ++it;  // This would be undefined, so we don't test it
        
        // Test decrementing begin iterator (undefined behavior, but shouldn't crash)
        it = list.begin();
        // --it;  // This would be undefined, so we don't test it
        
        // Test valid boundary operations
        it = list.begin();
        ++it;
        ++it;
        ++it;
        CHECK(it == list.end());
        
        --it;
        --it;
        --it;
        CHECK(it == list.begin());
    }

    SUBCASE("mixed operation patterns")
    {
        gg::List<std::string> list;
        
        // Pattern: build, modify, erase, rebuild
        list.push_back("a");
        list.push_back("b");
        list.push_back("c");
        
        // Modify
        list.front() = "A";
        list.back() = "C";
        
        // Erase middle
        auto it = list.begin();
        ++it;
        list.erase(it);
        
        CHECK(list.size() == 2);
        CHECK(list.front() == "A");
        CHECK(list.back() == "C");
        
        // Insert new middle
        it = list.begin();
        ++it;
        list.insert(it, "B");
        
        CHECK(list.size() == 3);
        
        std::vector<std::string> expected = {"A", "B", "C"};
        auto expectedIt = expected.begin();
        for (const auto& elem : list)
        {
            CHECK(elem == *expectedIt);
            ++expectedIt;
        }
        
        // Clear and rebuild
        list.clear();
        list.emplace_back("X");
        list.emplace_front("Y");
        list.emplace_back("Z");
        
        CHECK(list.size() == 3);
        CHECK(list.front() == "Y");
        CHECK(list.back() == "Z");
        
        it = list.begin();
        ++it;
        CHECK(*it == "X");
    }
}