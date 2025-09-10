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
#include <vector>
#include <string>

TEST_CASE("testing list constructors")
{
    SUBCASE("default constructor")
    {
        gg::List<int> list;
        CHECK(list.size() == 0);
        CHECK(list.empty() == true);
        CHECK(list.begin() == list.end());
    }

    SUBCASE("size constructor")
    {
        gg::List<int> list(5);
        CHECK(list.size() == 5);
        CHECK(list.empty() == false);
        
        // Check all elements are default initialized to 0
        int count = 0;
        for (const auto& elem : list)
        {
            CHECK(elem == 0);
            ++count;
        }
        CHECK(count == 5);
    }

    SUBCASE("size constructor with string")
    {
        gg::List<std::string> list(3);
        CHECK(list.size() == 3);
        CHECK(list.empty() == false);
        
        // Check all elements are default initialized to empty string
        for (const auto& elem : list)
        {
            CHECK(elem == "");
        }
    }

    SUBCASE("range constructor from vector")
    {
        std::vector<int> vec = {1, 2, 3, 4, 5};
        gg::List<int> list(vec.begin(), vec.end());
        
        CHECK(list.size() == 5);
        CHECK(list.empty() == false);
        
        // Check elements match vector
        auto vecIt = vec.begin();
        for (const auto& elem : list)
        {
            CHECK(elem == *vecIt);
            ++vecIt;
        }
    }

    SUBCASE("range constructor from another list")
    {
        gg::List<int> original;
        original.push_back(10);
        original.push_back(20);
        original.push_back(30);
        
        gg::List<int> copy(original.begin(), original.end());
        
        CHECK(copy.size() == 3);
        CHECK(copy.empty() == false);
        
        // Check elements match original
        auto origIt = original.begin();
        for (const auto& elem : copy)
        {
            CHECK(elem == *origIt);
            ++origIt;
        }
    }

    SUBCASE("range constructor with empty range")
    {
        std::vector<int> vec;
        gg::List<int> list(vec.begin(), vec.end());
        
        CHECK(list.size() == 0);
        CHECK(list.empty() == true);
        CHECK(list.begin() == list.end());
    }

    SUBCASE("copy constructor")
    {
        gg::List<int> original;
        original.push_back(1);
        original.push_back(2);
        original.push_back(3);
        
        gg::List<int> copy(original);
        
        CHECK(copy.size() == original.size());
        CHECK(copy.empty() == original.empty());
        
        // Check elements match
        auto origIt = original.begin();
        auto copyIt = copy.begin();
        while (origIt != original.end() && copyIt != copy.end())
        {
            CHECK(*copyIt == *origIt);
            ++origIt;
            ++copyIt;
        }
        
        // Verify they are independent - modify original
        original.push_back(4);
        CHECK(original.size() == 4);
        CHECK(copy.size() == 3);
    }

    SUBCASE("copy constructor with empty list")
    {
        gg::List<int> original;
        gg::List<int> copy(original);
        
        CHECK(copy.size() == 0);
        CHECK(copy.empty() == true);
        CHECK(copy.begin() == copy.end());
    }

    SUBCASE("move constructor")
    {
        gg::List<int> original;
        original.push_back(1);
        original.push_back(2);
        original.push_back(3);
        
        std::size_t originalSize = original.size();
        
        gg::List<int> moved(std::move(original));
        
        CHECK(moved.size() == originalSize);
        CHECK(moved.empty() == false);
        CHECK(original.size() == 0);
        CHECK(original.empty() == true);
        
        // Check moved elements
        std::vector<int> expected = {1, 2, 3};
        auto expectedIt = expected.begin();
        for (const auto& elem : moved)
        {
            CHECK(elem == *expectedIt);
            ++expectedIt;
        }
    }

    SUBCASE("move constructor with empty list")
    {
        gg::List<int> original;
        gg::List<int> moved(std::move(original));
        
        CHECK(moved.size() == 0);
        CHECK(moved.empty() == true);
        CHECK(original.size() == 0);
        CHECK(original.empty() == true);
    }
}

