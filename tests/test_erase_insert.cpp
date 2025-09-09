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

TEST_CASE("testing erase operations")
{
    SUBCASE("erase single element from single element list")
    {
        gg::List<int> list;
        list.push_back(42);
        
        CHECK(list.size() == 1);
        
        auto it = list.erase(list.begin());
        
        CHECK(list.size() == 0);
        CHECK(list.empty() == true);
        CHECK(it == list.end());
        CHECK(list.begin() == list.end());
    }

    SUBCASE("erase first element from multiple element list")
    {
        gg::List<int> list;
        list.push_back(1);
        list.push_back(2);
        list.push_back(3);
        
        CHECK(list.size() == 3);
        
        auto it = list.erase(list.begin());
        
        CHECK(list.size() == 2);
        CHECK(*it == 2);  // Should point to new first element
        
        // Verify remaining elements
        auto checkIt = list.begin();
        CHECK(*checkIt == 2);
        ++checkIt;
        CHECK(*checkIt == 3);
    }

    SUBCASE("erase last element from multiple element list")
    {
        gg::List<int> list;
        list.push_back(1);
        list.push_back(2);
        list.push_back(3);
        
        CHECK(list.size() == 3);
        
        auto lastIt = list.end();
        --lastIt;  // Point to last element
        
        auto it = list.erase(lastIt);
        
        CHECK(list.size() == 2);
        CHECK(it == list.end());  // Should point to end
        
        // Verify remaining elements
        auto checkIt = list.begin();
        CHECK(*checkIt == 1);
        ++checkIt;
        CHECK(*checkIt == 2);
    }

    SUBCASE("erase middle element from multiple element list")
    {
        gg::List<int> list;
        list.push_back(1);
        list.push_back(2);
        list.push_back(3);
        list.push_back(4);
        list.push_back(5);
        
        CHECK(list.size() == 5);
        
        auto middleIt = list.begin();
        ++middleIt; ++middleIt;  // Point to element 3
        CHECK(*middleIt == 3);
        
        auto it = list.erase(middleIt);
        
        CHECK(list.size() == 4);
        CHECK(*it == 4);  // Should point to element that was after erased one
        
        // Verify remaining elements [1, 2, 4, 5]
        std::vector<int> expected = {1, 2, 4, 5};
        auto expectedIt = expected.begin();
        for (const auto& elem : list)
        {
            CHECK(elem == *expectedIt);
            ++expectedIt;
        }
    }

    SUBCASE("erase multiple elements sequentially")
    {
        gg::List<int> list;
        for (int i = 1; i <= 5; ++i)
        {
            list.push_back(i);
        }
        
        CHECK(list.size() == 5);
        
        // Erase elements 2 and 4
        auto it = list.begin();
        ++it;  // Point to element 2
        it = list.erase(it);  // Erase 2, now points to 3
        CHECK(*it == 3);
        
        ++it;  // Point to element 4
        it = list.erase(it);  // Erase 4, now points to 5
        CHECK(*it == 5);
        
        CHECK(list.size() == 3);
        
        // Verify remaining elements [1, 3, 5]
        std::vector<int> expected = {1, 3, 5};
        auto expectedIt = expected.begin();
        for (const auto& elem : list)
        {
            CHECK(elem == *expectedIt);
            ++expectedIt;
        }
    }

    SUBCASE("erase all elements one by one from front")
    {
        gg::List<int> list;
        list.push_back(1);
        list.push_back(2);
        list.push_back(3);
        
        while (!list.empty())
        {
            auto it = list.erase(list.begin());
            if (!list.empty())
            {
                CHECK(it == list.begin());
            }
            else
            {
                CHECK(it == list.end());
            }
        }
        
        CHECK(list.size() == 0);
        CHECK(list.empty() == true);
    }

    SUBCASE("erase all elements one by one from back")
    {
        gg::List<int> list;
        list.push_back(1);
        list.push_back(2);
        list.push_back(3);
        
        while (!list.empty())
        {
            auto lastIt = list.end();
            --lastIt;
            auto it = list.erase(lastIt);
            CHECK(it == list.end());
        }
        
        CHECK(list.size() == 0);
        CHECK(list.empty() == true);
    }

    SUBCASE("erase with string objects")
    {
        gg::List<std::string> list;
        list.push_back("first");
        list.push_back("second");
        list.push_back("third");
        
        auto it = list.begin();
        ++it;  // Point to "second"
        
        auto resultIt = list.erase(it);
        
        CHECK(list.size() == 2);
        CHECK(*resultIt == "third");
        
        // Verify remaining elements
        auto checkIt = list.begin();
        CHECK(*checkIt == "first");
        ++checkIt;
        CHECK(*checkIt == "third");
    }

    SUBCASE("erase with const_iterator")
    {
        gg::List<int> list;
        list.push_back(10);
        list.push_back(20);
        list.push_back(30);
        
        gg::List<int>::const_iterator constIt = list.begin();
        ++constIt;  // Point to element 20
        
        auto it = list.erase(constIt);
        
        CHECK(list.size() == 2);
        CHECK(*it == 30);
        
        // Verify remaining elements
        auto checkIt = list.begin();
        CHECK(*checkIt == 10);
        ++checkIt;
        CHECK(*checkIt == 30);
    }
}

