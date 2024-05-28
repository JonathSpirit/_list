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
#include <iostream>
#include <array>

#include <matplot/matplot.h>

#define FIG_SIZE_W 800
#define FIG_SIZE_H 600

using namespace matplot;

using LogSteps = std::vector<uint32_t>;

LogSteps BuildLogStep(uint32_t nLogStart, uint32_t nLogStop, uint32_t nLogCount)
{
    auto const dresult = logspace(nLogStart, nLogStop, nLogCount);
    LogSteps result(dresult.size());
    for (std::size_t i=0; i<dresult.size(); ++i)
    {
        result[i] = static_cast<LogSteps::value_type>(dresult[i]);
    }
    return result;
}

template<class TContainer>
void test_pushFront(std::string_view containerName, LogSteps const& steps)
{
    std::cout << "test: pushFront, on " << containerName << std::endl;

    std::vector<uint64_t> resultX;
    std::vector<uint64_t> resultY;

    for (auto const iterations : steps)
    {
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

        resultX.emplace_back(iterations);
        resultY.emplace_back(time);
    }

    auto handle = semilogx(resultX, resultY);
    handle->display_name(containerName);
    handle->line_width(3.0f);
    std::cout << "---" << std::endl;
}
template<class TContainer>
void test_pushBack(std::string_view containerName, LogSteps const& steps)
{
    std::cout << "test: pushBack, on " << containerName << std::endl;

    std::vector<uint64_t> resultX;
    std::vector<uint64_t> resultY;

    for (auto const iterations : steps)
    {
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

        resultX.emplace_back(iterations);
        resultY.emplace_back(time);
    }

    auto handle = semilogx(resultX, resultY);
    handle->display_name(containerName);
    handle->line_width(3.0f);
    std::cout << "---" << std::endl;
}
template<class TContainer>
void test_creation(std::string_view containerName, LogSteps const& steps)
{
    std::cout << "test: creation, on " << containerName << std::endl;

    std::vector<uint64_t> resultX;
    std::vector<uint64_t> resultY;

    for (auto const iterations : steps)
    {
        std::cout << "\titerations: " << iterations << " ";

        auto start = std::chrono::high_resolution_clock::now();
        TContainer testContainer{iterations};
        auto stop = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();

        std::cout << "time: " << time << '\n';

        resultX.emplace_back(iterations);
        resultY.emplace_back(time);
    }

    auto handle = semilogx(resultX, resultY);
    handle->display_name(containerName);
    handle->line_width(3.0f);
    std::cout << "---" << std::endl;
}
template<class TContainer>
void test_destruction(std::string_view containerName, LogSteps const& steps)
{
    std::cout << "test: destruction, on " << containerName << std::endl;

    std::vector<uint64_t> resultX;
    std::vector<uint64_t> resultY;

    for (auto const iterations : steps)
    {
        std::cout << "\titerations: " << iterations << " ";

        auto* testContainer = new TContainer{iterations};
        auto start = std::chrono::high_resolution_clock::now();
        delete testContainer;
        auto stop = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();

        std::cout << "time: " << time << '\n';

        resultX.emplace_back(iterations);
        resultY.emplace_back(time);
    }

    auto handle = semilogx(resultX, resultY);
    handle->display_name(containerName);
    handle->line_width(3.0f);
    std::cout << "---" << std::endl;
}
template<class TContainer>
void test_erase(std::string_view containerName, LogSteps const& steps)
{
    std::cout << "test: erase, on " << containerName << std::endl;

    std::vector<uint64_t> resultX;
    std::vector<uint64_t> resultY;

    for (auto const iterations : steps)
    {
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

        resultX.emplace_back(iterations);
        resultY.emplace_back(time);
    }

    auto handle = semilogx(resultX, resultY);
    handle->display_name(containerName);
    handle->line_width(3.0f);
    std::cout << "---" << std::endl;
}
template<class TContainer>
void test_insert(std::string_view containerName, LogSteps const& steps)
{
    std::cout << "test: insert, on " << containerName << std::endl;

    std::vector<uint64_t> resultX;
    std::vector<uint64_t> resultY;

    for (auto const iterations : steps)
    {
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

        resultX.emplace_back(iterations);
        resultY.emplace_back(time);
    }

    auto handle = semilogx(resultX, resultY);
    handle->display_name(containerName);
    handle->line_width(3.0f);
    std::cout << "---" << std::endl;
}
template<class TContainer>
void test_iterations(std::string_view containerName, LogSteps const& steps)
{
    std::cout << "test: iterations, on " << containerName << std::endl;

    std::vector<uint64_t> resultX;
    std::vector<uint64_t> resultY;

    for (auto const iterations : steps)
    {
        std::cout << "\titerations: " << iterations << " ";

        TContainer testContainer{iterations};

        auto start = std::chrono::high_resolution_clock::now();
        for (auto it=testContainer.begin(); it!=testContainer.end(); ++it) {}
        auto stop = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();

        std::cout << "time: " << time << '\n';

        resultX.emplace_back(iterations);
        resultY.emplace_back(time);
    }

    auto handle = semilogx(resultX, resultY);
    handle->display_name(containerName);
    handle->line_width(3.0f);
    std::cout << "---" << std::endl;
}

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

    //return 0;

