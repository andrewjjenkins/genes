#include <benchmark/benchmark.h>
#include <common/genome.h>

static void BM_Substring(benchmark::State &state) {
  Genome g = loadGenomeFromPlainFile("./data/Vibrio_cholerae.txt");
  for (auto _ : state) {
    auto x = g.stringFrequency(9);
  }
}

BENCHMARK(BM_Substring);
