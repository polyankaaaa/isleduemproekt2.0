#pragma once

#include "types.hpp"

#include <cstddef>
#include <cstdint>
#include <random>
#include <vector>

namespace semga67 {

class QueryGenerator {
public:
    explicit QueryGenerator(unsigned seed = 42) : rng_(seed) {}

    [[nodiscard]] std::vector<RangeQuery> random_range_queries(std::size_t count, std::size_t n) {
        std::vector<RangeQuery> queries;
        queries.reserve(count);
        if (n == 0) {
            return queries;
        }

        std::uniform_int_distribution<std::size_t> index_dist(0, n - 1);
        for (std::size_t index = 0; index < count; ++index) {
            RangeQuery query;
            query.left = index_dist(rng_);
            query.right = index_dist(rng_);
            if (query.left > query.right) {
                std::swap(query.left, query.right);
            }
            queries.push_back(query);
        }
        return queries;
    }

    [[nodiscard]] std::vector<RectQuery> random_rect_queries(std::size_t count, std::size_t rows,
                                                             std::size_t cols) {
        std::vector<RectQuery> queries;
        queries.reserve(count);
        if (rows == 0 || cols == 0) {
            return queries;
        }

        std::uniform_int_distribution<std::size_t> row_dist(0, rows - 1);
        std::uniform_int_distribution<std::size_t> col_dist(0, cols - 1);
        for (std::size_t index = 0; index < count; ++index) {
            RectQuery query;
            query.row_top = row_dist(rng_);
            query.row_bottom = row_dist(rng_);
            query.col_left = col_dist(rng_);
            query.col_right = col_dist(rng_);
            if (query.row_top > query.row_bottom) {
                std::swap(query.row_top, query.row_bottom);
            }
            if (query.col_left > query.col_right) {
                std::swap(query.col_left, query.col_right);
            }
            queries.push_back(query);
        }
        return queries;
    }

    [[nodiscard]] std::vector<PointUpdate> random_point_updates(std::size_t count, std::size_t n,
                                                              std::int64_t min_value = -1'000'000,
                                                              std::int64_t max_value = 1'000'000) {
        std::vector<PointUpdate> updates;
        updates.reserve(count);
        if (n == 0) {
            return updates;
        }

        std::uniform_int_distribution<std::size_t> index_dist(0, n - 1);
        std::uniform_int_distribution<std::int64_t> value_dist(min_value, max_value);
        for (std::size_t index = 0; index < count; ++index) {
            updates.push_back(PointUpdate{index_dist(rng_), value_dist(rng_)});
        }
        return updates;
    }

    void reseed(unsigned seed) { rng_.seed(seed); }

private:
    std::mt19937 rng_;
};

}  
