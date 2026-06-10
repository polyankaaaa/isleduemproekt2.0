#pragma once

#include "operation_counter.hpp"
#include "types.hpp"

#include <cstddef>
#include <cstdint>
#include <vector>

namespace semga67 {


class PrefixSum2D {
public:
    PrefixSum2D(const std::vector<std::vector<std::int64_t>>& matrix, OperationCounter& counter) {
        build(matrix, counter);
    }

    [[nodiscard]] std::int64_t query(const RectQuery& rect, OperationCounter& counter) const {
        const std::size_t r1 = rect.row_top;
        const std::size_t c1 = rect.col_left;
        const std::size_t r2 = rect.row_bottom;
        const std::size_t c2 = rect.col_right;

        counter.add(4);  
        return prefix_[r2 + 1][c2 + 1] - prefix_[r1][c2 + 1] - prefix_[r2 + 1][c1] +
               prefix_[r1][c1];
    }

    [[nodiscard]] std::size_t rows() const noexcept {
        return prefix_.empty() ? 0 : prefix_.size() - 1;
    }

    [[nodiscard]] std::size_t cols() const noexcept {
        return (prefix_.empty() || prefix_[0].empty()) ? 0 : prefix_[0].size() - 1;
    }

private:
    void build(const std::vector<std::vector<std::int64_t>>& matrix, OperationCounter& counter) {
        const std::size_t row_count = matrix.size();
        const std::size_t col_count = row_count == 0 ? 0 : matrix[0].size();

        prefix_.assign(row_count + 1, std::vector<std::int64_t>(col_count + 1, 0));

        for (std::size_t row = 0; row < row_count; ++row) {
            for (std::size_t col = 0; col < col_count; ++col) {
                counter.add(3);  
                prefix_[row + 1][col + 1] =
                    prefix_[row][col + 1] + prefix_[row + 1][col] - prefix_[row][col] +
                    matrix[row][col];
            }
        }
    }

    std::vector<std::vector<std::int64_t>> prefix_;
};

}  