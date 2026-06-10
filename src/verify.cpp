#include "verify.hpp"
#include "sqrt_decomposition.hpp"
#include "segment_tree.hpp"
#include "prefix_sum_1d.hpp"
#include "prefix_sum_2d.hpp"
#include "precomputed_rmq.hpp"
#include "types.hpp"

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <random>
#include <vector>

namespace semga67 {
namespace {

std::int64_t brute_sum_1d(const std::vector<std::int64_t>& data,
                          std::size_t left,
                          std::size_t right) {
    std::int64_t sum = 0;
    for (std::size_t index = left; index <= right; ++index) {
        sum += data[index];
    }
    return sum;
}

std::int64_t brute_sum_2d(const std::vector<std::vector<std::int64_t>>& matrix,
                          const RectQuery& rect) {
    std::int64_t sum = 0;
    for (std::size_t row = rect.row_top; row <= rect.row_bottom; ++row) {
        for (std::size_t col = rect.col_left; col <= rect.col_right; ++col) {
            sum += matrix[row][col];
        }
    }
    return sum;
}

std::int64_t brute_min_1d(const std::vector<std::int64_t>& data,
                          std::size_t left,
                          std::size_t right) {
    std::int64_t minimum = data[left];
    for (std::size_t index = left + 1; index <= right; ++index) {
        minimum = std::min(minimum, data[index]);
    }
    return minimum;
}

bool verify_prefix_sum_1d() {
    const std::vector<std::int64_t> data{3, 1, 4, 1, 5, 9, 2, 6};
    OperationCounter counter;

    PrefixSum1D structure(data, counter);

    if (counter.get() != data.size()) {
        std::cerr << "verify prefix_sum_1d: неверное число операций построения, ожидалось "
                  << data.size() << ", получено " << counter.get() << '\n';
        return false;
    }

    const std::vector<RangeQuery> queries{{0, 7}, {2, 5}, {4, 4}, {1, 3}};

    for (const RangeQuery& query : queries) {
        counter.reset();

        const std::int64_t got = structure.query(query.left, query.right, counter);
        const std::int64_t expected = brute_sum_1d(data, query.left, query.right);

        if (got != expected) {
            std::cerr << "verify prefix_sum_1d: неверный ответ на ["
                      << query.left << ", " << query.right << "]\n";
            return false;
        }

        if (counter.get() != 1) {
            std::cerr << "verify prefix_sum_1d: ожидалась 1 операция на запрос\n";
            return false;
        }
    }

    return true;
}

bool verify_prefix_sum_2d() {
    const std::vector<std::vector<std::int64_t>> matrix{
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9},
    };

    OperationCounter counter;
    PrefixSum2D structure(matrix, counter);

    const std::size_t expected_build_ops = matrix.size() * matrix[0].size() * 3;

    if (counter.get() != expected_build_ops) {
        std::cerr << "verify prefix_sum_2d: неверное число операций построения\n";
        return false;
    }

    const std::vector<RectQuery> queries{
        {0, 0, 2, 2},
        {1, 1, 2, 2},
        {0, 2, 2, 2},
        {2, 0, 2, 1},
    };

    for (const RectQuery& query : queries) {
        counter.reset();

        const std::int64_t got = structure.query(query, counter);
        const std::int64_t expected = brute_sum_2d(matrix, query);

        if (got != expected) {
            std::cerr << "verify prefix_sum_2d: неверный ответ на прямоугольнике\n";
            return false;
        }

        if (counter.get() != 4) {
            std::cerr << "verify prefix_sum_2d: ожидалось 4 операции на запрос\n";
            return false;
        }
    }