#ifdef NDEBUG
    std::string mode = "release";
#else
    std::string mode = "debug";
#endif

#if 1
    {
        figure()->size(FIG_SIZE_W, FIG_SIZE_H);
        title("test: pushback "+mode);
        auto cfgLegend = legend({});
        cfgLegend->box(false);
        cfgLegend->horizontal_location(legend::horizontal_alignment::left);
        grid(true);
        hold(true);
        xlabel("iteration");
        ylabel("time us");

        auto const steps = BuildLogStep(5, 8, 10);
        xrange({static_cast<double>(steps.front()), static_cast<double>(steps.back())});

        test_pushBack<std::list<std::string> >("std::list<std::string>", steps);

        test_pushBack<std::vector<std::string> >("std::vector<std::string>", steps);

        test_pushBack<std::deque<std::string> >("std::deque<std::string>", steps);

        test_pushBack<gg::List<std::string, uint8_t> >("gg::List<std::string uint8_t>", steps);
        test_pushBack<gg::List<std::string, uint16_t> >("gg::List<std::string uint16_t>", steps);
        test_pushBack<gg::List<std::string, uint32_t> >("gg::List<std::string uint32_t>", steps);
        test_pushBack<gg::List<std::string, uint64_t> >("gg::List<std::string uint64_t>", steps);

        save("test_push_back.png");
    }
#endif

#if 1
    {
        figure()->size(FIG_SIZE_W, FIG_SIZE_H);
        title("test: creation "+mode);
        auto cfgLegend = legend({});
        cfgLegend->box(false);
        cfgLegend->horizontal_location(legend::horizontal_alignment::left);
        grid(true);
        hold(true);
        xlabel("iteration");
        ylabel("time us");

        auto const steps = BuildLogStep(5, 8, 10);
        xrange({static_cast<double>(steps.front()), static_cast<double>(steps.back())});

        test_creation<std::list<std::string> >("std::list<std::string>", steps);

        test_creation<std::vector<std::string> >("std::vector<std::string>", steps);

        test_creation<std::deque<std::string> >("std::deque<std::string>", steps);

        test_creation<gg::List<std::string, uint8_t> >("gg::List<std::string uint8_t>", steps);
        test_creation<gg::List<std::string, uint16_t> >("gg::List<std::string uint16_t>", steps);
        test_creation<gg::List<std::string, uint32_t> >("gg::List<std::string uint32_t>", steps);
        test_creation<gg::List<std::string, uint64_t> >("gg::List<std::string uint64_t>", steps);

        save("test_creation.png");
    }
#endif

#if 1
    {
        figure()->size(FIG_SIZE_W, FIG_SIZE_H);
        title("test: destruction "+mode);
        auto cfgLegend = legend({});
        cfgLegend->box(false);
        cfgLegend->horizontal_location(legend::horizontal_alignment::left);
        grid(true);
        hold(true);
        xlabel("iteration");
        ylabel("time us");

        auto const steps = BuildLogStep(5, 8, 10);
        xrange({static_cast<double>(steps.front()), static_cast<double>(steps.back())});

        test_destruction<std::list<std::string> >("std::list<std::string>", steps);

        test_destruction<std::vector<std::string> >("std::vector<std::string>", steps);

        test_destruction<std::deque<std::string> >("std::deque<std::string>", steps);

        test_destruction<gg::List<std::string, uint8_t> >("gg::List<std::string uint8_t>", steps);
        test_destruction<gg::List<std::string, uint16_t> >("gg::List<std::string uint16_t>", steps);
        test_destruction<gg::List<std::string, uint32_t> >("gg::List<std::string uint32_t>", steps);
        test_destruction<gg::List<std::string, uint64_t> >("gg::List<std::string uint64_t>", steps);

        save("test_destruction.png");
    }
#endif

#if 1
    {
        figure()->size(FIG_SIZE_W, FIG_SIZE_H);
        title("test: pushfront "+mode);
        auto cfgLegend = legend({});
        cfgLegend->box(false);
        cfgLegend->horizontal_location(legend::horizontal_alignment::left);
        grid(true);
        hold(true);
        xlabel("iteration");
        ylabel("time us");

        auto const steps = BuildLogStep(5, 8, 10);
        xrange({static_cast<double>(steps.front()), static_cast<double>(steps.back())});

        test_pushFront<std::list<std::string> >("std::list<std::string>", steps);

        test_pushFront<std::deque<std::string> >("std::deque<std::string>", steps);

        test_pushFront<gg::List<std::string, uint8_t> >("gg::List<std::string uint8_t>", steps);
        test_pushFront<gg::List<std::string, uint16_t> >("gg::List<std::string uint16_t>", steps);
        test_pushFront<gg::List<std::string, uint32_t> >("gg::List<std::string uint32_t>", steps);
        test_pushFront<gg::List<std::string, uint64_t> >("gg::List<std::string uint64_t>", steps);

        save("test_pushFront.png");
    }
