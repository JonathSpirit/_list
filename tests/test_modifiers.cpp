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
#include <utility>
#include <vector>

// Helper class for testing emplace operations
class TestObject
{
public:
    int value;
    std::string text;
    
    TestObject() : value(0), text("default") {}
    TestObject(int v) : value(v), text("int_ctor") {}
    TestObject(int v, const std::string& t) : value(v), text(t) {}
    TestObject(const TestObject& other) : value(other.value), text(other.text + "_copied") {}
    TestObject(TestObject&& other) noexcept : value(other.value), text(std::move(other.text) + "_moved") {}
    
    TestObject& operator=(const TestObject& other)
    {
        if (this != &other)
        {
            value = other.value;
            text = other.text + "_copy_assigned";
        }
        return *this;
    }
    
    TestObject& operator=(TestObject&& other) noexcept
    {
        if (this != &other)
        {
            value = other.value;
            text = std::move(other.text) + "_move_assigned";
        }
        return *this;
    }
    
    bool operator==(const TestObject& other) const
    {
        return value == other.value && text == other.text;
    }
};

TEST_CASE("testing push_back operations")
{
    SUBCASE("push_back to empty list")
    {
        gg::List<int> list;
        
        list.push_back(42);
        
        CHECK(list.size() == 1);
        CHECK(list.empty() == false);
        CHECK(*list.begin() == 42);
    }

    SUBCASE("push_back multiple elements")
    {
        gg::List<int> list;
        
        list.push_back(1);
        list.push_back(2);
        list.push_back(3);
        
        CHECK(list.size() == 3);
        CHECK(list.empty() == false);
        
        std::vector<int> expected = {1, 2, 3};
        auto expectedIt = expected.begin();
        for (const auto& elem : list)
        {
            CHECK(elem == *expectedIt);
            ++expectedIt;
        }
    }

    SUBCASE("push_back with copy semantics")
    {
        gg::List<TestObject> list;
        
        TestObject obj(10, "test");
        list.push_back(obj);
        
        CHECK(list.size() == 1);
        
        auto& stored = *list.begin();
        CHECK(stored.value == 10);
        CHECK(stored.text == "test_copied");  // Should be copied
    }

    SUBCASE("push_back with move semantics")
    {
        gg::List<TestObject> list;
        
        TestObject obj(20, "move_test");
        list.push_back(std::move(obj));
        
        CHECK(list.size() == 1);
        
        auto& stored = *list.begin();
        CHECK(stored.value == 20);
        CHECK(stored.text == "move_test_moved");  // Should be moved
    }

    SUBCASE("push_back strings")
    {
        gg::List<std::string> list;
        
        list.push_back("hello");
        list.push_back("world");
        
        CHECK(list.size() == 2);
        
        auto it = list.begin();
        CHECK(*it == "hello");
        ++it;
        CHECK(*it == "world");
    }

    SUBCASE("push_back many elements")
    {
        gg::List<int> list;
        
        // Test with more elements than might fit in a single block
        for (int i = 0; i < 100; ++i)
        {
            list.push_back(i);
        }
        
        CHECK(list.size() == 100);
        
        int expected = 0;
        for (const auto& elem : list)
        {
            CHECK(elem == expected);
            ++expected;
        }
    }
}