    return true;
}

bool verify_precomputed_rmq() {
    const std::vector<std::int64_t> data{9, 3, 7, 1, 8, 2, 6, 5};

    OperationCounter counter;
    PrecomputedRMQ structure(data, counter);

    const std::size_t n = data.size();
    const std::size_t expected_comparisons = n * (n - 1) / 2;

    if (counter.get() != expected_comparisons) {
        std::cerr << "verify precomputed_rmq: неверное число сравнений при построении\n";
        return false;
    }

    const std::vector<RangeQuery> queries{{0, 7}, {1, 4}, {3, 3}, {2, 6}};
    for (const RangeQuery& query : queries) {
        counter.reset();

        const std::int64_t got = structure.query(query.left, query.right, counter);
        const std::int64_t expected = brute_min_1d(data, query.left, query.right);

        if (got != expected) {
            std::cerr << "verify precomputed_rmq: неверный минимум на ["
                      << query.left << ", " << query.right << "]\n";
            return false;
        }

        if (counter.get() != 1) {
            std::cerr << "verify precomputed_rmq: ожидалась 1 операция на запрос\n";
            return false;
        }
    }

    return true;
}

bool verify_random_cases() {
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> value_dist(-20, 20);

    for (int trial = 0; trial < 50; ++trial) {
        const std::size_t n = 5 + static_cast<std::size_t>(trial % 10);

        std::vector<std::int64_t> data(n);
        for (std::int64_t& value : data) {
            value = value_dist(rng);
        }

        OperationCounter counter;
        PrefixSum1D structure(data, counter);

        std::uniform_int_distribution<std::size_t> index_dist(0, n - 1);

        for (int query_trial = 0; query_trial < 30; ++query_trial) {
            std::size_t left = index_dist(rng);
            std::size_t right = index_dist(rng);

            if (left > right) {
                std::swap(left, right);
            }

            counter.reset();

            const std::int64_t got = structure.query(left, right, counter);
            const std::int64_t expected = brute_sum_1d(data, left, right);

            if (got != expected) {
                std::cerr << "verify random prefix_sum_1d: ошибка\n";
                return false;
            }
        }
    }

    for (int trial = 0; trial < 20; ++trial) {
        const std::size_t rows = 3 + static_cast<std::size_t>(trial % 4);
        const std::size_t cols = 3 + static_cast<std::size_t>((trial + 1) % 4);

        std::vector<std::vector<std::int64_t>> matrix(
            rows, std::vector<std::int64_t>(cols));

        for (auto& row : matrix) {
            for (std::int64_t& value : row) {
                value = value_dist(rng);
            }
        }

        OperationCounter counter;
        PrefixSum2D structure(matrix, counter);

        std::uniform_int_distribution<std::size_t> row_dist(0, rows - 1);
        std::uniform_int_distribution<std::size_t> col_dist(0, cols - 1);

        for (int query_trial = 0; query_trial < 20; ++query_trial) {
            RectQuery query;
            query.row_top = row_dist(rng);
            query.row_bottom = row_dist(rng);
            query.col_left = col_dist(rng);
            query.col_right = col_dist(rng);

            if (query.row_top > query.row_bottom) {
                std::swap(query.row_top, query.row_bottom);
            }

            if (query.col_left > query.col_right) {
                std::swap(query.col_left, query.col_right);
            }

            counter.reset();

            const std::int64_t got = structure.query(query, counter);
            const std::int64_t expected = brute_sum_2d(matrix, query);

            if (got != expected) {
                std::cerr << "verify random prefix_sum_2d: ошибка\n";
                return false;
            }
        }
    }

    for (int trial = 0; trial < 30; ++trial) {
        const std::size_t n = 3 + static_cast<std::size_t>(trial % 8);

        std::vector<std::int64_t> data(n);
        for (std::int64_t& value : data) {
            value = value_dist(rng);
        }

        OperationCounter counter;
        PrecomputedRMQ structure(data, counter);

        std::uniform_int_distribution<std::size_t> index_dist(0, n - 1);

        for (int query_trial = 0; query_trial < 25; ++query_trial) {
            std::size_t left = index_dist(rng);
            std::size_t right = index_dist(rng);

            if (left > right) {
                std::swap(left, right);
            }

            counter.reset();
            const std::int64_t got = structure.query(left, right, counter);
            const std::int64_t expected = brute_min_1d(data, left, right);

            if (got != expected) {
                std::cerr << "verify random precomputed_rmq: ошибка\n";
                return false;
            }
        }
    }

    return true;
}

} 