TEST_CASE("testing insert operations")
{
    SUBCASE("insert into empty list")
    {
        gg::List<int> list;
        
        auto it = list.insert(list.begin(), 42);
        
        CHECK(list.size() == 1);
        CHECK(*it == 42);
        CHECK(*list.begin() == 42);
        CHECK(it == list.begin());
    }

    SUBCASE("insert at beginning of non-empty list")
    {
        gg::List<int> list;
        list.push_back(2);
        list.push_back(3);
        
        auto it = list.insert(list.begin(), 1);
        
        CHECK(list.size() == 3);
        CHECK(*it == 1);
        CHECK(it == list.begin());
        
        // Verify list is now [1, 2, 3]
        std::vector<int> expected = {1, 2, 3};
        auto expectedIt = expected.begin();
        for (const auto& elem : list)
        {
            CHECK(elem == *expectedIt);
            ++expectedIt;
        }
    }

    SUBCASE("insert at end of non-empty list")
    {
        gg::List<int> list;
        list.push_back(1);
        list.push_back(2);
        
        auto it = list.insert(list.end(), 3);
        
        CHECK(list.size() == 3);
        CHECK(*it == 3);
        
        // Verify list is now [1, 2, 3]
        std::vector<int> expected = {1, 2, 3};
        auto expectedIt = expected.begin();
        for (const auto& elem : list)
        {
            CHECK(elem == *expectedIt);
            ++expectedIt;
        }
    }

    SUBCASE("insert in middle of list")
    {
        gg::List<int> list;
        list.push_back(1);
        list.push_back(3);
        list.push_back(5);
        
        auto middleIt = list.begin();
        ++middleIt;  // Point to element 3
        
        auto it = list.insert(middleIt, 2);
        
        CHECK(list.size() == 4);
        CHECK(*it == 2);
        
        // Verify list is now [1, 2, 3, 5]
        std::vector<int> expected = {1, 2, 3, 5};
        auto expectedIt = expected.begin();
        for (const auto& elem : list)
        {
            CHECK(elem == *expectedIt);
            ++expectedIt;
        }
    }

    SUBCASE("insert multiple elements")
    {
        gg::List<int> list;
        list.push_back(1);
        list.push_back(5);
        
        auto it = list.begin();
        ++it;  // Point to element 5
        
        // Insert 2, 3, 4 before 5
        it = list.insert(it, 4);
        CHECK(*it == 4);
        
        it = list.insert(it, 3);
        CHECK(*it == 3);
        
        it = list.insert(it, 2);
        CHECK(*it == 2);
        
        CHECK(list.size() == 5);
        
        // Verify list is now [1, 2, 3, 4, 5]
        std::vector<int> expected = {1, 2, 3, 4, 5};
        auto expectedIt = expected.begin();
        for (const auto& elem : list)
        {
            CHECK(elem == *expectedIt);
            ++expectedIt;
        }
    }

    SUBCASE("insert with copy semantics")
    {
        gg::List<std::string> list;
        list.push_back("hello");
        list.push_back("world");
        
        std::string toInsert = "beautiful";
        auto it = list.insert(list.end(), toInsert);
        
        CHECK(list.size() == 3);
        CHECK(*it == "beautiful");
        CHECK(toInsert == "beautiful");  // Original should be unchanged
        
        // Verify list
        auto checkIt = list.begin();
        CHECK(*checkIt == "hello");
        ++checkIt;
        CHECK(*checkIt == "world");
        ++checkIt;
        CHECK(*checkIt == "beautiful");
    }

    SUBCASE("insert with move semantics")
    {
        gg::List<std::string> list;
        list.push_back("hello");
        list.push_back("world");
        
        std::string toInsert = "beautiful";
        auto it = list.insert(list.end(), std::move(toInsert));
        
        CHECK(list.size() == 3);
        CHECK(*it == "beautiful");
        
        // Verify list
        auto checkIt = list.begin();
        CHECK(*checkIt == "hello");
        ++checkIt;
        CHECK(*checkIt == "world");
        ++checkIt;
        CHECK(*checkIt == "beautiful");
    }

    SUBCASE("insert with const_iterator")
    {
        gg::List<int> list;
        list.push_back(1);
        list.push_back(3);
        
        gg::List<int>::const_iterator constIt = list.begin();
        ++constIt;  // Point to element 3
        
        auto it = list.insert(constIt, 2);
        
        CHECK(list.size() == 3);
        CHECK(*it == 2);
        
        // Verify list is now [1, 2, 3]
        std::vector<int> expected = {1, 2, 3};
        auto expectedIt = expected.begin();
        for (const auto& elem : list)
        {
            CHECK(elem == *expectedIt);
            ++expectedIt;
        }
    }
}