TEST_CASE("testing push_front operations")
{
    SUBCASE("push_front to empty list")
    {
        gg::List<int> list;
        
        list.push_front(42);
        
        CHECK(list.size() == 1);
        CHECK(list.empty() == false);
        CHECK(*list.begin() == 42);
    }

    SUBCASE("push_front multiple elements")
    {
        gg::List<int> list;
        
        list.push_front(1);
        list.push_front(2);
        list.push_front(3);
        
        CHECK(list.size() == 3);
        CHECK(list.empty() == false);
        
        // Elements should be in reverse order of insertion
        std::vector<int> expected = {3, 2, 1};
        auto expectedIt = expected.begin();
        for (const auto& elem : list)
        {
            CHECK(elem == *expectedIt);
            ++expectedIt;
        }
    }

    SUBCASE("push_front with copy semantics")
    {
        gg::List<TestObject> list;
        
        TestObject obj(15, "front_test");
        list.push_front(obj);
        
        CHECK(list.size() == 1);
        
        auto& stored = *list.begin();
        CHECK(stored.value == 15);
        CHECK(stored.text == "front_test_copied");  // Should be copied
    }

    SUBCASE("push_front with move semantics")
    {
        gg::List<TestObject> list;
        
        TestObject obj(25, "front_move");
        list.push_front(std::move(obj));
        
        CHECK(list.size() == 1);
        
        auto& stored = *list.begin();
        CHECK(stored.value == 25);
        CHECK(stored.text == "front_move_moved");  // Should be moved
    }

    SUBCASE("mixed push_front and push_back")
    {
        gg::List<int> list;
        
        list.push_back(2);    // [2]
        list.push_front(1);   // [1, 2]
        list.push_back(3);    // [1, 2, 3]
        list.push_front(0);   // [0, 1, 2, 3]
        
        CHECK(list.size() == 4);
        
        std::vector<int> expected = {0, 1, 2, 3};
        auto expectedIt = expected.begin();
        for (const auto& elem : list)
        {
            CHECK(elem == *expectedIt);
            ++expectedIt;
        }
    }

    SUBCASE("push_front many elements")
    {
        gg::List<int> list;
        
        // Test with more elements than might fit in a single block
        for (int i = 0; i < 50; ++i)
        {
            list.push_front(i);
        }
        
        CHECK(list.size() == 50);
        
        // Elements should be in reverse order
        int expected = 49;
        for (const auto& elem : list)
        {
            CHECK(elem == expected);
            --expected;
        }
    }
}

TEST_CASE("testing emplace_back operations")
{
    SUBCASE("emplace_back simple type")
    {
        gg::List<int> list;
        
        auto& ref = list.emplace_back(42);
        
        CHECK(list.size() == 1);
        CHECK(ref == 42);
        CHECK(*list.begin() == 42);
    }

    SUBCASE("emplace_back with constructor arguments")
    {
        gg::List<TestObject> list;
        
        auto& ref = list.emplace_back(10, "emplaced");
        
        CHECK(list.size() == 1);
        CHECK(ref.value == 10);
        CHECK(ref.text == "emplaced");
        
        auto& stored = *list.begin();
        CHECK(stored.value == 10);
        CHECK(stored.text == "emplaced");
    }

    SUBCASE("emplace_back default constructor")
    {
        gg::List<TestObject> list;
        
        auto& ref = list.emplace_back();
        
        CHECK(list.size() == 1);
        CHECK(ref.value == 0);
        CHECK(ref.text == "default");
    }

    SUBCASE("emplace_back single argument constructor")
    {
        gg::List<TestObject> list;
        
        auto& ref = list.emplace_back(99);
        
        CHECK(list.size() == 1);
        CHECK(ref.value == 99);
        CHECK(ref.text == "int_ctor");
    }

    SUBCASE("emplace_back multiple elements")
    {
        gg::List<TestObject> list;
        
        list.emplace_back(1, "first");
        list.emplace_back(2, "second");
        list.emplace_back(3, "third");
        
        CHECK(list.size() == 3);
        
        std::vector<std::pair<int, std::string>> expected = {
            {1, "first"}, {2, "second"}, {3, "third"}
        };
        
        auto expectedIt = expected.begin();
        for (const auto& elem : list)
        {
            CHECK(elem.value == expectedIt->first);
            CHECK(elem.text == expectedIt->second);
            ++expectedIt;
        }
    }

    SUBCASE("emplace_back strings")
    {
        gg::List<std::string> list;
        
        auto& ref1 = list.emplace_back("hello");
        auto& ref2 = list.emplace_back(5, 'a');  // String constructor with count and char
        
        CHECK(list.size() == 2);
        CHECK(ref1 == "hello");
        CHECK(ref2 == "aaaaa");
        
        auto it = list.begin();
        CHECK(*it == "hello");
        ++it;
        CHECK(*it == "aaaaa");
    }
}

