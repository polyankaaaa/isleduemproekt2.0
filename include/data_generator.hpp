#pragma once

#include <cstddef>
#include <cstdint>
#include <random>
#include <vector>

namespace semga67 {

class DataGenerator {
public:
    explicit DataGenerator(unsigned seed = 42) : rng_(seed) {}

    [[nodiscard]] std::vector<std::int64_t> random_array(std::size_t size,
                                                         std::int64_t min_value = -1'000'000,
                                                         std::int64_t max_value = 1'000'000) {
        std::uniform_int_distribution<std::int64_t> dist(min_value, max_value);
        std::vector<std::int64_t> data(size);
        for (std::int64_t& value : data) {
            value = dist(rng_);
        }
        return data;
    }

    [[nodiscard]] std::vector<std::vector<std::int64_t>> random_matrix(
        std::size_t rows, std::size_t cols, std::int64_t min_value = -1'000'000,
        std::int64_t max_value = 1'000'000) {
        std::uniform_int_distribution<std::int64_t> dist(min_value, max_value);
        std::vector<std::vector<std::int64_t>> matrix(rows, std::vector<std::int64_t>(cols));
        for (auto& row : matrix) {
            for (std::int64_t& value : row) {
                value = dist(rng_);
            }
        }
        return matrix;
    }

    void reseed(unsigned seed) { rng_.seed(seed); }

private:
    std::mt19937 rng_;
};

}  
