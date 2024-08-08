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

#include <benchmark/benchmark.h>
/*
template<class T>
static void BM_EmptyCreation(benchmark::State& state)
{
    for (auto _ : state)
    {
        T emptyList;
    }
}
BENCHMARK_TEMPLATE(BM_EmptyCreation, std::vector<std::string>);
BENCHMARK_TEMPLATE(BM_EmptyCreation, std::deque<std::string>);
BENCHMARK_TEMPLATE(BM_EmptyCreation, std::list<std::string>);
BENCHMARK_TEMPLATE(BM_EmptyCreation, gg::List<std::string, uint8_t>);
BENCHMARK_TEMPLATE(BM_EmptyCreation, gg::List<std::string, uint16_t>);
BENCHMARK_TEMPLATE(BM_EmptyCreation, gg::List<std::string, uint32_t>);
BENCHMARK_TEMPLATE(BM_EmptyCreation, gg::List<std::string, uint64_t>);

template<class T>
static void BM_FillBack(benchmark::State& state)
{
    T list;
    for (auto _ : state)
    {
        for (decltype(state.range()) i = 0; i < state.range(); ++i)
        {
            list.push_back(std::to_string(i));
        }
    }
    state.SetComplexityN(state.range());
}
BENCHMARK_TEMPLATE(BM_FillBack, std::vector<std::string>)->RangeMultiplier(2)->Range(1<<0, 1<<5)->Complexity();
BENCHMARK_TEMPLATE(BM_FillBack, std::deque<std::string>)->RangeMultiplier(2)->Range(1<<0, 1<<5)->Complexity();
BENCHMARK_TEMPLATE(BM_FillBack, std::list<std::string>)->RangeMultiplier(2)->Range(1<<0, 1<<5)->Complexity();
BENCHMARK_TEMPLATE(BM_FillBack, gg::List<std::string, uint8_t>)->RangeMultiplier(2)->Range(1<<0, 1<<5)->Complexity();
BENCHMARK_TEMPLATE(BM_FillBack, gg::List<std::string, uint16_t>)->RangeMultiplier(2)->Range(1<<0, 1<<5)->Complexity();
BENCHMARK_TEMPLATE(BM_FillBack, gg::List<std::string, uint32_t>)->RangeMultiplier(2)->Range(1<<0, 1<<5)->Complexity();
BENCHMARK_TEMPLATE(BM_FillBack, gg::List<std::string, uint64_t>)->RangeMultiplier(2)->Range(1<<0, 1<<5)->Complexity();

template<class T>
static void BM_FillFront(benchmark::State& state)
{
    T list;
    for (auto _ : state)
    {
        for (decltype(state.range()) i = 0; i < state.range(); ++i)
        {
            list.push_front(std::to_string(i));
        }
    }
    state.SetComplexityN(state.range());
}
BENCHMARK_TEMPLATE(BM_FillFront, std::deque<std::string>)->RangeMultiplier(2)->Range(1<<0, 1<<5)->Complexity();
BENCHMARK_TEMPLATE(BM_FillFront, std::list<std::string>)->RangeMultiplier(2)->Range(1<<0, 1<<5)->Complexity();
BENCHMARK_TEMPLATE(BM_FillFront, gg::List<std::string, uint8_t>)->RangeMultiplier(2)->Range(1<<0, 1<<5)->Complexity();
BENCHMARK_TEMPLATE(BM_FillFront, gg::List<std::string, uint16_t>)->RangeMultiplier(2)->Range(1<<0, 1<<5)->Complexity();
BENCHMARK_TEMPLATE(BM_FillFront, gg::List<std::string, uint32_t>)->RangeMultiplier(2)->Range(1<<0, 1<<5)->Complexity();
BENCHMARK_TEMPLATE(BM_FillFront, gg::List<std::string, uint64_t>)->RangeMultiplier(2)->Range(1<<0, 1<<5)->Complexity();

template<class T>
static void BM_InsertMiddle(benchmark::State& state)
{
    T list(state.range());
    auto it = std::next(list.begin(), list.size() / 2);

    for (auto _ : state)
    {
        for (decltype(state.range()) i = 0; i < state.range(); ++i)
        {
            it = list.insert(it, std::to_string(i));
        }
    }
    state.SetComplexityN(state.range());
}
BENCHMARK_TEMPLATE(BM_InsertMiddle, std::vector<std::string>)->RangeMultiplier(2)->Range(1<<0, 1<<5)->Complexity();
BENCHMARK_TEMPLATE(BM_InsertMiddle, std::deque<std::string>)->RangeMultiplier(2)->Range(1<<0, 1<<5)->Complexity();
BENCHMARK_TEMPLATE(BM_InsertMiddle, std::list<std::string>)->RangeMultiplier(2)->Range(1<<0, 1<<5)->Complexity();
BENCHMARK_TEMPLATE(BM_InsertMiddle, gg::List<std::string, uint8_t>)->RangeMultiplier(2)->Range(1<<0, 1<<5)->Complexity();
BENCHMARK_TEMPLATE(BM_InsertMiddle, gg::List<std::string, uint16_t>)->RangeMultiplier(2)->Range(1<<0, 1<<5)->Complexity();
BENCHMARK_TEMPLATE(BM_InsertMiddle, gg::List<std::string, uint32_t>)->RangeMultiplier(2)->Range(1<<0, 1<<5)->Complexity();
BENCHMARK_TEMPLATE(BM_InsertMiddle, gg::List<std::string, uint64_t>)->RangeMultiplier(2)->Range(1<<0, 1<<5)->Complexity();

template<class T>
static void BM_EraseMiddle(benchmark::State& state)
{
    T list(state.range());
    auto it = std::next(list.begin(), list.size() / 2);

    for (auto _ : state)
    {
        for (decltype(state.range()) i = 0; i < state.range(); ++i)
        {
            it = list.erase(it);
        }
    }
    state.SetComplexityN(state.range());
}
BENCHMARK_TEMPLATE(BM_EraseMiddle, std::vector<std::string>)->RangeMultiplier(2)->Range(1<<0, 1<<5)->Complexity();
BENCHMARK_TEMPLATE(BM_EraseMiddle, std::deque<std::string>)->RangeMultiplier(2)->Range(1<<0, 1<<5)->Complexity();
BENCHMARK_TEMPLATE(BM_EraseMiddle, std::list<std::string>)->RangeMultiplier(2)->Range(1<<0, 1<<5)->Complexity();
BENCHMARK_TEMPLATE(BM_EraseMiddle, gg::List<std::string, uint8_t>)->RangeMultiplier(2)->Range(1<<0, 1<<5)->Complexity();
BENCHMARK_TEMPLATE(BM_EraseMiddle, gg::List<std::string, uint16_t>)->RangeMultiplier(2)->Range(1<<0, 1<<5)->Complexity();
BENCHMARK_TEMPLATE(BM_EraseMiddle, gg::List<std::string, uint32_t>)->RangeMultiplier(2)->Range(1<<0, 1<<5)->Complexity();
BENCHMARK_TEMPLATE(BM_EraseMiddle, gg::List<std::string, uint64_t>)->RangeMultiplier(2)->Range(1<<0, 1<<5)->Complexity();*/