TEST_CASE("testing emplace_front operations")
{
    SUBCASE("emplace_front simple type")
    {
        gg::List<int> list;
        
        auto& ref = list.emplace_front(42);
        
        CHECK(list.size() == 1);
        CHECK(ref == 42);
        CHECK(*list.begin() == 42);
    }

    SUBCASE("emplace_front with constructor arguments")
    {
        gg::List<TestObject> list;
        
        auto& ref = list.emplace_front(20, "front_emplaced");
        
        CHECK(list.size() == 1);
        CHECK(ref.value == 20);
        CHECK(ref.text == "front_emplaced");
        
        auto& stored = *list.begin();
        CHECK(stored.value == 20);
        CHECK(stored.text == "front_emplaced");
    }

    SUBCASE("emplace_front multiple elements")
    {
        gg::List<TestObject> list;
        
        list.emplace_front(1, "first");
        list.emplace_front(2, "second");
        list.emplace_front(3, "third");
        
        CHECK(list.size() == 3);
        
        // Elements should be in reverse order of insertion
        std::vector<std::pair<int, std::string>> expected = {
            {3, "third"}, {2, "second"}, {1, "first"}
        };
        
        auto expectedIt = expected.begin();
        for (const auto& elem : list)
        {
            CHECK(elem.value == expectedIt->first);
            CHECK(elem.text == expectedIt->second);
            ++expectedIt;
        }
    }

    SUBCASE("mixed emplace operations")
    {
        gg::List<TestObject> list;
        
        list.emplace_back(2, "back");     // [TestObject(2, "back")]
        list.emplace_front(1, "front");   // [TestObject(1, "front"), TestObject(2, "back")]
        list.emplace_back(3, "back2");    // [TestObject(1, "front"), TestObject(2, "back"), TestObject(3, "back2")]
        
        CHECK(list.size() == 3);
        
        auto it = list.begin();
        CHECK(it->value == 1);
        CHECK(it->text == "front");
        ++it;
        CHECK(it->value == 2);
        CHECK(it->text == "back");
        ++it;
        CHECK(it->value == 3);
        CHECK(it->text == "back2");
    }
}

