#pragma once

#include "operation_counter.hpp"

#include <cstddef>
#include <cstdint>
#include <vector>

namespace semga67 {


class PrefixSum1D {
public:
    PrefixSum1D(const std::vector<std::int64_t>& data, OperationCounter& counter) { build(data, counter); }

    [[nodiscard]] std::int64_t query(std::size_t left, std::size_t right,
                                     OperationCounter& counter) const {
        counter.add();  
        return prefix_[right + 1] - prefix_[left];
    }

    [[nodiscard]] std::size_t size() const noexcept { return prefix_.empty() ? 0 : prefix_.size() - 1; }

private:
    void build(const std::vector<std::int64_t>& data, OperationCounter& counter) {
        prefix_.assign(data.size() + 1, 0);
        for (std::size_t index = 0; index < data.size(); ++index) {
            counter.add();  
            prefix_[index + 1] = prefix_[index] + data[index];
        }
    }

    std::vector<std::int64_t> prefix_;
};

}  