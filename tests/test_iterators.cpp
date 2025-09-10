/*
 * Copyright 2025 Guillaume Guillet
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
#include <algorithm>
#include <vector>

TEST_CASE("testing iterator basic functionality")
{
    SUBCASE("empty list iterators")
    {
        gg::List<int> list;
        
        auto it_begin = list.begin();
        auto it_end = list.end();
        
        CHECK(it_begin == it_end);
        CHECK_FALSE(it_begin != it_end);
    }

    SUBCASE("single element list iterators")
    {
        gg::List<int> list;
        list.push_back(42);
        
        auto it_begin = list.begin();
        auto it_end = list.end();
        
        CHECK(it_begin != it_end);
        CHECK_FALSE(it_begin == it_end);
        
        CHECK(*it_begin == 42);
        CHECK(it_begin.operator->() != nullptr);
        
        auto it_next = it_begin;
        ++it_next;
        CHECK(it_next == it_end);
    }

    SUBCASE("multiple elements forward iteration")
    {
        gg::List<int> list;
        list.push_back(1);
        list.push_back(2);
        list.push_back(3);
        
        std::vector<int> expected = {1, 2, 3};
        auto expectedIt = expected.begin();
        
        for (auto it = list.begin(); it != list.end(); ++it)
        {
            CHECK(*it == *expectedIt);
            ++expectedIt;
        }
        
        CHECK(expectedIt == expected.end());
    }

    SUBCASE("multiple elements backward iteration")
    {
        gg::List<int> list;
        list.push_back(1);
        list.push_back(2);
        list.push_back(3);
        
        std::vector<int> expected = {3, 2, 1};
        auto expectedIt = expected.begin();
        
        auto it = list.end();
        while (it != list.begin())
        {
            --it;
            CHECK(*it == *expectedIt);
            ++expectedIt;
        }
        
        CHECK(expectedIt == expected.end());
    }

    SUBCASE("iterator pre-increment")
    {
        gg::List<int> list;
        list.push_back(10);
        list.push_back(20);
        
        auto it = list.begin();
        CHECK(*it == 10);
        
        auto& ref = ++it;
        CHECK(*it == 20);
        CHECK(&ref == &it);  // Should return reference to self
        
        ++it;
        CHECK(it == list.end());
    }

    SUBCASE("iterator post-increment")
    {
        gg::List<int> list;
        list.push_back(10);
        list.push_back(20);
        
        auto it = list.begin();
        CHECK(*it == 10);
        
        auto old_it = it++;
        CHECK(*old_it == 10);
        CHECK(*it == 20);
        
        it++;
        CHECK(it == list.end());
    }

    SUBCASE("iterator pre-decrement")
    {
        gg::List<int> list;
        list.push_back(10);
        list.push_back(20);
        
        auto it = list.end();
        
        auto& ref = --it;
        CHECK(*it == 20);
        CHECK(&ref == &it);  // Should return reference to self
        
        --it;
        CHECK(*it == 10);
        CHECK(it == list.begin());
    }

    SUBCASE("iterator post-decrement")
    {
        gg::List<int> list;
        list.push_back(10);
        list.push_back(20);
        
        auto it = list.end();
        
        auto old_it = it--;
        CHECK(old_it == list.end());
        CHECK(*it == 20);
        
        old_it = it--;
        CHECK(*old_it == 20);
        CHECK(*it == 10);
        CHECK(it == list.begin());
    }

    SUBCASE("iterator dereference and arrow operator")
    {
        gg::List<std::string> list;
        list.push_back("hello");
        list.push_back("world");
        
        auto it = list.begin();
        
        // Test dereference operator
        CHECK(*it == "hello");
        
        // Test arrow operator
        CHECK(it->length() == 5);
        CHECK(it->substr(0, 2) == "he");
        
        ++it;
        CHECK(*it == "world");
        CHECK(it->length() == 5);
    }
}

TEST_CASE("testing const_iterator functionality")
{
    SUBCASE("const list with const iterators")
    {
        gg::List<int> mutable_list;
        mutable_list.push_back(1);
        mutable_list.push_back(2);
        mutable_list.push_back(3);
        
        const gg::List<int>& const_list = mutable_list;
        
        auto it_begin = const_list.begin();
        auto it_end = const_list.end();
        
        CHECK(it_begin != it_end);
        
        std::vector<int> expected = {1, 2, 3};
        auto expectedIt = expected.begin();
        
        for (auto it = it_begin; it != it_end; ++it)
        {
            CHECK(*it == *expectedIt);
            ++expectedIt;
        }
    }

    SUBCASE("cbegin and cend methods")
    {
        gg::List<int> list;
        list.push_back(10);
        list.push_back(20);
        list.push_back(30);
        
        auto it_cbegin = list.cbegin();
        auto it_cend = list.cend();
        
        CHECK(it_cbegin != it_cend);
        
        std::vector<int> expected = {10, 20, 30};
        auto expectedIt = expected.begin();
        
        for (auto it = it_cbegin; it != it_cend; ++it)
        {
            CHECK(*it == *expectedIt);
            ++expectedIt;
        }
    }

    SUBCASE("const_iterator construction from iterator")
    {
        gg::List<int> list;
        list.push_back(5);
        list.push_back(15);
        
        auto it = list.begin();
        gg::List<int>::const_iterator const_it(it);
        
        CHECK(*const_it == 5);
        
        ++const_it;
        CHECK(*const_it == 15);
        
        ++const_it;
        CHECK(const_it == list.cend());
    }

    SUBCASE("const_iterator comparison with iterator")
    {
        gg::List<int> list;
        list.push_back(100);
        
        auto it = list.begin();
        auto const_it = list.cbegin();
        
        CHECK(it == const_it);
        CHECK(const_it == it);
        CHECK_FALSE(it != const_it);
        CHECK_FALSE(const_it != it);
    }

    SUBCASE("const_iterator arrow operator")
    {
        gg::List<std::string> list;
        list.push_back("test");
        
        auto const_it = list.cbegin();
        
        CHECK(const_it->length() == 4);
        CHECK(const_it->substr(0, 2) == "te");
    }
}

TEST_CASE("testing iterator range-based for loops")
{
    SUBCASE("range-based for loop with non-const list")
    {
        gg::List<int> list;
        list.push_back(1);
        list.push_back(2);
        list.push_back(3);
        
        std::vector<int> result;
        for (auto& elem : list)
        {
            result.push_back(elem);
        }
        
        std::vector<int> expected = {1, 2, 3};
        CHECK(result == expected);
    }

    SUBCASE("range-based for loop with const list")
    {
        gg::List<int> mutable_list;
        mutable_list.push_back(10);
        mutable_list.push_back(20);
        mutable_list.push_back(30);
        
        const gg::List<int>& const_list = mutable_list;
        
        std::vector<int> result;
        for (const auto& elem : const_list)
        {
            result.push_back(elem);
        }
        
        std::vector<int> expected = {10, 20, 30};
        CHECK(result == expected);
    }

    SUBCASE("range-based for loop modification")
    {
        gg::List<int> list;
        list.push_back(1);
        list.push_back(2);
        list.push_back(3);
        
        // Modify elements through range-based for loop
        for (auto& elem : list)
        {
            elem *= 10;
        }
        
        std::vector<int> expected = {10, 20, 30};
        auto expectedIt = expected.begin();
        for (const auto& elem : list)
        {
            CHECK(elem == *expectedIt);
            ++expectedIt;
        }
    }
}

TEST_CASE("testing iterator with STL algorithms")
{
    SUBCASE("std::find")
    {
        gg::List<int> list;
        list.push_back(1);
        list.push_back(2);
        list.push_back(3);
        list.push_back(4);
        list.push_back(5);
        
        auto it = std::find(list.begin(), list.end(), 3);
        CHECK(it != list.end());
        CHECK(*it == 3);
        
        auto it_not_found = std::find(list.begin(), list.end(), 99);
        CHECK(it_not_found == list.end());
    }

    SUBCASE("std::count")
    {
        gg::List<int> list;
        list.push_back(1);
        list.push_back(2);
        list.push_back(2);
        list.push_back(3);
        list.push_back(2);
        
        auto count = std::count(list.begin(), list.end(), 2);
        CHECK(count == 3);
        
        auto count_none = std::count(list.begin(), list.end(), 99);
        CHECK(count_none == 0);
    }

    SUBCASE("std::distance")
    {
        gg::List<int> list;
        list.push_back(1);
        list.push_back(2);
        list.push_back(3);
        list.push_back(4);
        
        auto distance = std::distance(list.begin(), list.end());
        CHECK(distance == 4);
        
        auto it_start = list.begin();
        ++it_start;
        auto partial_distance = std::distance(it_start, list.end());
        CHECK(partial_distance == 3);
    }

    SUBCASE("std::for_each")
    {
        gg::List<int> list;
        list.push_back(1);
        list.push_back(2);
        list.push_back(3);
        
        int sum = 0;
        std::for_each(list.begin(), list.end(), [&sum](int value) {
            sum += value;
        });
        
        CHECK(sum == 6);
    }

    SUBCASE("std::any_of")
    {
        gg::List<int> list;
        list.push_back(1);
        list.push_back(3);
        list.push_back(5);
        
        bool has_even = std::any_of(list.begin(), list.end(), [](int value) {
            return value % 2 == 0;
        });
        CHECK_FALSE(has_even);
        
        list.push_back(4);
        
        has_even = std::any_of(list.begin(), list.end(), [](int value) {
            return value % 2 == 0;
        });
        CHECK(has_even);
    }

    SUBCASE("std::all_of")
    {
        gg::List<int> list;
        list.push_back(2);
        list.push_back(4);
        list.push_back(6);
        
        bool all_even = std::all_of(list.begin(), list.end(), [](int value) {
            return value % 2 == 0;
        });
        CHECK(all_even);
        
        list.push_back(7);
        
        all_even = std::all_of(list.begin(), list.end(), [](int value) {
            return value % 2 == 0;
        });
        CHECK_FALSE(all_even);
    }
}

TEST_CASE("testing iterator with complex data types")
{
    struct TestStruct
    {
        int id;
        std::string name;
        double value;
        
        TestStruct(int i, const std::string& n, double v) : id(i), name(n), value(v) {}
        
        bool operator==(const TestStruct& other) const
        {
            return id == other.id && name == other.name && value == other.value;
        }
    };

    SUBCASE("iterate over custom structs")
    {
        gg::List<TestStruct> list;
        list.emplace_back(1, "first", 1.1);
        list.emplace_back(2, "second", 2.2);
        list.emplace_back(3, "third", 3.3);
        
        std::vector<TestStruct> expected = {
            {1, "first", 1.1},
            {2, "second", 2.2},
            {3, "third", 3.3}
        };
        
        auto expectedIt = expected.begin();
        for (const auto& elem : list)
        {
            CHECK(elem == *expectedIt);
            ++expectedIt;
        }
    }

    SUBCASE("arrow operator with custom structs")
    {
        gg::List<TestStruct> list;
        list.emplace_back(42, "test", 99.9);
        
        auto it = list.begin();
        
        CHECK(it->id == 42);
        CHECK(it->name == "test");
        CHECK(it->value == 99.9);
        
        // Modify through arrow operator
        it->id = 100;
        it->name = "modified";
        it->value = 123.45;
        
        CHECK(it->id == 100);
        CHECK(it->name == "modified");
        CHECK(it->value == 123.45);
    }
}

TEST_CASE("testing iterator edge cases")
{
    SUBCASE("single element forward and backward")
    {
        gg::List<int> list;
        list.push_back(42);
        
        auto it = list.begin();
        CHECK(*it == 42);
        
        ++it;
        CHECK(it == list.end());
        
        --it;
        CHECK(it == list.begin());
        CHECK(*it == 42);
    }

    SUBCASE("multiple increments and decrements")
    {
        gg::List<int> list;
        list.push_back(1);
        list.push_back(2);
        list.push_back(3);
        list.push_back(4);
        list.push_back(5);
        
        auto it = list.begin();
        
        // Go to middle
        ++it; ++it;  // At element 3
        CHECK(*it == 3);
        
        // Go back to start
        --it; --it;
        CHECK(it == list.begin());
        CHECK(*it == 1);
        
        // Go to end
        ++it; ++it; ++it; ++it; ++it;
        CHECK(it == list.end());
        
        // Go back to last element
        --it;
        CHECK(*it == 5);
    }

    SUBCASE("iterator with large list")
    {
        gg::List<int> list;
        
        // Create a large list
        for (int i = 0; i < 1000; ++i)
        {
            list.push_back(i);
        }
        
        // Test forward iteration
        int expected = 0;
        for (auto it = list.begin(); it != list.end(); ++it)
        {
            CHECK(*it == expected);
            ++expected;
        }
        
        // Test backward iteration
        expected = 999;
        auto it = list.end();
        while (it != list.begin())
        {
            --it;
            CHECK(*it == expected);
            --expected;
        }
    }

    SUBCASE("iterator comparison edge cases")
    {
        gg::List<int> list;
        list.push_back(1);
        list.push_back(2);
        
        auto it1 = list.begin();
        auto it2 = list.begin();
        
        CHECK(it1 == it2);
        
        ++it2;
        CHECK(it1 != it2);
        
        ++it1;
        CHECK(it1 == it2);
        
        ++it1;
        ++it2;
        CHECK(it1 == list.end());
        CHECK(it2 == list.end());
        CHECK(it1 == it2);
    }
}