TEST_CASE("testing pop operations")
{
    SUBCASE("pop_back single element")
    {
        gg::List<int> list;
        list.push_back(42);
        
        CHECK(list.size() == 1);
        
        list.pop_back();
        
        CHECK(list.size() == 0);
        CHECK(list.empty() == true);
        CHECK(list.begin() == list.end());
    }

    SUBCASE("pop_back multiple elements")
    {
        gg::List<int> list;
        list.push_back(1);
        list.push_back(2);
        list.push_back(3);
        
        CHECK(list.size() == 3);
        
        list.pop_back();
        CHECK(list.size() == 2);
        
        auto it = list.begin();
        CHECK(*it == 1);
        ++it;
        CHECK(*it == 2);
        
        list.pop_back();
        CHECK(list.size() == 1);
        CHECK(*list.begin() == 1);
        
        list.pop_back();
        CHECK(list.size() == 0);
        CHECK(list.empty() == true);
    }

    SUBCASE("pop_front single element")
    {
        gg::List<int> list;
        list.push_back(42);
        
        CHECK(list.size() == 1);
        
        list.pop_front();
        
        CHECK(list.size() == 0);
        CHECK(list.empty() == true);
        CHECK(list.begin() == list.end());
    }

    SUBCASE("pop_front multiple elements")
    {
        gg::List<int> list;
        list.push_back(1);
        list.push_back(2);
        list.push_back(3);
        
        CHECK(list.size() == 3);
        
        list.pop_front();
        CHECK(list.size() == 2);
        
        auto it = list.begin();
        CHECK(*it == 2);
        ++it;
        CHECK(*it == 3);
        
        list.pop_front();
        CHECK(list.size() == 1);
        CHECK(*list.begin() == 3);
        
        list.pop_front();
        CHECK(list.size() == 0);
        CHECK(list.empty() == true);
    }

    SUBCASE("mixed pop operations")
    {
        gg::List<int> list;
        for (int i = 1; i <= 5; ++i)
        {
            list.push_back(i);  // [1, 2, 3, 4, 5]
        }
        
        list.pop_front();  // [2, 3, 4, 5]
        list.pop_back();   // [2, 3, 4]
        list.pop_front();  // [3, 4]
        
        CHECK(list.size() == 2);
        
        auto it = list.begin();
        CHECK(*it == 3);
        ++it;
        CHECK(*it == 4);
    }

    SUBCASE("pop with custom objects")
    {
        gg::List<TestObject> list;
        list.emplace_back(1, "first");
        list.emplace_back(2, "second");
        list.emplace_back(3, "third");
        
        CHECK(list.size() == 3);
        
        list.pop_back();
        CHECK(list.size() == 2);
        
        auto it = list.begin();
        CHECK(it->value == 1);
        CHECK(it->text == "first");
        ++it;
        CHECK(it->value == 2);
        CHECK(it->text == "second");
        
        list.pop_front();
        CHECK(list.size() == 1);
        CHECK(list.begin()->value == 2);
        CHECK(list.begin()->text == "second");
    }

    SUBCASE("pop and push interleaved")
    {
        gg::List<int> list;
        
        list.push_back(1);
        list.push_back(2);
        CHECK(list.size() == 2);
        
        list.pop_front();  // [2]
        CHECK(list.size() == 1);
        CHECK(*list.begin() == 2);
        
        list.push_front(0);  // [0, 2]
        CHECK(list.size() == 2);
        
        list.pop_back();   // [0]
        CHECK(list.size() == 1);
        CHECK(*list.begin() == 0);
        
        list.push_back(3);  // [0, 3]
        CHECK(list.size() == 2);
        
        auto it = list.begin();
        CHECK(*it == 0);
        ++it;
        CHECK(*it == 3);
    }
}

TEST_CASE("testing large scale operations")
{
    SUBCASE("large push operations")
    {
        gg::List<int> list;
        
        // Push many elements to both ends
        for (int i = 0; i < 100; ++i)
        {
            list.push_back(i);
            list.push_front(-i-1);
        }
        
        CHECK(list.size() == 200);
        
        // Verify the pattern: [-100, -99, ..., -1, 0, 1, 2, ..., 99]
        auto it = list.begin();
        for (int i = -100; i < 100; ++i)
        {
            CHECK(*it == i);
            ++it;
        }
    }

    SUBCASE("large emplace operations")
    {
        gg::List<std::string> list;
        
        for (int i = 0; i < 50; ++i)
        {
            list.emplace_back(std::to_string(i));
            list.emplace_front(std::to_string(-i-1));
        }
        
        CHECK(list.size() == 100);
        
        // Verify the pattern
        auto it = list.begin();
        for (int i = -50; i < 50; ++i)
        {
            CHECK(*it == std::to_string(i));
            ++it;
        }
    }

    SUBCASE("large mixed operations")
    {
        gg::List<int> list;
        
        // Build up
        for (int i = 0; i < 50; ++i)
        {
            list.push_back(i);
        }
        
        // Tear down from both ends
        for (int i = 0; i < 20; ++i)
        {
            list.pop_front();
            list.pop_back();
        }
        
        CHECK(list.size() == 10);
        
        // Should have elements [20, 21, 22, ..., 29]
        int expected = 20;
        for (const auto& elem : list)
        {
            CHECK(elem == expected);
            ++expected;
        }
    }
}