bool verify_sqrt_rsq() {
    const std::vector<std::int64_t> data{3, 1, 4, 1, 5, 9, 2, 6};
    OperationCounter counter;
    auto structure = make_sqrt_rsq(data, counter);

    if (counter.get() != data.size()) {
        std::cerr << "verify sqrt_rsq: неверное число операций построения\n";
        return false;
    }

    const std::vector<RangeQuery> queries{{0, 7}, {2, 5}, {4, 4}, {1, 3}};

    for (const RangeQuery& query : queries) {
        counter.reset();

        const std::int64_t got = structure.query(query.left, query.right, counter);
        const std::int64_t expected = brute_sum_1d(data, query.left, query.right);

        if (got != expected) {
            std::cerr << "verify sqrt_rsq: неверный ответ\n";
            return false;
        }

        if (counter.get() == 0) {
            std::cerr << "verify sqrt_rsq: ожидалась хотя бы одна операция combine\n";
            return false;
        }
    }

    return true;
}

bool verify_sqrt_rmq() {
    const std::vector<std::int64_t> data{9, 3, 7, 1, 8, 2, 6, 5};
    OperationCounter counter;
    auto structure = make_sqrt_rmq(data, counter);

    if (counter.get() != data.size()) {
        std::cerr << "verify sqrt_rmq: неверное число операций построения\n";
        return false;
    }

    const std::vector<RangeQuery> queries{{0, 7}, {1, 4}, {3, 3}, {2, 6}};

    for (const RangeQuery& query : queries) {
        counter.reset();

        const std::int64_t got = structure.query(query.left, query.right, counter);
        const std::int64_t expected = brute_min_1d(data, query.left, query.right);

        if (got != expected) {
            std::cerr << "verify sqrt_rmq: неверный минимум\n";
            return false;
        }

        if (counter.get() == 0) {
            std::cerr << "verify sqrt_rmq: ожидалась хотя бы одна операция combine\n";
            return false;
        }
    }

    return true;
}

bool verify_sqrt_random_cases() {
    std::mt19937 rng(123);
    std::uniform_int_distribution<int> value_dist(-30, 30);

    for (int trial = 0; trial < 40; ++trial) {
        const std::size_t n = 4 + static_cast<std::size_t>(trial % 12);

        std::vector<std::int64_t> data(n);
        for (std::int64_t& value : data) {
            value = value_dist(rng);
        }

        OperationCounter counter;
        auto rsq = make_sqrt_rsq(data, counter);
        auto rmq = make_sqrt_rmq(data, counter);

        std::uniform_int_distribution<std::size_t> index_dist(0, n - 1);

        for (int query_trial = 0; query_trial < 30; ++query_trial) {
            std::size_t left = index_dist(rng);
            std::size_t right = index_dist(rng);

            if (left > right) {
                std::swap(left, right);
            }

            counter.reset();

            const std::int64_t sum_got = rsq.query(left, right, counter);
            const std::int64_t sum_expected = brute_sum_1d(data, left, right);

            if (sum_got != sum_expected) {
                std::cerr << "verify random sqrt_rsq: ошибка\n";
                return false;
            }

            counter.reset();

            const std::int64_t min_got = rmq.query(left, right, counter);
            const std::int64_t min_expected = brute_min_1d(data, left, right);

            if (min_got != min_expected) {
                std::cerr << "verify random sqrt_rmq: ошибка\n";
                return false;
            }
        }
    }

    return true;
}