#endif

#if 1
    {
        figure()->size(FIG_SIZE_W, FIG_SIZE_H);
        title("test: erase "+mode);
        auto cfgLegend = legend({});
        cfgLegend->box(false);
        cfgLegend->horizontal_location(legend::horizontal_alignment::left);
        grid(true);
        hold(true);
        xlabel("iteration");
        ylabel("time us");

        auto const steps = BuildLogStep(4, 5, 10);
        xrange({static_cast<double>(steps.front()), static_cast<double>(steps.back())});

        test_erase<std::list<std::string> >("std::list<std::string>", steps);

        test_erase<std::vector<std::string> >("std::vector<std::string>", steps);

        test_erase<std::deque<std::string> >("std::deque<std::string>", steps);

        test_erase<gg::List<std::string, uint8_t> >("gg::List<std::string uint8_t>", steps);
        test_erase<gg::List<std::string, uint16_t> >("gg::List<std::string uint16_t>", steps);
        test_erase<gg::List<std::string, uint32_t> >("gg::List<std::string uint32_t>", steps);
        test_erase<gg::List<std::string, uint64_t> >("gg::List<std::string uint64_t>", steps);

        save("test_erase.png");
    }
#endif

#if 1
    {
        figure()->size(FIG_SIZE_W, FIG_SIZE_H);
        title("test: iterations "+mode);
        auto cfgLegend = legend({});
        cfgLegend->box(false);
        cfgLegend->horizontal_location(legend::horizontal_alignment::left);
        grid(true);
        hold(true);
        xlabel("iteration");
        ylabel("time us");

        auto const steps = BuildLogStep(5, 8, 10);
        xrange({static_cast<double>(steps.front()), static_cast<double>(steps.back())});

        test_iterations<std::list<std::string> >("std::list<std::string>", steps);

        test_iterations<std::vector<std::string> >("std::vector<std::string>", steps);

        test_iterations<std::deque<std::string> >("std::deque<std::string>", steps);

        test_iterations<gg::List<std::string, uint8_t> >("gg::List<std::string uint8_t>", steps);
        test_iterations<gg::List<std::string, uint16_t> >("gg::List<std::string uint16_t>", steps);
        test_iterations<gg::List<std::string, uint32_t> >("gg::List<std::string uint32_t>", steps);
        test_iterations<gg::List<std::string, uint64_t> >("gg::List<std::string uint64_t>", steps);

        save("test_iterations.png");
    }
#endif

#if 1
    {
        figure()->size(FIG_SIZE_W, FIG_SIZE_H);
        title("test: insert str "+mode);
        auto cfgLegend = legend({});
        cfgLegend->box(false);
        cfgLegend->horizontal_location(legend::horizontal_alignment::left);
        grid(true);
        hold(true);
        xlabel("iteration");
        ylabel("time us");

        auto const steps = BuildLogStep(6, 8, 20);
        xrange({static_cast<double>(steps.front()), static_cast<double>(steps.back())});

        test_insert<std::list<std::string> >("std::list<std::string>", steps);

        test_insert<gg::List<std::string, uint8_t> >("gg::List<std::string uint8_t>", steps);
        test_insert<gg::List<std::string, uint16_t> >("gg::List<std::string uint16_t>", steps);
        test_insert<gg::List<std::string, uint32_t> >("gg::List<std::string uint32_t>", steps);
        test_insert<gg::List<std::string, uint64_t> >("gg::List<std::string uint64_t>", steps);

        save("test_insert_str.png");
    }

    {
        figure()->size(FIG_SIZE_W, FIG_SIZE_H);
        title("test: insert int "+mode);
        auto cfgLegend = legend({});
        cfgLegend->box(false);
        cfgLegend->horizontal_location(legend::horizontal_alignment::left);
        grid(true);
        hold(true);
        xlabel("iteration");
        ylabel("time us");

        auto const steps = BuildLogStep(6, 8, 20);
        xrange({static_cast<double>(steps.front()), static_cast<double>(steps.back())});

        test_insert<std::list<uint32_t> >("std::list<uint32_t>", steps);

        test_insert<gg::List<uint32_t, uint8_t> >("gg::List<uint32_t uint8_t>", steps);
        test_insert<gg::List<uint32_t, uint16_t> >("gg::List<uint32_t uint16_t>", steps);
        test_insert<gg::List<uint32_t, uint32_t> >("gg::List<uint32_t uint32_t>", steps);
        test_insert<gg::List<uint32_t, uint64_t> >("gg::List<uint32_t uint64_t>", steps);

        save("test_insert_int.png");
    }
#endif

    return 0;
}
