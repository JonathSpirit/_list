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
        
        for (int i = 1; i < 200; i += 2)
        {
            it = list.insert(it, i);  // Insert odd number
            ++it;  // Move past inserted element
            if (it != list.end())
            {
                ++it;  // Move past next even number
            }
        }
        
        CHECK(list.size() == 200);  // Original 100 + 100 inserted
        
        // Verify that all elements are in order
        auto checkIt = list.begin();
        for (int i = 0; i < 200; ++i)
        {
            CHECK(*checkIt == i);
            ++checkIt;
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

TEST_CASE("testing insert with std::string for move-assign optimization")
{
    SUBCASE("insert strings in middle triggers shift-left with move-assign")
    {
        // Use uint8_t block size (8 positions) to exercise block-full paths
        gg::List<std::string, uint8_t> list;
        // Fill a block to trigger the full-block insert path
        for (int i = 0; i < 8; ++i)
        {
            list.push_back("elem_" + std::to_string(i));
        }

        CHECK(list.size() == 8);

        // Insert in the middle, forcing a shift-left through the move-assign chain
        auto it = list.begin();
        for (int i = 0; i < 4; ++i) ++it;  // Point to elem_4

        it = list.insert(it, "inserted_middle");

        CHECK(list.size() == 9);
        CHECK(*it == "inserted_middle");

        // Verify ordering is preserved
        auto checkIt = list.begin();
        for (int i = 0; i < 4; ++i)
        {
            CHECK(*checkIt == "elem_" + std::to_string(i));
            ++checkIt;
        }
        CHECK(*checkIt == "inserted_middle");
        ++checkIt;
        for (int i = 4; i < 8; ++i)
        {
            CHECK(*checkIt == "elem_" + std::to_string(i));
            ++checkIt;
        }
    }

    SUBCASE("insert strings repeatedly at same position")
    {
        gg::List<std::string, uint8_t> list;
        list.push_back("anchor");

        // Insert many strings before the anchor, exercising shift paths repeatedly
        auto anchorIt = list.begin();
        for (int i = 0; i < 20; ++i)
        {
            list.insert(anchorIt, "str_" + std::to_string(i));
        }

        CHECK(list.size() == 21);

        // Verify all inserted strings are before anchor
        auto it = list.begin();
        for (int i = 0; i < 20; ++i)
        {
            CHECK(*it == "str_" + std::to_string(i));
            ++it;
        }
        CHECK(*it == "anchor");
    }

    SUBCASE("insert large strings to exercise move semantics")
    {
        gg::List<std::string, uint16_t> list;
        // Use strings larger than SSO to ensure heap-allocated moves
        std::string longStr(100, 'x');

        for (int i = 0; i < 20; ++i)
        {
            list.push_back(longStr + std::to_string(i));
        }

        auto it = list.begin();
        for (int i = 0; i < 10; ++i) ++it;  // Point to element 10

        std::string insertStr(100, 'y');
        it = list.insert(it, insertStr + "inserted");

        CHECK(list.size() == 21);
        CHECK(*it == insertStr + "inserted");

        // Verify element at position 10 is now the old element 10
        ++it;
        CHECK(*it == longStr + std::to_string(10));
    }

    SUBCASE("insert with different block sizes preserves correctness")
    {
        // Test with uint8_t (8 positions per block)
        {
            gg::List<std::string, uint8_t> list;
            for (int i = 0; i < 50; ++i)
            {
                list.push_back(std::to_string(i * 2));
            }

            auto it = list.begin();
            ++it;
            for (int i = 0; i < 50; ++i)
            {
                it = list.insert(it, std::to_string(i * 2 + 1));
                ++it;
                if (it != list.end()) ++it;
            }

            CHECK(list.size() == 100);
            auto checkIt = list.begin();
            for (int i = 0; i < 100; ++i)
            {
                CHECK(*checkIt == std::to_string(i));
                ++checkIt;
            }
        }
        // Test with uint32_t (32 positions per block)
        {
            gg::List<std::string, uint32_t> list;
            for (int i = 0; i < 50; ++i)
            {
                list.push_back(std::to_string(i * 2));
            }

            auto it = list.begin();
            ++it;
            for (int i = 0; i < 50; ++i)
            {
                it = list.insert(it, std::to_string(i * 2 + 1));
                ++it;
                if (it != list.end()) ++it;
            }

            CHECK(list.size() == 100);
            auto checkIt = list.begin();
            for (int i = 0; i < 100; ++i)
            {
                CHECK(*checkIt == std::to_string(i));
                ++checkIt;
            }
        }
    }

    SUBCASE("insert after erase exercises Case 4 move-assign path")
    {
        gg::List<std::string, uint8_t> list;
        for (int i = 0; i < 8; ++i)
        {
            list.push_back("item_" + std::to_string(i));
        }

        // Erase element at position 2 to create a gap
        auto it = list.begin();
        ++it; ++it;  // Point to item_2
        list.erase(it);
        CHECK(list.size() == 7);

        // Now insert at position 5 (after the gap)
        // This should trigger Case 4 (shift within block with gap)
        it = list.begin();
        for (int i = 0; i < 4; ++i) ++it;  // Should point to item_5

        it = list.insert(it, "new_item");
        CHECK(list.size() == 8);
        CHECK(*it == "new_item");

        // Verify order: item_0, item_1, item_3, item_4, new_item, item_5, item_6, item_7
        std::vector<std::string> expected = {
            "item_0", "item_1", "item_3", "item_4",
            "new_item", "item_5", "item_6", "item_7"
        };
        auto checkIt = list.begin();
        for (const auto& exp : expected)
        {
            CHECK(*checkIt == exp);
            ++checkIt;
        }
    }

    SUBCASE("iterator validity after insert")
    {
        gg::List<std::string> list;
        list.push_back("A");
        list.push_back("B");
        list.push_back("C");

        // Get iterator to B
        auto itB = list.begin();
        ++itB;
        CHECK(*itB == "B");

        // Insert before B
        auto itNew = list.insert(itB, "X");

        // itB should still point to B (iterator stability guarantee)
        CHECK(*itB == "B");
        CHECK(*itNew == "X");

        // Verify full order
        std::vector<std::string> expected = {"A", "X", "B", "C"};
        auto checkIt = list.begin();
        for (const auto& exp : expected)
        {
            CHECK(*checkIt == exp);
            ++checkIt;
        }
    }
}