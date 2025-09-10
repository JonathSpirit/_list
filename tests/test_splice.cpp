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

TEST_CASE("testing splice entire list (lvalue reference)")
{
    SUBCASE("splice non-empty list into empty list")
    {
        gg::List<int> dest;
        gg::List<int> source;
        
        source.push_back(1);
        source.push_back(2);
        source.push_back(3);
        
        CHECK(dest.size() == 0);
        CHECK(source.size() == 3);
        
        dest.splice(dest.begin(), source);
        
        CHECK(dest.size() == 3);
        CHECK(source.size() == 0);
        CHECK(source.empty() == true);
        
        // Verify dest contains the elements
        std::vector<int> expected = {1, 2, 3};
        auto expectedIt = expected.begin();
        for (const auto& elem : dest)
        {
            CHECK(elem == *expectedIt);
            ++expectedIt;
        }
    }

    SUBCASE("splice empty list into non-empty list")
    {
        gg::List<int> dest;
        gg::List<int> source;
        
        dest.push_back(10);
        dest.push_back(20);
        
        CHECK(dest.size() == 2);
        CHECK(source.size() == 0);
        
        dest.splice(dest.begin(), source);
        
        CHECK(dest.size() == 2);
        CHECK(source.size() == 0);
        
        // Verify dest is unchanged
        auto it = dest.begin();
        CHECK(*it == 10);
        ++it;
        CHECK(*it == 20);
    }

    SUBCASE("splice non-empty list into non-empty list at beginning")
    {
        gg::List<int> dest;
        gg::List<int> source;
        
        dest.push_back(4);
        dest.push_back(5);
        dest.push_back(6);
        
        source.push_back(1);
        source.push_back(2);
        source.push_back(3);
        
        dest.splice(dest.begin(), source);
        
        CHECK(dest.size() == 6);
        CHECK(source.size() == 0);
        CHECK(source.empty() == true);
        
        // Verify dest contains [1, 2, 3, 4, 5, 6]
        std::vector<int> expected = {1, 2, 3, 4, 5, 6};
        auto expectedIt = expected.begin();
        for (const auto& elem : dest)
        {
            CHECK(elem == *expectedIt);
            ++expectedIt;
        }
    }

    SUBCASE("splice non-empty list into non-empty list at end")
    {
        gg::List<int> dest;
        gg::List<int> source;
        
        dest.push_back(1);
        dest.push_back(2);
        dest.push_back(3);
        
        source.push_back(4);
        source.push_back(5);
        source.push_back(6);
        
        dest.splice(dest.end(), source);
        
        CHECK(dest.size() == 6);
        CHECK(source.size() == 0);
        CHECK(source.empty() == true);
        
        // Verify dest contains [1, 2, 3, 4, 5, 6]
        std::vector<int> expected = {1, 2, 3, 4, 5, 6};
        auto expectedIt = expected.begin();
        for (const auto& elem : dest)
        {
            CHECK(elem == *expectedIt);
            ++expectedIt;
        }
    }

    SUBCASE("splice non-empty list into non-empty list in middle")
    {
        gg::List<int> dest;
        gg::List<int> source;
        
        dest.push_back(1);
        dest.push_back(4);
        dest.push_back(5);
        
        source.push_back(2);
        source.push_back(3);
        
        auto it = dest.begin();
        ++it;  // Point to element 4
        
        dest.splice(it, source);
        
        CHECK(dest.size() == 5);
        CHECK(source.size() == 0);
        CHECK(source.empty() == true);
        
        // Verify dest contains [1, 2, 3, 4, 5]
        std::vector<int> expected = {1, 2, 3, 4, 5};
        auto expectedIt = expected.begin();
        for (const auto& elem : dest)
        {
            CHECK(elem == *expectedIt);
            ++expectedIt;
        }
    }

    SUBCASE("splice with string objects")
    {
        gg::List<std::string> dest;
        gg::List<std::string> source;
        
        dest.push_back("hello");
        dest.push_back("world");
        
        source.push_back("beautiful");
        source.push_back("amazing");
        
        auto it = dest.begin();
        ++it;  // Point to "world"
        
        dest.splice(it, source);
        
        CHECK(dest.size() == 4);
        CHECK(source.size() == 0);
        
        // Verify dest contains ["hello", "beautiful", "amazing", "world"]
        std::vector<std::string> expected = {"hello", "beautiful", "amazing", "world"};
        auto expectedIt = expected.begin();
        for (const auto& elem : dest)
        {
            CHECK(elem == *expectedIt);
            ++expectedIt;
        }
    }
}

