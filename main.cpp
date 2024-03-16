#include <algorithm>

#include "C_list.hpp"
#include <list>
#include <vector>
#include <deque>
#include <chrono>
#include <fstream>
#include <memory>
#include <cstdint>
#include <cmath>
#include <cassert>
#include <iostream>
#include <array>

#if 1

struct Results
{
    std::string _name;
    std::vector<std::pair<uint64_t, uint64_t> > _data;
};

template<class TContainer>
void test_pushFront(char const* const containerName, Results& result, uint32_t nLogStart, uint32_t nLogStop, uint32_t nLogCount)
{
    std::cout << "test: pushFront, on " << containerName << std::endl;

    result._name = containerName;

    for (uint32_t iLogStep=nLogStart; iLogStep<nLogStop; ++iLogStep)
    {
        uint32_t logStep = static_cast<uint32_t>(std::pow(10, iLogStep+1))/nLogCount;

        for (uint32_t iLog=static_cast<uint32_t>(iLogStep!=nLogStart); iLog<nLogCount; ++iLog)
        {
            uint32_t iterations = static_cast<uint32_t>(std::pow(10, iLogStep)) + iLog * logStep;

            std::cout << "\titerations: " << iterations << " ";

            TContainer testContainer{};

            auto start = std::chrono::high_resolution_clock::now();
            for (uint32_t i = 0; i < iterations; ++i)
            {
                testContainer.push_front(typename TContainer::value_type{});
            }
            auto stop = std::chrono::high_resolution_clock::now();
            auto time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();

            std::cout << "time: " << time << '\n';

            result._data.emplace_back(iterations, time);
        }
    }

    std::cout << "---" << std::endl;
}
template<class TContainer>
void test_pushBack(char const* const containerName, Results& result, uint32_t nLogStart, uint32_t nLogStop, uint32_t nLogCount)
{
    std::cout << "test: pushBack, on " << containerName << std::endl;

    result._name = containerName;

    for (uint32_t iLogStep=nLogStart; iLogStep<nLogStop; ++iLogStep)
    {
        uint32_t logStep = static_cast<uint32_t>(std::pow(10, iLogStep+1))/nLogCount;

        for (uint32_t iLog=static_cast<uint32_t>(iLogStep!=nLogStart); iLog<nLogCount; ++iLog)
        {
            uint32_t iterations = static_cast<uint32_t>(std::pow(10, iLogStep)) + iLog * logStep;

            std::cout << "\titerations: " << iterations << " ";

            TContainer testContainer{};

            auto start = std::chrono::high_resolution_clock::now();
            for (uint32_t i = 0; i < iterations; ++i)
            {
                testContainer.push_back(typename TContainer::value_type{});
            }
            auto stop = std::chrono::high_resolution_clock::now();
            auto time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();

            std::cout << "time: " << time << '\n';

            result._data.emplace_back(iterations, time);
        }
    }

    std::cout << "---" << std::endl;
}
template<class TContainer>
void test_creation(char const* const containerName, Results& result, uint32_t nLogStart, uint32_t nLogStop, uint32_t nLogCount)
{
    std::cout << "test: creation, on " << containerName << std::endl;

    result._name = containerName;

    for (uint32_t iLogStep=nLogStart; iLogStep<nLogStop; ++iLogStep)
    {
        uint32_t logStep = static_cast<uint32_t>(std::pow(10, iLogStep+1))/nLogCount;

        for (uint32_t iLog=static_cast<uint32_t>(iLogStep!=nLogStart); iLog<nLogCount; ++iLog)
        {
            uint32_t iterations = static_cast<uint32_t>(std::pow(10, iLogStep)) + iLog * logStep;

            std::cout << "\titerations: " << iterations << " ";

            auto start = std::chrono::high_resolution_clock::now();
            TContainer testContainer{iterations};
            auto stop = std::chrono::high_resolution_clock::now();
            auto time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();

            std::cout << "time: " << time << '\n';

            result._data.emplace_back(iterations, time);
        }
    }

    std::cout << "---" << std::endl;
}
template<class TContainer>
void test_destruction(char const* const containerName, Results& result, uint32_t nLogStart, uint32_t nLogStop, uint32_t nLogCount)
{
    std::cout << "test: destruction, on " << containerName << std::endl;

    result._name = containerName;

    for (uint32_t iLogStep=nLogStart; iLogStep<nLogStop; ++iLogStep)
    {
        uint32_t logStep = static_cast<uint32_t>(std::pow(10, iLogStep+1))/nLogCount;

        for (uint32_t iLog=static_cast<uint32_t>(iLogStep!=nLogStart); iLog<nLogCount; ++iLog)
        {
            uint32_t iterations = static_cast<uint32_t>(std::pow(10, iLogStep)) + iLog * logStep;

            std::cout << "\titerations: " << iterations << " ";

            auto* testContainer = new TContainer{iterations};
            auto start = std::chrono::high_resolution_clock::now();
            delete testContainer;
            auto stop = std::chrono::high_resolution_clock::now();
            auto time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();

            std::cout << "time: " << time << '\n';

            result._data.emplace_back(iterations, time);
        }
    }

    std::cout << "---" << std::endl;
}
template<class TContainer>
void test_erase(char const* const containerName, Results& result, uint32_t nLogStart, uint32_t nLogStop, uint32_t nLogCount)
{
    std::cout << "test: erase, on " << containerName << std::endl;

    result._name = containerName;

    for (uint32_t iLogStep=nLogStart; iLogStep<nLogStop; ++iLogStep)
    {
        uint32_t logStep = static_cast<uint32_t>(std::pow(10, iLogStep+1))/nLogCount;

        for (uint32_t iLog=static_cast<uint32_t>(iLogStep!=nLogStart); iLog<nLogCount; ++iLog)
        {
            uint32_t iterations = static_cast<uint32_t>(std::pow(10, iLogStep)) + iLog * logStep;

            std::cout << "\titerations: " << iterations << " ";

            TContainer testContainer{iterations};
            auto start = std::chrono::high_resolution_clock::now();
            for (uint32_t i = 0; i < iterations; ++i)
            {
                auto itMiddle = testContainer.begin();
                for (std::size_t a=0; a<testContainer.size()/2; ++a)
                {
                    ++itMiddle;
                }
                testContainer.erase(itMiddle);
            }
            auto stop = std::chrono::high_resolution_clock::now();
            auto time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();

            std::cout << "time: " << time << '\n';

            result._data.emplace_back(iterations, time);
        }
    }

    std::cout << "---" << std::endl;
}
template<class TContainer>
void test_insert(char const* const containerName, Results& result, uint32_t nLogStart, uint32_t nLogStop, uint32_t nLogCount)
{
    std::cout << "test: insert, on " << containerName << std::endl;

    result._name = containerName;

    for (uint32_t iLogStep=nLogStart; iLogStep<nLogStop; ++iLogStep)
    {
        uint32_t logStep = static_cast<uint32_t>(std::pow(10, iLogStep+1))/nLogCount;

        for (uint32_t iLog=static_cast<uint32_t>(iLogStep!=nLogStart); iLog<nLogCount; ++iLog)
        {
            uint32_t iterations = static_cast<uint32_t>(std::pow(10, iLogStep)) + iLog * logStep;

            std::cout << "\titerations: " << iterations << " ";

            TContainer testContainer{iterations};

            auto itMiddle = testContainer.begin();
            for (std::size_t a=0; a<testContainer.size()/2; ++a)
            {
                ++itMiddle;
            }

            auto start = std::chrono::high_resolution_clock::now();
            for (uint32_t i = 0; i < iterations; ++i)
            {
                testContainer.insert(itMiddle, typename TContainer::value_type{});
            }
            auto stop = std::chrono::high_resolution_clock::now();
            auto time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();

            std::cout << "time: " << time << '\n';

            result._data.emplace_back(iterations, time);
        }
    }

    std::cout << "---" << std::endl;
}
template<class TContainer>
void test_iterations(char const* const containerName, Results& result, uint32_t nLogStart, uint32_t nLogStop, uint32_t nLogCount)
{
    std::cout << "test: iterations, on " << containerName << std::endl;

    result._name = containerName;

    for (uint32_t iLogStep=nLogStart; iLogStep<nLogStop; ++iLogStep)
    {
        uint32_t logStep = static_cast<uint32_t>(std::pow(10, iLogStep+1))/nLogCount;

        for (uint32_t iLog=static_cast<uint32_t>(iLogStep!=nLogStart); iLog<nLogCount; ++iLog)
        {
            uint32_t iterations = static_cast<uint32_t>(std::pow(10, iLogStep)) + iLog * logStep;

            std::cout << "\titerations: " << iterations << " ";

            TContainer testContainer{iterations};

            auto start = std::chrono::high_resolution_clock::now();
            for (auto it=testContainer.begin(); it!=testContainer.end(); ++it) {}
            auto stop = std::chrono::high_resolution_clock::now();
            auto time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();

            std::cout << "time: " << time << '\n';

            result._data.emplace_back(iterations, time);
        }
    }

    std::cout << "---" << std::endl;
}

