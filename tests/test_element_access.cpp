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

TEST_CASE("testing front() method")
{
    SUBCASE("front() on single element list")
    {
        gg::List<int> list;
        list.push_back(42);
        
        CHECK(list.front() == 42);
        
        // Test modifying through front()
        list.front() = 100;
        CHECK(list.front() == 100);
        CHECK(*list.begin() == 100);
    }

    SUBCASE("front() on multiple element list")
    {
        gg::List<int> list;
        list.push_back(1);
        list.push_back(2);
        list.push_back(3);
        
        CHECK(list.front() == 1);
        
        // Test modifying through front()
        list.front() = 99;
        CHECK(list.front() == 99);
        
        // Verify other elements unchanged
        auto it = list.begin();
        CHECK(*it == 99);
        ++it;
        CHECK(*it == 2);
        ++it;
        CHECK(*it == 3);
    }

    SUBCASE("front() with push_front operations")
    {
        gg::List<int> list;
        list.push_back(10);
        list.push_front(5);
        list.push_front(1);
        
        CHECK(list.front() == 1);
        
        list.front() = 0;
        CHECK(list.front() == 0);
        
        // Verify the list is [0, 5, 10]
        auto it = list.begin();
        CHECK(*it == 0);
        ++it;
        CHECK(*it == 5);
        ++it;
        CHECK(*it == 10);
    }

    SUBCASE("front() with string objects")
    {
        gg::List<std::string> list;
        list.push_back("hello");
        list.push_back("world");
        
        CHECK(list.front() == "hello");
        
        list.front() = "hi";
        CHECK(list.front() == "hi");
        
        // Verify second element unchanged
        auto it = list.begin();
        ++it;
        CHECK(*it == "world");
    }

    SUBCASE("front() returns reference")
    {
        gg::List<int> list;
        list.push_back(5);
        
        int& ref = list.front();
        CHECK(ref == 5);
        
        ref = 10;
        CHECK(list.front() == 10);
        CHECK(*list.begin() == 10);
    }
}

TEST_CASE("testing const front() method")
{
    SUBCASE("const front() on single element list")
    {
        gg::List<int> mutable_list;
        mutable_list.push_back(42);
        
        const gg::List<int>& const_list = mutable_list;
        
        CHECK(const_list.front() == 42);
        
        // Verify const-correctness - should not be able to modify
        const int& const_ref = const_list.front();
        CHECK(const_ref == 42);
    }

    SUBCASE("const front() on multiple element list")
    {
        gg::List<int> mutable_list;
        mutable_list.push_back(1);
        mutable_list.push_back(2);
        mutable_list.push_back(3);
        
        const gg::List<int>& const_list = mutable_list;
        
        CHECK(const_list.front() == 1);
        
        // Modify through mutable reference
        mutable_list.front() = 99;
        
        // Verify change visible through const reference
        CHECK(const_list.front() == 99);
    }

    SUBCASE("const front() with string objects")
    {
        gg::List<std::string> mutable_list;
        mutable_list.push_back("test");
        
        const gg::List<std::string>& const_list = mutable_list;
        
        CHECK(const_list.front() == "test");
        CHECK(const_list.front().length() == 4);
    }
}

