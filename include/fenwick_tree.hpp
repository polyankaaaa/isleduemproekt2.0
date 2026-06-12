#pragma once

#include "operation_counter.hpp"

#include <cstddef>
#include <cstdint>
#include <vector>

namespace semga67 {


class FenwickTreeRsq {
public:
    FenwickTreeRsq(const std::vector<std::int64_t>& data, OperationCounter& counter) { build(data, counter); }

    void update(std::size_t index, std::int64_t value, OperationCounter& counter) {
        if (index >= size_) {
            return;
        }
        const std::int64_t delta = value - values_[index];
        values_[index] = value;
        add_delta(index, delta, counter);
    }

    [[nodiscard]] std::int64_t query(std::size_t left, std::size_t right,
                                     OperationCounter& counter) const {
        if (size_ == 0 || left > right) {
            return 0;
        }
        const std::int64_t right_sum = prefix_sum(right, counter);
        if (left == 0) {
            return right_sum;
        }
        counter.add();
        return right_sum - prefix_sum(left - 1, counter);
    }

    [[nodiscard]] std::size_t size() const noexcept { return size_; }

private:
    void build(const std::vector<std::int64_t>& data, OperationCounter& counter) {
        size_ = data.size();
        values_ = data;
        tree_.assign(size_ + 1, 0);

        for (std::size_t index = 0; index < size_; ++index) {
            tree_[index + 1] = data[index];
        }

        for (std::size_t index = 1; index <= size_; ++index) {
            const std::size_t parent = index + lowbit(index);
            if (parent <= size_) {
                counter.add();
                tree_[parent] += tree_[index];
            }
        }
    }

    void add_delta(std::size_t index, std::int64_t delta, OperationCounter& counter) {
        for (std::size_t bit_index = index + 1; bit_index <= size_; bit_index += lowbit(bit_index)) {
            counter.add();
            tree_[bit_index] += delta;
        }
    }

    [[nodiscard]] std::int64_t prefix_sum(std::size_t index, OperationCounter& counter) const {
        std::int64_t sum = 0;
        for (std::size_t bit_index = index + 1; bit_index > 0; bit_index -= lowbit(bit_index)) {
            counter.add();
            sum += tree_[bit_index];
        }
        return sum;
    }

    [[nodiscard]] static std::size_t lowbit(std::size_t value) noexcept { return value & (~value + 1); }

    std::size_t size_{0};
    std::vector<std::int64_t> values_;
    std::vector<std::int64_t> tree_;
};

}  
