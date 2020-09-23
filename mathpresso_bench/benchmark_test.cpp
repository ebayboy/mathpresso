#include <iostream>
#include <string>
#include <cstring>

#include <benchmark/benchmark.h>

#include "mathpresso_bench.hpp"

/**
 * 第一列是程序名称，第二列是平均迭代一次的时钟时间，
 * 第三列是平均迭代一次的CPU时间，第四列是循环的迭代次数。
 * 时钟时间 = 阻塞时间 + 就绪时间 + 运行时间（也就是CPU时间）
**/
static void BM_compute(benchmark::State& state)
{
	init();

    for (auto _ : state)
        compute();
}

// Register the function as a benchmark
// Iterations:指定迭代此时
// Arg: 指定参数
BENCHMARK(BM_compute);

BENCHMARK_MAIN(); //程序入口