TEST_CASE("testing back() method")
{
    SUBCASE("back() on single element list")
    {
        gg::List<int> list;
        list.push_back(42);
        
        CHECK(list.back() == 42);
        CHECK(list.back() == list.front());  // Should be same for single element
        
        // Test modifying through back()
        list.back() = 100;
        CHECK(list.back() == 100);
        CHECK(list.front() == 100);  // Should be same for single element
    }

    SUBCASE("back() on multiple element list")
    {
        gg::List<int> list;
        list.push_back(1);
        list.push_back(2);
        list.push_back(3);
        
        CHECK(list.back() == 3);
        CHECK(list.front() == 1);
        
        // Test modifying through back()
        list.back() = 99;
        CHECK(list.back() == 99);
        CHECK(list.front() == 1);  // Front should be unchanged
        
        // Verify the list is [1, 2, 99]
        auto it = list.begin();
        CHECK(*it == 1);
        ++it;
        CHECK(*it == 2);
        ++it;
        CHECK(*it == 99);
    }

    SUBCASE("back() with push_back operations")
    {
        gg::List<int> list;
        list.push_back(1);
        list.push_back(5);
        list.push_back(10);
        
        CHECK(list.back() == 10);
        
        list.back() = 15;
        CHECK(list.back() == 15);
        
        // Verify the list is [1, 5, 15]
        auto it = list.begin();
        CHECK(*it == 1);
        ++it;
        CHECK(*it == 5);
        ++it;
        CHECK(*it == 15);
    }

    SUBCASE("back() with push_front operations")
    {
        gg::List<int> list;
        list.push_front(10);
        list.push_front(5);
        list.push_front(1);
        
        CHECK(list.back() == 10);
        CHECK(list.front() == 1);
        
        list.back() = 20;
        CHECK(list.back() == 20);
        CHECK(list.front() == 1);  // Front should be unchanged
    }

    SUBCASE("back() with string objects")
    {
        gg::List<std::string> list;
        list.push_back("hello");
        list.push_back("world");
        
        CHECK(list.back() == "world");
        CHECK(list.front() == "hello");
        
        list.back() = "universe";
        CHECK(list.back() == "universe");
        CHECK(list.front() == "hello");
    }

    SUBCASE("back() returns reference")
    {
        gg::List<int> list;
        list.push_back(1);
        list.push_back(2);
        
        int& ref = list.back();
        CHECK(ref == 2);
        
        ref = 20;
        CHECK(list.back() == 20);
        
        // Verify front unchanged
        CHECK(list.front() == 1);
    }
}

TEST_CASE("testing const back() method")
{
    SUBCASE("const back() on single element list")
    {
        gg::List<int> mutable_list;
        mutable_list.push_back(42);
        
        const gg::List<int>& const_list = mutable_list;
        
        CHECK(const_list.back() == 42);
        CHECK(const_list.back() == const_list.front());
        
        // Verify const-correctness
        const int& const_ref = const_list.back();
        CHECK(const_ref == 42);
    }

    SUBCASE("const back() on multiple element list")
    {
        gg::List<int> mutable_list;
        mutable_list.push_back(1);
        mutable_list.push_back(2);
        mutable_list.push_back(3);
        
        const gg::List<int>& const_list = mutable_list;
        
        CHECK(const_list.back() == 3);
        CHECK(const_list.front() == 1);
        
        // Modify through mutable reference
        mutable_list.back() = 99;
        
        // Verify change visible through const reference
        CHECK(const_list.back() == 99);
        CHECK(const_list.front() == 1);  // Front should be unchanged
    }

    SUBCASE("const back() with string objects")
    {
        gg::List<std::string> mutable_list;
        mutable_list.push_back("first");
        mutable_list.push_back("last");
        
        const gg::List<std::string>& const_list = mutable_list;
        
        CHECK(const_list.back() == "last");
        CHECK(const_list.back().length() == 4);
        CHECK(const_list.front() == "first");
    }
}

TEST_CASE("testing front() and back() interaction")
{
    SUBCASE("front() and back() with dynamic operations")
    {
        gg::List<int> list;
        
        // Start with one element
        list.push_back(1);
        CHECK(list.front() == 1);
        CHECK(list.back() == 1);
        
        // Add to back
        list.push_back(2);
        CHECK(list.front() == 1);
        CHECK(list.back() == 2);
        
        // Add to front
        list.push_front(0);
        CHECK(list.front() == 0);
        CHECK(list.back() == 2);
        
        // Remove from back
        list.pop_back();
        CHECK(list.front() == 0);
        CHECK(list.back() == 1);
        
        // Remove from front
        list.pop_front();
        CHECK(list.front() == 1);
        CHECK(list.back() == 1);  // Single element again
    }

    SUBCASE("modify both ends")
    {
        gg::List<int> list;
        list.push_back(10);
        list.push_back(20);
        list.push_back(30);
        list.push_back(40);
        
        CHECK(list.front() == 10);
        CHECK(list.back() == 40);
        
        list.front() = 100;
        list.back() = 400;
        
        CHECK(list.front() == 100);
        CHECK(list.back() == 400);
        
        // Verify middle elements unchanged
        auto it = list.begin();
        ++it;
        CHECK(*it == 20);
        ++it;
        CHECK(*it == 30);
    }

    SUBCASE("front() and back() with complex objects")
    {
        struct TestObject
        {
            int id;
            std::string name;
            
            TestObject(int i, const std::string& n) : id(i), name(n) {}
            
            bool operator==(const TestObject& other) const
            {
                return id == other.id && name == other.name;
            }
        };
        
        gg::List<TestObject> list;
        list.emplace_back(1, "first");
        list.emplace_back(2, "second");
        list.emplace_back(3, "third");
        
        CHECK(list.front().id == 1);
        CHECK(list.front().name == "first");
        CHECK(list.back().id == 3);
        CHECK(list.back().name == "third");
        
        // Modify through references
        list.front().id = 10;
        list.front().name = "modified_first";
        list.back().id = 30;
        list.back().name = "modified_third";
        
        CHECK(list.front().id == 10);
        CHECK(list.front().name == "modified_first");
        CHECK(list.back().id == 30);
        CHECK(list.back().name == "modified_third");
        
        // Verify middle element unchanged
        auto it = list.begin();
        ++it;
        CHECK(it->id == 2);
        CHECK(it->name == "second");
    }
}