template<class T>
static void BM_Iterate(benchmark::State& state)
{
    T list(state.range());
    auto it = list.begin();

    for (auto _ : state)
    {
        for (decltype(state.range()) i = 0; i < state.range(); ++i)
        {
            benchmark::DoNotOptimize(*it);
            ++it;
        }
    }
    state.SetComplexityN(state.range());
}
BENCHMARK_TEMPLATE(BM_Iterate, std::vector<std::string>)->RangeMultiplier(2)->Range(1<<0, 1<<5)->Complexity();
BENCHMARK_TEMPLATE(BM_Iterate, std::deque<std::string>)->RangeMultiplier(2)->Range(1<<0, 1<<5)->Complexity();
BENCHMARK_TEMPLATE(BM_Iterate, std::list<std::string>)->RangeMultiplier(2)->Range(1<<0, 1<<5)->Complexity();
BENCHMARK_TEMPLATE(BM_Iterate, gg::List<std::string, uint8_t>)->RangeMultiplier(2)->Range(1<<0, 1<<5)->Complexity();
BENCHMARK_TEMPLATE(BM_Iterate, gg::List<std::string, uint16_t>)->RangeMultiplier(2)->Range(1<<0, 1<<5)->Complexity();
BENCHMARK_TEMPLATE(BM_Iterate, gg::List<std::string, uint32_t>)->RangeMultiplier(2)->Range(1<<0, 1<<5)->Complexity();
BENCHMARK_TEMPLATE(BM_Iterate, gg::List<std::string, uint64_t>)->RangeMultiplier(2)->Range(1<<0, 1<<5)->Complexity();

BENCHMARK_MAIN();