void writeResults(const std::string& filename, const std::vector<Results>& resultsVector)
{
    std::ofstream csv{filename};

    if (!csv)
    {
        std::cout << "erreur c chiant merde !" << std::endl;
        return;
    }

    for (auto& results : resultsVector)
    {
        if (&results == &(*resultsVector.begin()))
        {
            csv << ',' << results._name << ',';
        }
        else
        {
            csv << results._name << ',';
        }
    }
    csv << std::endl;

    for (unsigned int i=0; i<resultsVector.back()._data.size(); ++i)
    {
        for (auto& results : resultsVector)
        {
            if (&results == &(*resultsVector.begin()))
            {
                csv << results._data[i].first << ',' << results._data[i].second << ',';
            }
            else
            {
                csv << results._data[i].second << ',';
            }
        }
        csv << std::endl;
    }
    csv << std::endl;

    csv.close();
}
#endif

int main()
{
    gg::List<int8_t, uint8_t> testList;

    std::cout << "size: " << sizeof(testList) << std::endl;
    std::cout << "std::list size: " << sizeof(std::list<int8_t>) << std::endl;

    testList.push_back(1);
    testList.push_back(2);
    testList.push_front(-1);
    testList.push_front(-2);
    testList.push_front(-3);
    testList.push_front(-4);
    //testList.push_front(-5);

    //testList.insert(++testList.begin(), 9);
    testList.insert(testList.begin(), 9);
    testList.insert(testList.begin(), 99);

    auto itInsert = testList.begin();
    testList.insert(itInsert, 15);
    testList.insert(itInsert, 30);
    testList.insert(--testList.end(), 77);

    for (auto it=testList.cbegin(); it!=testList.cend(); ++it)
    {
        if (*it == -2)
        {
            testList.erase(testList.erase(it));
            break;
        }
    }

    //std::reverse(testList.begin(), testList.end());

    for (auto it=testList.cbegin(); it!=testList.cend(); ++it)
    {
        std::cout << static_cast<int32_t>(*it) << std::endl;
    }

    std::cout << "begin: " << static_cast<int32_t>(*testList.begin()) << std::endl;
    std::cout << "end-1: " << static_cast<int32_t>(*--testList.end()) << std::endl;
    std::cout << "end-2: " << static_cast<int32_t>(*----testList.end()) << std::endl;
    std::cout << "final size: " << testList.size() << std::endl;

    return 0;

#ifdef NDEBUG
    std::string mode = "release_";
#else
    std::string mode = "debug_";
#endif

#if 1
    {
        std::vector<Results> results;

        results.emplace_back();
        test_pushBack<std::list<std::string> >("std::list<std::string>", results.back(), 2, 8, 10);

        results.emplace_back();
        test_pushBack<std::vector<std::string> >("std::vector<std::string>", results.back(), 2, 8, 10);

        results.emplace_back();
        test_pushBack<std::deque<std::string> >("std::deque<std::string>", results.back(), 2, 8, 10);

        results.emplace_back();
        test_pushBack<gg::List<std::string, uint8_t> >("gg::List<std::string uint8_t>", results.back(), 2, 8, 10);
        results.emplace_back();
        test_pushBack<gg::List<std::string, uint16_t> >("gg::List<std::string uint16_t>", results.back(), 2, 8, 10);
        results.emplace_back();
        test_pushBack<gg::List<std::string, uint32_t> >("gg::List<std::string uint32_t>", results.back(), 2, 8, 10);
        results.emplace_back();
        test_pushBack<gg::List<std::string, uint64_t> >("gg::List<std::string uint64_t>", results.back(), 2, 8, 10);

        writeResults(mode+"test_pushBack.csv", results);
    }
#endif

#if 1
    {
        std::vector<Results> results;

        results.emplace_back();
        test_creation<std::list<std::string> >("std::list<std::string>", results.back(), 2, 8, 10);

        results.emplace_back();
        test_creation<std::vector<std::string> >("std::vector<std::string>", results.back(), 2, 8, 10);

        results.emplace_back();
        test_creation<std::deque<std::string> >("std::deque<std::string>", results.back(), 2, 8, 10);

        results.emplace_back();
        test_creation<gg::List<std::string, uint8_t> >("gg::List<std::string uint8_t>", results.back(), 2, 8, 10);
        results.emplace_back();
        test_creation<gg::List<std::string, uint16_t> >("gg::List<std::string uint16_t>", results.back(), 2, 8, 10);
        results.emplace_back();
        test_creation<gg::List<std::string, uint32_t> >("gg::List<std::string uint32_t>", results.back(), 2, 8, 10);
        results.emplace_back();
        test_creation<gg::List<std::string, uint64_t> >("gg::List<std::string uint64_t>", results.back(), 2, 8, 10);

        writeResults(mode+"test_creation.csv", results);
    }
#endif

#if 1
    {
        std::vector<Results> results;

        results.emplace_back();
        test_destruction<std::list<std::string> >("std::list<std::string>", results.back(), 2, 8, 10);

        results.emplace_back();
        test_destruction<std::vector<std::string> >("std::vector<std::string>", results.back(), 2, 8, 10);

        results.emplace_back();
        test_destruction<std::deque<std::string> >("std::deque<std::string>", results.back(), 2, 8, 10);

        results.emplace_back();
        test_destruction<gg::List<std::string, uint8_t> >("gg::List<std::string uint8_t>", results.back(), 2, 8, 10);
        results.emplace_back();
        test_destruction<gg::List<std::string, uint16_t> >("gg::List<std::string uint16_t>", results.back(), 2, 8, 10);
        results.emplace_back();
        test_destruction<gg::List<std::string, uint32_t> >("gg::List<std::string uint32_t>", results.back(), 2, 8, 10);
        results.emplace_back();
        test_destruction<gg::List<std::string, uint64_t> >("gg::List<std::string uint64_t>", results.back(), 2, 8, 10);

        writeResults(mode+"test_destruction.csv", results);
    }
#endif

#if 1
    {
        std::vector<Results> results;

        results.emplace_back();
        test_pushFront<std::list<std::string> >("std::list<std::string>", results.back(), 2, 8, 10);

        results.emplace_back();
        test_pushFront<std::deque<std::string> >("std::deque<std::string>", results.back(), 2, 8, 10);

        results.emplace_back();
        test_pushFront<gg::List<std::string, uint8_t> >("gg::List<std::string uint8_t>", results.back(), 2, 8, 10);
        results.emplace_back();
        test_pushFront<gg::List<std::string, uint16_t> >("gg::List<std::string uint16_t>", results.back(), 2, 8, 10);
        results.emplace_back();
        test_pushFront<gg::List<std::string, uint32_t> >("gg::List<std::string uint32_t>", results.back(), 2, 8, 10);
        results.emplace_back();
        test_pushFront<gg::List<std::string, uint64_t> >("gg::List<std::string uint64_t>", results.back(), 2, 8, 10);

        writeResults(mode+"test_pushFront.csv", results);
    }
#endif

#if 1
    {
        std::vector<Results> results;

        results.emplace_back();
        test_erase<std::list<std::string> >("std::list<std::string>", results.back(), 2, 5, 10);

        results.emplace_back();
        test_erase<std::vector<std::string> >("std::vector<std::string>", results.back(), 2, 5, 10);

        results.emplace_back();
        test_erase<std::deque<std::string> >("std::deque<std::string>", results.back(), 2, 5, 10);

        results.emplace_back();
        test_erase<gg::List<std::string, uint8_t> >("gg::List<std::string uint8_t>", results.back(), 2, 5, 10);
        results.emplace_back();
        test_erase<gg::List<std::string, uint16_t> >("gg::List<std::string uint16_t>", results.back(), 2, 5, 10);
        results.emplace_back();
        test_erase<gg::List<std::string, uint32_t> >("gg::List<std::string uint32_t>", results.back(), 2, 5, 10);
        results.emplace_back();
        test_erase<gg::List<std::string, uint64_t> >("gg::List<std::string uint64_t>", results.back(), 2, 5, 10);

        writeResults(mode+"test_erase.csv", results);
    }
#endif

#if 1
    {
        std::vector<Results> results;

        results.emplace_back();
        test_iterations<std::list<std::string> >("std::list<std::string>", results.back(), 2, 8, 10);

        results.emplace_back();
        test_iterations<std::vector<std::string> >("std::vector<std::string>", results.back(), 2, 8, 10);

        results.emplace_back();
        test_iterations<std::deque<std::string> >("std::deque<std::string>", results.back(), 2, 8, 10);

        results.emplace_back();
        test_iterations<gg::List<std::string, uint8_t> >("gg::List<std::string uint8_t>", results.back(), 2, 6, 10);
        results.emplace_back();
        test_iterations<gg::List<std::string, uint16_t> >("gg::List<std::string uint16_t>", results.back(), 2, 6, 10);
        results.emplace_back();
        test_iterations<gg::List<std::string, uint32_t> >("gg::List<std::string uint32_t>", results.back(), 2, 6, 10);
        results.emplace_back();
        test_iterations<gg::List<std::string, uint64_t> >("gg::List<std::string uint64_t>", results.back(), 2, 6, 10);

        writeResults(mode+"test_iterations.csv", results);
    }
#endif

#if 1
    {
        std::vector<Results> results;

        results.emplace_back();
        test_insert<std::list<std::string> >("std::list<std::string>", results.back(), 2, 7, 10);
        results.emplace_back();
        test_insert<std::list<uint32_t> >("std::list<uint32_t>", results.back(), 2, 7, 10);

        //results.emplace_back();
        //test_insert<std::vector<std::string> >("std::vector<std::string>", results.back(), 2, 5, 10);
        //results.emplace_back();
        //test_insert<std::vector<uint32_t> >("std::vector<uint32_t>", results.back(), 2, 5, 10);

        //results.emplace_back();
        //test_insert<std::deque<std::string> >("std::deque<std::string>", results.back(), 2, 5, 10);
        //results.emplace_back();
        //test_insert<std::deque<uint32_t> >("std::deque<uint32_t>", results.back(), 2, 5, 10);

        results.emplace_back();
        test_insert<gg::List<uint32_t, uint32_t> >("gg::List<uint32_t uint16_t>", results.back(), 2, 7, 10);

        results.emplace_back();
        test_insert<gg::List<std::string, uint8_t> >("gg::List<std::string uint8_t>", results.back(), 2, 7, 10);
        results.emplace_back();
        test_insert<gg::List<std::string, uint16_t> >("gg::List<std::string uint16_t>", results.back(), 2, 7, 10);
        results.emplace_back();
        test_insert<gg::List<std::string, uint32_t> >("gg::List<std::string uint32_t>", results.back(), 2, 7, 10);
        results.emplace_back();
        test_insert<gg::List<std::string, uint64_t> >("gg::List<std::string uint64_t>", results.back(), 2, 7, 10);

        writeResults(mode+"test_insert.csv", results);
    }
#endif

    return 0;
}