TEST_CASE("testing element access with different block sizes")
{
    SUBCASE("front() and back() with uint8_t block size")
    {
        gg::List<int, uint8_t> list;
        list.push_back(1);
        list.push_back(2);
        list.push_back(3);
        
        CHECK(list.front() == 1);
        CHECK(list.back() == 3);
        
        list.front() = 10;
        list.back() = 30;
        
        CHECK(list.front() == 10);
        CHECK(list.back() == 30);
    }

    SUBCASE("front() and back() with uint32_t block size")
    {
        gg::List<int, uint32_t> list;
        list.push_back(100);
        list.push_back(200);
        list.push_back(300);
        
        CHECK(list.front() == 100);
        CHECK(list.back() == 300);
        
        list.front() = 1000;
        list.back() = 3000;
        
        CHECK(list.front() == 1000);
        CHECK(list.back() == 3000);
    }

    SUBCASE("front() and back() with uint64_t block size")
    {
        gg::List<int, uint64_t> list;
        
        // Add many elements to test larger block handling
        for (int i = 1; i <= 100; ++i)
        {
            list.push_back(i);
        }
        
        CHECK(list.front() == 1);
        CHECK(list.back() == 100);
        
        list.front() = 999;
        list.back() = 888;
        
        CHECK(list.front() == 999);
        CHECK(list.back() == 888);
        
        // Verify size unchanged
        CHECK(list.size() == 100);
    }
}

TEST_CASE("testing element access edge cases")
{
    SUBCASE("element access after modifications")
    {
        gg::List<int> list;
        
        list.push_back(1);
        CHECK(list.front() == 1);
        CHECK(list.back() == 1);
        
        list.push_back(2);
        list.push_back(3);
        CHECK(list.front() == 1);
        CHECK(list.back() == 3);
        
        list.pop_front();
        CHECK(list.front() == 2);
        CHECK(list.back() == 3);
        
        list.pop_back();
        CHECK(list.front() == 2);
        CHECK(list.back() == 2);
    }

    SUBCASE("element access with large lists")
    {
        gg::List<int> list;
        
        // Create large list
        for (int i = 0; i < 1000; ++i)
        {
            list.push_back(i);
        }
        
        CHECK(list.front() == 0);
        CHECK(list.back() == 999);
        
        // Modify ends
        list.front() = -1;
        list.back() = 1001;
        
        CHECK(list.front() == -1);
        CHECK(list.back() == 1001);
        CHECK(list.size() == 1000);
        
        // Verify some middle elements unchanged
        auto it = list.begin();
        ++it;
        CHECK(*it == 1);
        
        auto it_end = list.end();
        --it_end;
        --it_end;
        CHECK(*it_end == 998);
    }

    SUBCASE("element access consistency after clear and rebuild")
    {
        gg::List<int> list;
        
        // Build initial list
        list.push_back(10);
        list.push_back(20);
        list.push_back(30);
        
        CHECK(list.front() == 10);
        CHECK(list.back() == 30);
        
        // Clear and rebuild
        list.clear();
        list.push_back(100);
        list.push_back(200);
        
        CHECK(list.front() == 100);
        CHECK(list.back() == 200);
        CHECK(list.size() == 2);
    }
}