TEST_CASE("testing combined erase and insert operations")
{
    SUBCASE("erase then insert at same position")
    {
        gg::List<int> list;
        list.push_back(1);
        list.push_back(2);
        list.push_back(3);
        
        auto it = list.begin();
        ++it;  // Point to element 2
        
        it = list.erase(it);  // Erase 2, now points to 3
        CHECK(*it == 3);
        
        it = list.insert(it, 99);  // Insert 99 before 3
        CHECK(*it == 99);
        
        CHECK(list.size() == 3);
        
        // Verify list is now [1, 99, 3]
        std::vector<int> expected = {1, 99, 3};
        auto expectedIt = expected.begin();
        for (const auto& elem : list)
        {
            CHECK(elem == *expectedIt);
            ++expectedIt;
        }
    }

    SUBCASE("alternating erase and insert")
    {
        gg::List<int> list;
        for (int i = 1; i <= 10; ++i)
        {
            list.push_back(i);
        }
        
        // Erase every other element starting from second
        auto it = list.begin();
        ++it;  // Start at element 2
        
        while (it != list.end())
        {
            it = list.erase(it);  // Erase current, move to next
            if (it != list.end())
            {
                ++it;  // Skip next element
            }
        }
        
        CHECK(list.size() == 5);
        
        // Should have [1, 3, 5, 7, 9]
        std::vector<int> expected = {1, 3, 5, 7, 9};
        auto expectedIt = expected.begin();
        for (const auto& elem : list)
        {
            CHECK(elem == *expectedIt);
            ++expectedIt;
        }
        
        // Now insert elements between each pair
        it = list.begin();
        ++it;  // Start after first element
        
        int insertValue = 2;
        while (it != list.end())
        {
            it = list.insert(it, insertValue);
            ++it;  // Move past inserted element
            if (it != list.end())
            {
                ++it;  // Move past next original element
            }
            insertValue += 2;
        }
        
        CHECK(list.size() == 9);
        
        // Should have [1, 2, 3, 4, 5, 6, 7, 8, 9]
        std::vector<int> finalExpected = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        auto finalExpectedIt = finalExpected.begin();
        for (const auto& elem : list)
        {
            CHECK(elem == *finalExpectedIt);
            ++finalExpectedIt;
        }
    }

    SUBCASE("replace all elements using erase and insert")
    {
        gg::List<std::string> list;
        list.push_back("old1");
        list.push_back("old2");
        list.push_back("old3");
        
        std::vector<std::string> newValues = {"new1", "new2", "new3"};
        
        auto it = list.begin();
        for (const auto& newValue : newValues)
        {
            it = list.erase(it);  // Remove old value
            it = list.insert(it, newValue);  // Insert new value
            if (it != list.end())
            {
                ++it;  // Move to next position
            }
        }
        
        CHECK(list.size() == 3);
        
        // Verify new values
        auto expectedIt = newValues.begin();
        for (const auto& elem : list)
        {
            CHECK(elem == *expectedIt);
            ++expectedIt;
        }
    }
}

