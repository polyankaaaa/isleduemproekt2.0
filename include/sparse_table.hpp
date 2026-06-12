#pragma once

#include "operation_counter.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace semga67 {


class SparseTableRmq {
public:
    SparseTableRmq(const std::vector<std::int64_t>& data, OperationCounter& counter) { build(data, counter); }

    [[nodiscard]] std::int64_t query(std::size_t left, std::size_t right,
                                     OperationCounter& counter) const {
        if (size_ == 0 || left > right) {
            return 0;
        }
        const std::size_t length = right - left + 1;
        const std::size_t power = log2_[length];
        const std::size_t shift = 1U << power;

        counter.add();
        return std::min(table_[power][left], table_[power][right - shift + 1]);
    }

    [[nodiscard]] std::size_t size() const noexcept { return size_; }

private:
    void build(const std::vector<std::int64_t>& data, OperationCounter& counter) {
        size_ = data.size();
        if (size_ == 0) {
            return;
        }

        log2_.assign(size_ + 1, 0);
        for (std::size_t length = 2; length <= size_; ++length) {
            log2_[length] = log2_[length / 2] + 1;
        }

        const std::size_t levels = log2_[size_] + 1;
        table_.assign(levels, std::vector<std::int64_t>(size_, 0));
        table_[0] = data;

        for (std::size_t power = 1; power < levels; ++power) {
            const std::size_t segment_length = 1U << power;
            const std::size_t half = segment_length >> 1;
            for (std::size_t left = 0; left + segment_length <= size_; ++left) {
                counter.add();
                table_[power][left] =
                    std::min(table_[power - 1][left], table_[power - 1][left + half]);
            }
        }
    }

    std::size_t size_{0};
    std::vector<std::size_t> log2_;
    std::vector<std::vector<std::int64_t>> table_;
};

}  