TEST_CASE("testing list assignment operators")
{
    SUBCASE("copy assignment")
    {
        gg::List<int> original;
        original.push_back(10);
        original.push_back(20);
        original.push_back(30);
        
        gg::List<int> target;
        target.push_back(999);  // Add something to be overwritten
        
        target = original;
        
        CHECK(target.size() == original.size());
        CHECK(target.empty() == original.empty());
        
        // Check elements match
        auto origIt = original.begin();
        auto targetIt = target.begin();
        while (origIt != original.end() && targetIt != target.end())
        {
            CHECK(*targetIt == *origIt);
            ++origIt;
            ++targetIt;
        }
        
        // Verify they are independent
        original.push_back(40);
        CHECK(original.size() == 4);
        CHECK(target.size() == 3);
    }

    SUBCASE("copy assignment with empty source")
    {
        gg::List<int> original;  // empty
        
        gg::List<int> target;
        target.push_back(1);
        target.push_back(2);
        
        target = original;
        
        CHECK(target.size() == 0);
        CHECK(target.empty() == true);
        CHECK(target.begin() == target.end());
    }

    SUBCASE("copy assignment to empty target")
    {
        gg::List<int> original;
        original.push_back(5);
        original.push_back(6);
        
        gg::List<int> target;  // empty
        
        target = original;
        
        CHECK(target.size() == 2);
        CHECK(target.empty() == false);
        
        auto origIt = original.begin();
        auto targetIt = target.begin();
        while (origIt != original.end() && targetIt != target.end())
        {
            CHECK(*targetIt == *origIt);
            ++origIt;
            ++targetIt;
        }
    }

    SUBCASE("self assignment")
    {
        gg::List<int> list;
        list.push_back(1);
        list.push_back(2);
        list.push_back(3);
        
        std::size_t originalSize = list.size();
        
        list = list;  // self assignment
        
        CHECK(list.size() == originalSize);
        CHECK(list.empty() == false);
        
        // Check elements are still there
        std::vector<int> expected = {1, 2, 3};
        auto expectedIt = expected.begin();
        for (const auto& elem : list)
        {
            CHECK(elem == *expectedIt);
            ++expectedIt;
        }
    }

    SUBCASE("move assignment")
    {
        gg::List<int> source;
        source.push_back(10);
        source.push_back(20);
        source.push_back(30);
        
        gg::List<int> target;
        target.push_back(999);  // Add something to be overwritten
        
        std::size_t sourceSize = source.size();
        
        target = std::move(source);
        
        CHECK(target.size() == sourceSize);
        CHECK(target.empty() == false);
        CHECK(source.size() == 0);
        CHECK(source.empty() == true);
        
        // Check moved elements
        std::vector<int> expected = {10, 20, 30};
        auto expectedIt = expected.begin();
        for (const auto& elem : target)
        {
            CHECK(elem == *expectedIt);
            ++expectedIt;
        }
    }

    SUBCASE("move assignment with empty source")
    {
        gg::List<int> source;  // empty
        
        gg::List<int> target;
        target.push_back(1);
        target.push_back(2);
        
        target = std::move(source);
        
        CHECK(target.size() == 0);
        CHECK(target.empty() == true);
        CHECK(source.size() == 0);
        CHECK(source.empty() == true);
    }

    SUBCASE("move assignment to empty target")
    {
        gg::List<int> source;
        source.push_back(5);
        source.push_back(6);
        
        gg::List<int> target;  // empty
        
        std::size_t sourceSize = source.size();
        
        target = std::move(source);
        
        CHECK(target.size() == sourceSize);
        CHECK(target.empty() == false);
        CHECK(source.size() == 0);
        CHECK(source.empty() == true);
        
        std::vector<int> expected = {5, 6};
        auto expectedIt = expected.begin();
        for (const auto& elem : target)
        {
            CHECK(elem == *expectedIt);
            ++expectedIt;
        }
    }
}

TEST_CASE("testing clear method")
{
    SUBCASE("clear non-empty list")
    {
        gg::List<int> list;
        list.push_back(1);
        list.push_back(2);
        list.push_back(3);
        
        CHECK(list.size() == 3);
        CHECK(list.empty() == false);
        
        list.clear();
        
        CHECK(list.size() == 0);
        CHECK(list.empty() == true);
        CHECK(list.begin() == list.end());
    }

    SUBCASE("clear empty list")
    {
        gg::List<int> list;
        
        CHECK(list.size() == 0);
        CHECK(list.empty() == true);
        
        list.clear();
        
        CHECK(list.size() == 0);
        CHECK(list.empty() == true);
        CHECK(list.begin() == list.end());
    }

    SUBCASE("clear and reuse")
    {
        gg::List<int> list;
        list.push_back(1);
        list.push_back(2);
        
        list.clear();
        
        CHECK(list.size() == 0);
        CHECK(list.empty() == true);
        
        // Test that list can be used after clear
        list.push_back(10);
        list.push_back(20);
        
        CHECK(list.size() == 2);
        CHECK(list.empty() == false);
        
        auto it = list.begin();
        CHECK(*it == 10);
        ++it;
        CHECK(*it == 20);
    }
}