TEST_CASE("testing splice entire list (rvalue reference)")
{
    SUBCASE("splice temporary list into empty list")
    {
        gg::List<int> dest;
        
        // Create temporary list
        gg::List<int> temp;
        temp.push_back(1);
        temp.push_back(2);
        temp.push_back(3);
        
        dest.splice(dest.begin(), std::move(temp));
        
        CHECK(dest.size() == 3);
        CHECK(temp.size() == 0);
        CHECK(temp.empty() == true);
        
        // Verify dest contains the elements
        std::vector<int> expected = {1, 2, 3};
        auto expectedIt = expected.begin();
        for (const auto& elem : dest)
        {
            CHECK(elem == *expectedIt);
            ++expectedIt;
        }
    }

    SUBCASE("splice temporary list into non-empty list")
    {
        gg::List<int> dest;
        dest.push_back(1);
        dest.push_back(4);
        
        // Create temporary list
        gg::List<int> temp;
        temp.push_back(2);
        temp.push_back(3);
        
        auto it = dest.begin();
        ++it;  // Point to element 4
        
        dest.splice(it, std::move(temp));
        
        CHECK(dest.size() == 4);
        CHECK(temp.size() == 0);
        
        // Verify dest contains [1, 2, 3, 4]
        std::vector<int> expected = {1, 2, 3, 4};
        auto expectedIt = expected.begin();
        for (const auto& elem : dest)
        {
            CHECK(elem == *expectedIt);
            ++expectedIt;
        }
    }
}

TEST_CASE("testing splice single element (lvalue reference)")
{
    SUBCASE("splice single element from beginning")
    {
        gg::List<int> dest;
        gg::List<int> source;
        
        dest.push_back(2);
        dest.push_back(3);
        
        source.push_back(1);
        source.push_back(10);
        source.push_back(20);
        
        dest.splice(dest.begin(), source, source.begin());
        
        CHECK(dest.size() == 3);
        CHECK(source.size() == 2);
        
        // Verify dest contains [1, 2, 3]
        std::vector<int> expectedDest = {1, 2, 3};
        auto expectedDestIt = expectedDest.begin();
        for (const auto& elem : dest)
        {
            CHECK(elem == *expectedDestIt);
            ++expectedDestIt;
        }
        
        // Verify source contains [10, 20]
        std::vector<int> expectedSource = {10, 20};
        auto expectedSourceIt = expectedSource.begin();
        for (const auto& elem : source)
        {
            CHECK(elem == *expectedSourceIt);
            ++expectedSourceIt;
        }
    }

    SUBCASE("splice single element from middle")
    {
        gg::List<int> dest;
        gg::List<int> source;
        
        dest.push_back(1);
        dest.push_back(3);
        dest.push_back(5);
        
        source.push_back(10);
        source.push_back(2);
        source.push_back(20);
        
        auto sourceIt = source.begin();
        ++sourceIt;  // Point to element 2
        
        auto destIt = dest.begin();
        ++destIt;  // Point to element 3
        
        dest.splice(destIt, source, sourceIt);
        
        CHECK(dest.size() == 4);
        CHECK(source.size() == 2);
        
        // Verify dest contains [1, 2, 3, 5]
        std::vector<int> expectedDest = {1, 2, 3, 5};
        auto expectedDestIt = expectedDest.begin();
        for (const auto& elem : dest)
        {
            CHECK(elem == *expectedDestIt);
            ++expectedDestIt;
        }
        
        // Verify source contains [10, 20]
        std::vector<int> expectedSource = {10, 20};
        auto expectedSourceIt = expectedSource.begin();
        for (const auto& elem : source)
        {
            CHECK(elem == *expectedSourceIt);
            ++expectedSourceIt;
        }
    }

    SUBCASE("splice single element from end")
    {
        gg::List<int> dest;
        gg::List<int> source;
        
        dest.push_back(1);
        dest.push_back(2);
        
        source.push_back(10);
        source.push_back(20);
        source.push_back(3);
        
        auto sourceIt = source.end();
        --sourceIt;  // Point to last element (3)
        
        dest.splice(dest.end(), source, sourceIt);
        
        CHECK(dest.size() == 3);
        CHECK(source.size() == 2);
        
        // Verify dest contains [1, 2, 3]
        std::vector<int> expectedDest = {1, 2, 3};
        auto expectedDestIt = expectedDest.begin();
        for (const auto& elem : dest)
        {
            CHECK(elem == *expectedDestIt);
            ++expectedDestIt;
        }
        
        // Verify source contains [10, 20]
        std::vector<int> expectedSource = {10, 20};
        auto expectedSourceIt = expectedSource.begin();
        for (const auto& elem : source)
        {
            CHECK(elem == *expectedSourceIt);
            ++expectedSourceIt;
        }
    }

    SUBCASE("splice single element from single element list")
    {
        gg::List<int> dest;
        gg::List<int> source;
        
        dest.push_back(1);
        dest.push_back(3);
        
        source.push_back(2);
        
        auto destIt = dest.begin();
        ++destIt;  // Point to element 3
        
        dest.splice(destIt, source, source.begin());
        
        CHECK(dest.size() == 3);
        CHECK(source.size() == 0);
        CHECK(source.empty() == true);
        
        // Verify dest contains [1, 2, 3]
        std::vector<int> expected = {1, 2, 3};
        auto expectedIt = expected.begin();
        for (const auto& elem : dest)
        {
            CHECK(elem == *expectedIt);
            ++expectedIt;
        }
    }

    SUBCASE("splice single element with string objects")
    {
        gg::List<std::string> dest;
        gg::List<std::string> source;
        
        dest.push_back("hello");
        dest.push_back("world");
        
        source.push_back("extra");
        source.push_back("beautiful");
        source.push_back("more");
        
        auto sourceIt = source.begin();
        ++sourceIt;  // Point to "beautiful"
        
        auto destIt = dest.begin();
        ++destIt;  // Point to "world"
        
        dest.splice(destIt, source, sourceIt);
        
        CHECK(dest.size() == 3);
        CHECK(source.size() == 2);
        
        // Verify dest contains ["hello", "beautiful", "world"]
        std::vector<std::string> expectedDest = {"hello", "beautiful", "world"};
        auto expectedDestIt = expectedDest.begin();
        for (const auto& elem : dest)
        {
            CHECK(elem == *expectedDestIt);
            ++expectedDestIt;
        }
        
        // Verify source contains ["extra", "more"]
        std::vector<std::string> expectedSource = {"extra", "more"};
        auto expectedSourceIt = expectedSource.begin();
        for (const auto& elem : source)
        {
            CHECK(elem == *expectedSourceIt);
            ++expectedSourceIt;
        }
    }
}

