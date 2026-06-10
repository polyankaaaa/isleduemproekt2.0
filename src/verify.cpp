#include "verify.hpp"

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

    std::cout << "verify: OK (prefix_sum_1d, prefix_sum_2d, precomputed_rmq)\n";
    return true;
}

} 
