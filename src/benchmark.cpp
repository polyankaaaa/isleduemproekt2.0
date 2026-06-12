#include "benchmark.hpp"

#include "data_generator.hpp"
#include "fenwick_tree.hpp"
#include "prefix_sum_1d.hpp"
#include "prefix_sum_2d.hpp"
#include "precomputed_rmq.hpp"
#include "query_generator.hpp"
#include "results_writer.hpp"
#include "segment_tree.hpp"
#include "sparse_table.hpp"
#include "sqrt_decomposition.hpp"

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

namespace semga67 {
namespace {

constexpr unsigned kSeed = 42;
constexpr std::size_t kQueryCount = 10'000;
constexpr std::size_t kUpdateCount = 1'000;

const std::vector<std::size_t> kSizes1D{100,   200,   500,   1000,  2000,
                                        5000,  10000, 20000, 50000};
const std::vector<std::size_t> kSizesPrecomputedRMQ{50, 100, 200, 500, 1000, 2000};
const std::vector<std::size_t> kSizes2D{10, 20, 50, 100, 200, 500};

BenchmarkResult make_result_1d(std::string structure_name, std::size_t n, std::uint64_t build_ops,
                               double avg_query_ops, double avg_update_ops = 0.0) {
    return BenchmarkResult{std::move(structure_name), n, 0, build_ops, avg_query_ops,
                           avg_update_ops};
}

double average_query_ops(const std::vector<RangeQuery>& queries,
                         const auto& query_callable) {
    OperationCounter counter;
    for (const RangeQuery& query : queries) {
        (void)query_callable(query, counter);
    }
    return static_cast<double>(counter.get()) / static_cast<double>(queries.size());
}

double average_update_ops(const std::vector<PointUpdate>& updates,
                          const auto& update_callable) {
    OperationCounter counter;
    for (const PointUpdate& update : updates) {
        update_callable(update, counter);
    }
    return static_cast<double>(counter.get()) / static_cast<double>(updates.size());
}

void benchmark_prefix_sum_1d(const std::vector<std::int64_t>& data,
                             const std::vector<RangeQuery>& queries,
                             std::vector<BenchmarkResult>& results) {
    OperationCounter counter;
    PrefixSum1D structure(data, counter);
    const std::uint64_t build_ops = counter.get();

    const double avg_query = average_query_ops(queries, [&](const RangeQuery& query,
                                                            OperationCounter& query_counter) {
        structure.query(query.left, query.right, query_counter);
    });

    results.push_back(make_result_1d("prefix_sum_1d", data.size(), build_ops, avg_query));
}

void benchmark_precomputed_rmq(const std::vector<std::int64_t>& data,
                               const std::vector<RangeQuery>& queries,
                               std::vector<BenchmarkResult>& results) {
    OperationCounter counter;
    PrecomputedRMQ structure(data, counter);
    const std::uint64_t build_ops = counter.get();

    const double avg_query = average_query_ops(queries, [&](const RangeQuery& query,
                                                            OperationCounter& query_counter) {
        structure.query(query.left, query.right, query_counter);
    });

    results.push_back(make_result_1d("precomputed_rmq", data.size(), build_ops, avg_query));
}

void benchmark_sqrt_rsq(const std::vector<std::int64_t>& data,
                        const std::vector<RangeQuery>& queries,
                        std::vector<BenchmarkResult>& results) {
    OperationCounter counter;
    auto structure = make_sqrt_rsq(data, counter);
    const std::uint64_t build_ops = counter.get();

    const double avg_query = average_query_ops(queries, [&](const RangeQuery& query,
                                                            OperationCounter& query_counter) {
        structure.query(query.left, query.right, query_counter);
    });

    results.push_back(make_result_1d("sqrt_rsq", data.size(), build_ops, avg_query));
}

void benchmark_sqrt_rmq(const std::vector<std::int64_t>& data,
                        const std::vector<RangeQuery>& queries,
                        std::vector<BenchmarkResult>& results) {
    OperationCounter counter;
    auto structure = make_sqrt_rmq(data, counter);
    const std::uint64_t build_ops = counter.get();

    const double avg_query = average_query_ops(queries, [&](const RangeQuery& query,
                                                            OperationCounter& query_counter) {
        structure.query(query.left, query.right, query_counter);
    });

    results.push_back(make_result_1d("sqrt_rmq", data.size(), build_ops, avg_query));
}

void benchmark_segment_tree_rsq(const std::vector<std::int64_t>& data,
                              const std::vector<RangeQuery>& queries,
                              const std::vector<PointUpdate>& updates,
                              std::vector<BenchmarkResult>& results) {
    OperationCounter counter;
    SegmentTreeRsq structure(data, counter);
    const std::uint64_t build_ops = counter.get();

    const double avg_query = average_query_ops(queries, [&](const RangeQuery& query,
                                                            OperationCounter& query_counter) {
        structure.query(query.left, query.right, query_counter);
    });

    SegmentTreeRsq update_structure(data, counter);
    const double avg_update = average_update_ops(updates, [&](const PointUpdate& update,
                                                            OperationCounter& update_counter) {
        update_structure.update(update.index, update.value, update_counter);
    });

    results.push_back(
        make_result_1d("segment_tree_rsq", data.size(), build_ops, avg_query, avg_update));
}

void benchmark_fenwick_tree_rsq(const std::vector<std::int64_t>& data,
                                const std::vector<RangeQuery>& queries,
                                const std::vector<PointUpdate>& updates,
                                std::vector<BenchmarkResult>& results) {
    OperationCounter counter;
    FenwickTreeRsq structure(data, counter);
    const std::uint64_t build_ops = counter.get();

    const double avg_query = average_query_ops(queries, [&](const RangeQuery& query,
                                                            OperationCounter& query_counter) {
        structure.query(query.left, query.right, query_counter);
    });

    FenwickTreeRsq update_structure(data, counter);
    const double avg_update = average_update_ops(updates, [&](const PointUpdate& update,
                                                              OperationCounter& update_counter) {
        update_structure.update(update.index, update.value, update_counter);
    });

    results.push_back(
        make_result_1d("fenwick_tree_rsq", data.size(), build_ops, avg_query, avg_update));
}

void benchmark_sparse_table_rmq(const std::vector<std::int64_t>& data,
                                const std::vector<RangeQuery>& queries,
                                std::vector<BenchmarkResult>& results) {
    OperationCounter counter;
    SparseTableRmq structure(data, counter);
    const std::uint64_t build_ops = counter.get();

    const double avg_query = average_query_ops(queries, [&](const RangeQuery& query,
                                                            OperationCounter& query_counter) {
        structure.query(query.left, query.right, query_counter);
    });

    results.push_back(make_result_1d("sparse_table_rmq", data.size(), build_ops, avg_query));
}

void benchmark_prefix_sum_2d(const std::vector<std::vector<std::int64_t>>& matrix,
                             const std::vector<RectQuery>& queries,
                             std::vector<BenchmarkResult>& results) {
    OperationCounter counter;
    PrefixSum2D structure(matrix, counter);
    const std::uint64_t build_ops = counter.get();
    const std::size_t rows = matrix.size();
    const std::size_t cols = rows == 0 ? 0 : matrix[0].size();

    OperationCounter query_counter;
    for (const RectQuery& query : queries) {
        (void)structure.query(query, query_counter);
    }
    const double avg_query =
        static_cast<double>(query_counter.get()) / static_cast<double>(queries.size());

    BenchmarkResult result;
    result.structure_name = "prefix_sum_2d";
    result.n = rows;
    result.m = cols;
    result.build_ops = build_ops;
    result.avg_query_ops = avg_query;
    result.avg_update_ops = 0.0;
    results.push_back(std::move(result));
}

void run_benchmark_1d(DataGenerator& data_generator, QueryGenerator& query_generator,
                      ResultsWriter& writer) {
    std::vector<BenchmarkResult> results;
    results.reserve(kSizes1D.size() * 7 + kSizesPrecomputedRMQ.size());

    for (std::size_t n : kSizes1D) {
        data_generator.reseed(kSeed + static_cast<unsigned>(n));
        query_generator.reseed(kSeed + static_cast<unsigned>(n) + 1);

        const std::vector<std::int64_t> data = data_generator.random_array(n);
        const std::vector<RangeQuery> queries = query_generator.random_range_queries(kQueryCount, n);
        const std::vector<PointUpdate> updates =
            query_generator.random_point_updates(kUpdateCount, n);

        benchmark_prefix_sum_1d(data, queries, results);
        benchmark_sqrt_rsq(data, queries, results);
        benchmark_sqrt_rmq(data, queries, results);
        benchmark_segment_tree_rsq(data, queries, updates, results);
        benchmark_fenwick_tree_rsq(data, queries, updates, results);
        benchmark_sparse_table_rmq(data, queries, results);

        std::cout << "benchmark 1D: n=" << n << " готово\n";
    }

    for (std::size_t n : kSizesPrecomputedRMQ) {
        data_generator.reseed(kSeed + static_cast<unsigned>(n));
        query_generator.reseed(kSeed + static_cast<unsigned>(n) + 1);

        const std::vector<std::int64_t> data = data_generator.random_array(n);
        const std::vector<RangeQuery> queries = query_generator.random_range_queries(kQueryCount, n);

        benchmark_precomputed_rmq(data, queries, results);
        std::cout << "benchmark 1D (precomputed_rmq): n=" << n << " готово\n";
    }

    writer.write_1d_csv("benchmark_1d.csv", results);
    std::cout << "Записан файл: " << (writer.output_directory() / "benchmark_1d.csv") << '\n';
}

void run_benchmark_2d(DataGenerator& data_generator, QueryGenerator& query_generator,
                      ResultsWriter& writer) {
    std::vector<BenchmarkResult> results;
    results.reserve(kSizes2D.size());

    for (std::size_t n : kSizes2D) {
        data_generator.reseed(kSeed + static_cast<unsigned>(n) + 10'000);
        query_generator.reseed(kSeed + static_cast<unsigned>(n) + 10'001);

        const std::vector<std::vector<std::int64_t>> matrix =
            data_generator.random_matrix(n, n);
        const std::vector<RectQuery> queries =
            query_generator.random_rect_queries(kQueryCount, n, n);

        benchmark_prefix_sum_2d(matrix, queries, results);
        std::cout << "benchmark 2D: n=m=" << n << " готово\n";
    }

    writer.write_2d_csv("benchmark_2d.csv", results);
    std::cout << "Записан файл: " << (writer.output_directory() / "benchmark_2d.csv") << '\n';
}

}  

bool run_benchmark() {
    DataGenerator data_generator(kSeed);
    QueryGenerator query_generator(kSeed + 1);
    ResultsWriter writer("results");

    std::cout << "Запуск benchmark (операции, не время). Запросов: " << kQueryCount
              << ", обновлений: " << kUpdateCount << '\n';

    run_benchmark_1d(data_generator, query_generator, writer);
    run_benchmark_2d(data_generator, query_generator, writer);

    std::cout << "benchmark: OK\n";
    return true;
}

}  