TEST_CASE("testing splice single element (rvalue reference)")
{
    SUBCASE("splice single element from temporary list")
    {
        gg::List<int> dest;
        dest.push_back(1);
        dest.push_back(3);
        
        // Create temporary list
        gg::List<int> temp;
        temp.push_back(10);
        temp.push_back(2);
        temp.push_back(20);
        
        auto tempIt = temp.begin();
        ++tempIt;  // Point to element 2
        
        auto destIt = dest.begin();
        ++destIt;  // Point to element 3
        
        dest.splice(destIt, std::move(temp), tempIt);
        
        CHECK(dest.size() == 3);
        CHECK(temp.size() == 2);
        
        // Verify dest contains [1, 2, 3]
        std::vector<int> expectedDest = {1, 2, 3};
        auto expectedDestIt = expectedDest.begin();
        for (const auto& elem : dest)
        {
            CHECK(elem == *expectedDestIt);
            ++expectedDestIt;
        }
        
        // temp should still have remaining elements [10, 20]
        std::vector<int> expectedTemp = {10, 20};
        auto expectedTempIt = expectedTemp.begin();
        for (const auto& elem : temp)
        {
            CHECK(elem == *expectedTempIt);
            ++expectedTempIt;
        }
    }

    SUBCASE("splice only element from temporary single element list")
    {
        gg::List<int> dest;
        dest.push_back(1);
        dest.push_back(3);
        
        // Create temporary list with single element
        gg::List<int> temp;
        temp.push_back(2);
        
        auto destIt = dest.begin();
        ++destIt;  // Point to element 3
        
        dest.splice(destIt, std::move(temp), temp.begin());
        
        CHECK(dest.size() == 3);
        CHECK(temp.size() == 0);
        CHECK(temp.empty() == true);
        
        // Verify dest contains [1, 2, 3]
        std::vector<int> expected = {1, 2, 3};
        auto expectedIt = expected.begin();
        for (const auto& elem : dest)
        {
            CHECK(elem == *expectedIt);
            ++expectedIt;
        }
    }
}

