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

TEST_CASE("testing basic list instantiation")
{
    SUBCASE("block size of 8")
    {
        gg::List<int, uint8_t> testList;
        CHECK(testList.size() == 0);
        CHECK(testList.empty() == true);

        auto itBegin = testList.begin();
        auto itEnd = testList.end();

        CHECK(itBegin == itEnd);

        //Try to erase begin iterator
        testList.erase(itBegin);
        CHECK(testList.size() == 0);
        CHECK(testList.empty() == true);

        itBegin = testList.begin();
        itEnd = testList.end();

        CHECK(itBegin == itEnd);

        //Try to erase end iterator
        testList.erase(itEnd);
        CHECK(testList.size() == 0);
        CHECK(testList.empty() == true);

        itBegin = testList.begin();
        itEnd = testList.end();

        CHECK(itBegin == itEnd);
    }

    SUBCASE("block size of 16")
    {
        gg::List<int, uint16_t> testList;
        CHECK(testList.size() == 0);
        CHECK(testList.empty() == true);

        auto itBegin = testList.begin();
        auto itEnd = testList.end();

        CHECK(itBegin == itEnd);

        //Try to erase begin iterator
        testList.erase(itBegin);
        CHECK(testList.size() == 0);
        CHECK(testList.empty() == true);

        itBegin = testList.begin();
        itEnd = testList.end();

        CHECK(itBegin == itEnd);

        //Try to erase end iterator
        testList.erase(itEnd);
        CHECK(testList.size() == 0);
        CHECK(testList.empty() == true);

        itBegin = testList.begin();
        itEnd = testList.end();

        CHECK(itBegin == itEnd);
    }

    SUBCASE("block size of 32")
    {
        gg::List<int, uint32_t> testList;
        CHECK(testList.size() == 0);
        CHECK(testList.empty() == true);

        auto itBegin = testList.begin();
        auto itEnd = testList.end();

        CHECK(itBegin == itEnd);

        //Try to erase begin iterator
        testList.erase(itBegin);
        CHECK(testList.size() == 0);
        CHECK(testList.empty() == true);

        itBegin = testList.begin();
        itEnd = testList.end();

        CHECK(itBegin == itEnd);

        //Try to erase end iterator
        testList.erase(itEnd);
        CHECK(testList.size() == 0);
        CHECK(testList.empty() == true);

        itBegin = testList.begin();
        itEnd = testList.end();

        CHECK(itBegin == itEnd);
    }

    SUBCASE("block size of 64")
    {
        gg::List<int, uint64_t> testList;
        CHECK(testList.size() == 0);
        CHECK(testList.empty() == true);

        auto itBegin = testList.begin();
        auto itEnd = testList.end();

        CHECK(itBegin == itEnd);

        //Try to erase begin iterator
        testList.erase(itBegin);
        CHECK(testList.size() == 0);
        CHECK(testList.empty() == true);

        itBegin = testList.begin();
        itEnd = testList.end();

        CHECK(itBegin == itEnd);

        //Try to erase end iterator
        testList.erase(itEnd);
        CHECK(testList.size() == 0);
        CHECK(testList.empty() == true);

        itBegin = testList.begin();
        itEnd = testList.end();

        CHECK(itBegin == itEnd);
    }
}
