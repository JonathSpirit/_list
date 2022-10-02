#include <iostream>
#include "C_list.hpp"
#include <list>
#include <vector>
#include <deque>
#include <chrono>
#include <fstream>
#include <memory>
#include <cmath>
#include <boost/type_index.hpp>
#include <algorithm>

struct Results
{
    std::string _name;
    std::vector<std::pair<uint64_t, uint64_t> > _data;
};

template<class TContainer>
void test_pushBack_delete(Results& resultPushBack, Results& resultDelete, unsigned int n, unsigned int nStart)
{
    std::string name = boost::typeindex::type_id<TContainer>().pretty_name();
    std::replace( name.begin(), name.end(), ',', ' ');

    std::cout << name << std::endl;

    resultPushBack._name = name;
    resultDelete._name = name;

    for (unsigned int in=nStart; in<n; ++in)
    {
        for (unsigned int ilog=1; ilog<10; ++ilog)
        {
            uint64_t iterations = std::pow(10, in)*ilog;

            std::cout << "\titerations: " << iterations << " ";

            auto* testContainer = new TContainer{};

            auto start = std::chrono::high_resolution_clock::now();
            for (uint64_t i = 0; i < iterations; ++i)
            {
                testContainer->push_back({});
            }
            auto stop = std::chrono::high_resolution_clock::now();
            uint64_t tms = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();

            std::cout << "time_push_back: " << tms << ' ';

            resultPushBack._data.emplace_back(iterations, tms);

            start = std::chrono::high_resolution_clock::now();
            delete testContainer;
            stop = std::chrono::high_resolution_clock::now();
            tms = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();

            std::cout << "time_delete: " << tms << '\n';

            resultDelete._data.emplace_back(iterations, tms);
        }
    }

    std::cout << "---" << std::endl;
}

template<class TContainer>
void test_erase(Results& resultErase, unsigned int n, unsigned int nStart)
{
    std::string name = boost::typeindex::type_id<TContainer>().pretty_name();
    std::replace( name.begin(), name.end(), ',', ' ');

    std::cout << name << std::endl;

    resultErase._name = name;

    for (unsigned int in=nStart; in<n; ++in)
    {
        for (unsigned int ilog=1; ilog<10; ++ilog)
        {
            uint64_t iterations = std::pow(10, in)*ilog;

            std::cout << "\titerations: " << iterations << " ";

            auto* testContainer = new TContainer{};

            for (uint64_t i = 0; i < iterations; ++i)
            {
                testContainer->push_back({});
            }

            auto start = std::chrono::high_resolution_clock::now();
            uint64_t a=0;
            for (auto it = testContainer->begin(); it != testContainer->end();)
            {
                if (a%20 == 0)
                {
                    it = testContainer->erase(it);
                }
                else
                {
                    ++it;
                }
                ++a;
            }
            auto stop = std::chrono::high_resolution_clock::now();
            uint64_t tms = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();

            resultErase._data.emplace_back(iterations, tms);

            std::cout << "time: " << tms << '\n';

            delete testContainer;
        }
    }

    std::cout << "---" << std::endl;
}