TEST_CASE("testing splice complex scenarios")
{
    SUBCASE("multiple splice operations")
    {
        gg::List<int> dest;
        gg::List<int> source1;
        gg::List<int> source2;
        gg::List<int> source3;
        
        dest.push_back(5);
        
        source1.push_back(1);
        source1.push_back(2);
        
        source2.push_back(3);
        source2.push_back(4);
        
        source3.push_back(6);
        source3.push_back(7);
        
        // Splice source1 at beginning
        dest.splice(dest.begin(), source1);
        
        // Splice source2 in middle (before 5)
        auto it = dest.end();
        --it;  // Point to element 5
        dest.splice(it, source2);
        
        // Splice source3 at end
        dest.splice(dest.end(), source3);
        
        CHECK(dest.size() == 7);
        CHECK(source1.empty());
        CHECK(source2.empty());
        CHECK(source3.empty());
        
        // Verify dest contains [1, 2, 3, 4, 5, 6, 7]
        std::vector<int> expected = {1, 2, 3, 4, 5, 6, 7};
        auto expectedIt = expected.begin();
        for (const auto& elem : dest)
        {
            CHECK(elem == *expectedIt);
            ++expectedIt;
        }
    }

    SUBCASE("splice elements from same list to itself")
    {
        gg::List<int> list;
        list.push_back(1);
        list.push_back(2);
        list.push_back(3);
        list.push_back(4);
        list.push_back(5);
        
        // Move element 2 to before element 4
        auto sourceIt = list.begin();
        ++sourceIt;  // Point to element 2
        
        auto destIt = list.begin();
        ++destIt; ++destIt; ++destIt;  // Point to element 4
        
        list.splice(destIt, list, sourceIt);
        
        CHECK(list.size() == 5);
        
        // Verify list contains [1, 3, 2, 4, 5]
        std::vector<int> expected = {1, 3, 2, 4, 5};
        auto expectedIt = expected.begin();
        for (const auto& elem : list)
        {
            CHECK(elem == *expectedIt);
            ++expectedIt;
        }
    }

    SUBCASE("chain splice operations")
    {
        gg::List<int> list1;
        gg::List<int> list2;
        gg::List<int> list3;
        gg::List<int> final;
        
        list1.push_back(1);
        list1.push_back(2);
        
        list2.push_back(3);
        list2.push_back(4);
        
        list3.push_back(5);
        list3.push_back(6);
        
        // Splice list1 into list2
        list2.splice(list2.begin(), list1);
        
        // Splice list3 into list2
        list2.splice(list2.end(), list3);
        
        // Splice everything into final
        final.splice(final.begin(), list2);
        
        CHECK(final.size() == 6);
        CHECK(list1.empty());
        CHECK(list2.empty());
        CHECK(list3.empty());
        
        // Verify final contains [1, 2, 3, 4, 5, 6]
        std::vector<int> expected = {1, 2, 3, 4, 5, 6};
        auto expectedIt = expected.begin();
        for (const auto& elem : final)
        {
            CHECK(elem == *expectedIt);
            ++expectedIt;
        }
    }

    SUBCASE("splice with large lists")
    {
        gg::List<int> dest;
        gg::List<int> source;
        
        // Create large source list
        for (int i = 0; i < 1000; ++i)
        {
            source.push_back(i);
        }
        
        // Create destination list
        dest.push_back(-1);
        dest.push_back(9999);
        
        // Splice source between -1 and 9999
        auto it = dest.begin();
        ++it;  // Point to 9999
        
        dest.splice(it, source);
        
        CHECK(dest.size() == 1002);
        CHECK(source.empty());
        
        // Verify first few and last few elements
        auto destIt = dest.begin();
        CHECK(*destIt == -1);
        ++destIt;
        CHECK(*destIt == 0);
        ++destIt;
        CHECK(*destIt == 1);
        
        // Check end
        auto endIt = dest.end();
        --endIt;
        CHECK(*endIt == 9999);
        --endIt;
        CHECK(*endIt == 999);
    }
}

TEST_CASE("testing splice edge cases")
{
    SUBCASE("splice empty list into empty list")
    {
        gg::List<int> dest;
        gg::List<int> source;
        
        dest.splice(dest.begin(), source);
        
        CHECK(dest.empty());
        CHECK(source.empty());
        CHECK(dest.size() == 0);
        CHECK(source.size() == 0);
    }

    SUBCASE("splice with const iterators")
    {
        gg::List<int> dest;
        gg::List<int> source;
        
        dest.push_back(1);
        dest.push_back(3);
        
        source.push_back(2);
        
        gg::List<int>::const_iterator destPos = dest.begin();
        ++destPos;  // Point to element 3
        
        gg::List<int>::const_iterator sourcePos = source.begin();
        
        dest.splice(destPos, source, sourcePos);
        
        CHECK(dest.size() == 3);
        CHECK(source.empty());
        
        // Verify dest contains [1, 2, 3]
        std::vector<int> expected = {1, 2, 3};
        auto expectedIt = expected.begin();
        for (const auto& elem : dest)
        {
            CHECK(elem == *expectedIt);
            ++expectedIt;
        }
    }

    SUBCASE("splice after clear operations")
    {
        gg::List<int> dest;
        gg::List<int> source;
        
        // Build up lists
        dest.push_back(1);
        source.push_back(2);
        source.push_back(3);
        
        // Clear dest
        dest.clear();
        
        // Splice should still work
        dest.splice(dest.begin(), source);
        
        CHECK(dest.size() == 2);
        CHECK(source.empty());
        
        std::vector<int> expected = {2, 3};
        auto expectedIt = expected.begin();
        for (const auto& elem : dest)
        {
            CHECK(elem == *expectedIt);
            ++expectedIt;
        }
    }
}