TEST_CASE("testing erase and insert with large lists")
{
    SUBCASE("erase from large list")
    {
        gg::List<int> list;
        
        // Create large list
        for (int i = 0; i < 1000; ++i)
        {
            list.push_back(i);
        }
        
        CHECK(list.size() == 1000);
        
        // Erase every 10th element
        auto it = list.begin();
        int count = 0;
        while (it != list.end())
        {
            if (count % 10 == 9)  // Every 10th element (0-indexed)
            {
                it = list.erase(it);
            }
            else
            {
                ++it;
            }
            ++count;
        }
        
        CHECK(list.size() == 900);  // Should have removed 100 elements
        
        // Verify no element is divisible by 10 or ends in 9
        for (const auto& elem : list)
        {
            CHECK(elem % 10 != 9);
        }
    }

    SUBCASE("insert into large list")
    {
        gg::List<int> list;
        
        // Create list with even numbers
        for (int i = 0; i < 100; ++i)
        {
            list.push_back(i * 2);
        }
        
        CHECK(list.size() == 100);
        
        // Insert odd numbers
        auto it = list.begin();
        ++it;  // Start after first even number
        
        for (int i = 1; i < 100; i += 2)
        {
            it = list.insert(it, i);  // Insert odd number
            ++it;  // Move past inserted element
            if (it != list.end())
            {
                ++it;  // Move past next even number
            }
        }
        
        CHECK(list.size() == 150);  // Original 100 + 50 inserted
        
        // Verify all elements are in correct order by comparing actual result
        // with the expected result from simulating the same operations
        gg::List<int> expected;
        
        // Create expected list with even numbers
        for (int i = 0; i < 100; ++i)
        {
            expected.push_back(i * 2);
        }
        
        // Insert odd numbers using the same logic
        auto exp_it = expected.begin();
        ++exp_it;  // Start after first even number
        
        for (int i = 1; i < 100; i += 2)
        {
            exp_it = expected.insert(exp_it, i);  // Insert odd number
            ++exp_it;  // Move past inserted element
            if (exp_it != expected.end())
            {
                ++exp_it;  // Move past next even number
            }
        }
        
        // Verify all elements match the expected sequence
        CHECK(list.size() == expected.size());
        auto listIt = list.begin();
        auto expectedIt = expected.begin();
        
        while (listIt != list.end() && expectedIt != expected.end())
        {
            CHECK(*listIt == *expectedIt);
            ++listIt;
            ++expectedIt;
        }
    }
}

TEST_CASE("testing edge cases for erase and insert")
{
    SUBCASE("erase from empty list should be safe")
    {
        gg::List<int> list;
        
        // These should not crash and should return end()
        auto it1 = list.erase(list.begin());
        auto it2 = list.erase(list.end());
        
        CHECK(it1 == list.end());
        CHECK(it2 == list.end());
        CHECK(list.size() == 0);
        CHECK(list.empty() == true);
    }

    SUBCASE("insert at various positions in single element list")
    {
        gg::List<int> list;
        list.push_back(2);
        
        // Insert before the element
        auto it1 = list.insert(list.begin(), 1);
        CHECK(*it1 == 1);
        CHECK(list.size() == 2);
        
        // Insert after the last element
        auto it2 = list.insert(list.end(), 3);
        CHECK(*it2 == 3);
        CHECK(list.size() == 3);
        
        // Verify final list [1, 2, 3]
        std::vector<int> expected = {1, 2, 3};
        auto expectedIt = expected.begin();
        for (const auto& elem : list)
        {
            CHECK(elem == *expectedIt);
            ++expectedIt;
        }
    }

    SUBCASE("complex erase insert pattern")
    {
        gg::List<int> list;
        
        // Build initial list [1, 2, 3, 4, 5]
        for (int i = 1; i <= 5; ++i)
        {
            list.push_back(i);
        }
        
        // Complex pattern: erase 3, insert 30, erase 2, insert 20
        auto it = list.begin();
        ++it; ++it;  // Point to 3
        it = list.erase(it);  // Erase 3, now points to 4
        it = list.insert(it, 30);  // Insert 30 before 4
        
        // List should now be [1, 2, 30, 4, 5]
        it = list.begin();
        ++it;  // Point to 2
        it = list.erase(it);  // Erase 2, now points to 30
        it = list.insert(it, 20);  // Insert 20 before 30
        
        // Final list should be [1, 20, 30, 4, 5]
        CHECK(list.size() == 5);
        
        std::vector<int> expected = {1, 20, 30, 4, 5};
        auto expectedIt = expected.begin();
        for (const auto& elem : list)
        {
            CHECK(elem == *expectedIt);
            ++expectedIt;
        }
    }
}