bool verify_segment_tree_rsq() {
    std::vector<std::int64_t> data{3, 1, 4, 1, 5, 9, 2, 6};
    OperationCounter counter;

    SegmentTreeRsq structure(data, counter);

    const std::size_t n = data.size();

    if (n > 1 && counter.get() != n - 1) {
        std::cerr << "verify segment_tree_rsq: ожидалось " << (n - 1)
                  << " операций построения, получено " << counter.get() << '\n';
        return false;
    }

    const std::vector<RangeQuery> queries{{0, 7}, {2, 5}, {4, 4}, {1, 3}};

    for (const RangeQuery& query : queries) {
        counter.reset();
        const std::int64_t got = structure.query(query.left, query.right, counter);
        const std::int64_t expected = brute_sum_1d(data, query.left, query.right);

        if (got != expected) {
            std::cerr << "verify segment_tree_rsq: неверный ответ на запросе\n";
            return false;
        }
    }

    const std::vector<PointUpdate> updates{{2, 10}, {0, -5}, {7, 0}};

    for (const PointUpdate& update : updates) {
        data[update.index] = update.value;

        counter.reset();
        structure.update(update.index, update.value, counter);

        if (counter.get() == 0) {
            std::cerr << "verify segment_tree_rsq: ожидалась хотя бы одна операция update\n";
            return false;
        }
    }

    for (const RangeQuery& query : queries) {
        counter.reset();

        const std::int64_t got = structure.query(query.left, query.right, counter);
        const std::int64_t expected = brute_sum_1d(data, query.left, query.right);

        if (got != expected) {
            std::cerr << "verify segment_tree_rsq: неверный ответ после обновлений\n";
            return false;
        }
    }

    return true;
}

bool verify_segment_tree_random_cases() {
    std::mt19937 rng(321);
    std::uniform_int_distribution<int> value_dist(-40, 40);

    for (int trial = 0; trial < 35; ++trial) {
        const std::size_t n = 3 + static_cast<std::size_t>(trial % 10);

        std::vector<std::int64_t> data(n);
        for (std::int64_t& value : data) {
            value = value_dist(rng);
        }

        OperationCounter counter;
        SegmentTreeRsq structure(data, counter);

        std::uniform_int_distribution<std::size_t> index_dist(0, n - 1);

        for (int step = 0; step < 50; ++step) {
            if (step % 2 == 0) {
                std::size_t left = index_dist(rng);
                std::size_t right = index_dist(rng);

                if (left > right) {
                    std::swap(left, right);
                }

                counter.reset();

                const std::int64_t got = structure.query(left, right, counter);
                const std::int64_t expected = brute_sum_1d(data, left, right);

                if (got != expected) {
                    std::cerr << "verify random segment_tree_rsq: ошибка запроса\n";
                    return false;
                }
            } else {
                const std::size_t index = index_dist(rng);
                const std::int64_t value = value_dist(rng);

                data[index] = value;

                counter.reset();
                structure.update(index, value, counter);

                if (counter.get() == 0) {
                    std::cerr << "verify random segment_tree_rsq: ошибка update\n";
                    return false;
                }
            }
        }
    }

    return true;
}


bool run_verify() {
    if (!verify_prefix_sum_1d()) {
        return false;
    }

    if (!verify_prefix_sum_2d()) {
        return false;
    }

    if (!verify_precomputed_rmq()) {
        return false;
    }

    if (!verify_random_cases()) {
        return false;
    }
    if (!verify_sqrt_rsq()) {
        return false;
    }

    if (!verify_sqrt_rmq()) {
        return false;
    }

    if (!verify_sqrt_random_cases()) {
        return false;
    }

    if (!verify_segment_tree_rsq()) {
        return false;
    }

    if (!verify_segment_tree_random_cases()) {
        return false;
    }

    std::cout << "verify: OK (prefix_sum_1d, prefix_sum_2d, precomputed_rmq, "
                 "sqrt_rsq, sqrt_rmq, segment_tree_rsq)\n";
    return true;
}

} 