template<class TContainer>
void test_insert(Results& resultInsert, unsigned int n, unsigned int nStart)
{
    std::string name = boost::typeindex::type_id<TContainer>().pretty_name();
    std::replace( name.begin(), name.end(), ',', ' ');

    std::cout << name << std::endl;

    resultInsert._name = name;

    for (unsigned int in=nStart; in<n; ++in)
    {
        for (unsigned int ilog=1; ilog<10; ++ilog)
        {
            uint64_t iterations = std::pow(10, in)*ilog;

            std::cout << "\titerations: " << iterations << " ";

            auto* testContainer = new TContainer{};

            auto start = std::chrono::high_resolution_clock::now();
            for (uint64_t i = 0; i < iterations; ++i)
            {
                testContainer->insert(testContainer->begin(), {});
                testContainer->insert(testContainer->end(), {});
            }
            auto stop = std::chrono::high_resolution_clock::now();
            uint64_t tms = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();

            resultInsert._data.emplace_back(iterations, tms);

            std::cout << "time: " << tms << '\n';

            delete testContainer;
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

int main()
{
#if 0
    gg::List<uint8_t, uint32_t> test;

    gg::List<uint8_t, uint32_t>::iterator it2;
    for (int i=0; i<32; ++i)
    {
        //auto it = test.begin();
        test.push_front(i);
        if (i == 15)
        {
            it2 = test.begin();
        }
        //test.insert(it, 99);
    }

    test.erase(it2);
    test.push_back(99);

    /*test.push_front(42);
    auto itTest = test.begin();
    test.erase(itTest);

    test.push_back(69);
    test.push_back(96);

    itTest = test.begin();

    test.push_front(111);
    test.push_front(112);

    test.insert(itTest, 113);

    std::cout << "---------------" << std::endl;*/

    for (auto it=test.begin(); it!=test.end(); ++it)
    {
        int a = *it;
        std::cout << a << std::endl;
    }

    /*std::cout << "......." << std::endl;

    auto itTest2 = test.begin();
    std::cout << (int)*itTest2 << std::endl;
    ++itTest2;
    std::cout << (int)*itTest2 << std::endl;
    --itTest2;
    std::cout << (int)*itTest2 << std::endl;*/


    return 0;
#endif

    ///TESTING
#if 0
        std::vector<Results> results1;
        std::vector<Results> results2;

        results1.emplace_back();
        results2.emplace_back();
        test_pushBack_delete<std::list<std::string> >(results1.back(), results2.back(), 8, 3);

        results1.emplace_back();
        results2.emplace_back();
        test_pushBack_delete<std::vector<std::string> >(results1.back(), results2.back(), 8, 3);

        results1.emplace_back();
        results2.emplace_back();
        test_pushBack_delete<std::deque<std::string> >(results1.back(), results2.back(), 8, 3);

        results1.emplace_back();
        results2.emplace_back();
        test_pushBack_delete<gg::List<std::string, uint32_t> >(results1.back(), results2.back(), 8, 3);

        results1.emplace_back();
        results2.emplace_back();
        test_pushBack_delete<gg::List<std::string, uint8_t> >(results1.back(), results2.back(), 8, 3);

        writeResults("test_push_back.csv", results1);
        writeResults("test_delete.csv", results2);
    }
#endif

#if 0
    {
        std::vector<Results> results;

        results.emplace_back();
        test_erase<std::list<std::string> >(results.back(), 8, 3);

        //results.emplace_back();
        //test_erase<std::vector<std::string> >(results.back(), 8, 3);

        //results.emplace_back();
        //test_erase<std::deque<std::string> >(results.back(), 8, 3);

        results.emplace_back();
        test_erase<gg::List<std::string, uint8_t> >(results.back(), 8, 3);

        results.emplace_back();
        test_erase<gg::List<std::string, uint16_t> >(results.back(), 8, 3);

        results.emplace_back();
        test_erase<gg::List<std::string, uint32_t> >(results.back(), 8, 3);

        results.emplace_back();
        test_erase<gg::List<std::string, uint64_t> >(results.back(), 8, 3);

        writeResults("test_erase.csv", results);
    }
#endif

#if 1
    gg::List<std::string, uint8_t> gggg;
    {
        std::vector<Results> results;

        results.emplace_back();
        test_insert<std::list<std::string> >(results.back(), 8, 3);

        results.emplace_back();
        test_insert<std::vector<std::string> >(results.back(), 8, 3);

        results.emplace_back();
        test_insert<std::deque<std::string> >(results.back(), 8, 3);

        results.emplace_back();
        test_insert<gg::List<std::string, uint8_t> >(results.back(), 8, 3);

        results.emplace_back();
        test_insert<gg::List<std::string, uint16_t> >(results.back(), 8, 3);

        results.emplace_back();
        test_insert<gg::List<std::string, uint32_t> >(results.back(), 8, 3);

        results.emplace_back();
        test_insert<gg::List<std::string, uint64_t> >(results.back(), 8, 3);

        writeResults("test_insert.csv", results);
    }
#endif

#if 0
    {
        std::vector<Results> results1;
        std::vector<Results> results2;

        results1.emplace_back();
        results2.emplace_back();
        test_pushBack_delete<gg::List<std::string, uint8_t> >(results1.back(), results2.back(), 8, 3);

        results1.emplace_back();
        results2.emplace_back();
        test_pushBack_delete<gg::List<std::string, uint16_t> >(results1.back(), results2.back(), 8, 3);

        results1.emplace_back();
        results2.emplace_back();
        test_pushBack_delete<gg::List<std::string, uint32_t> >(results1.back(), results2.back(), 8, 3);

        results1.emplace_back();
        results2.emplace_back();
        test_pushBack_delete<gg::List<std::string, uint64_t> >(results1.back(), results2.back(), 8, 3);

        writeResults("test_push_back_gg.csv", results1);
        writeResults("test_delete_gg.csv", results2);

        results1.clear();

        results1.emplace_back();
        test_erase<gg::List<std::string, uint8_t> >(results1.back(), 8, 3);

        results1.emplace_back();
        test_erase<gg::List<std::string, uint16_t> >(results1.back(), 8, 3);

        results1.emplace_back();
        test_erase<gg::List<std::string, uint32_t> >(results1.back(), 8, 3);

        results1.emplace_back();
        test_erase<gg::List<std::string, uint64_t> >(results1.back(), 8, 3);

        writeResults("test_erase_gg.csv", results1);
    }
#endif

    return 0;
}
