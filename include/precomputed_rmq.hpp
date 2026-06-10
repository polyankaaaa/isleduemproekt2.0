#pragma once

#include "operation_counter.hpp"

#include <cstddef>
#include <cstdint>
#include <vector>

namespace semga67 {


class PrecomputedRMQ {
public:
    PrecomputedRMQ(const std::vector<std::int64_t>& data, OperationCounter& counter) {
        build(data, counter);
    }

    [[nodiscard]] std::int64_t query(std::size_t left, std::size_t right,
                                     OperationCounter& counter) const {
        counter.add();  
        return table_[left][right];
    }

    [[nodiscard]] std::size_t size() const noexcept { return size_; }

private:
    void build(const std::vector<std::int64_t>& data, OperationCounter& counter) {
        size_ = data.size();
        table_.assign(size_, std::vector<std::int64_t>(size_, 0));

        for (std::size_t left = 0; left < size_; ++left) {
            table_[left][left] = data[left];
            for (std::size_t right = left + 1; right < size_; ++right) {
                counter.add();  
                table_[left][right] = std::min(table_[left][right - 1], data[right]);
            }
        }
    }

    std::size_t size_{0};
    std::vector<std::vector<std::int64_t>> table_;
};

}
