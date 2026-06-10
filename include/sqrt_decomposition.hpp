#pragma once

#include "operation_counter.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <utility>
#include <vector>

namespace semga67 {


template <typename T, typename Combine>
class SqrtDecomposition {
public:
    SqrtDecomposition(std::vector<T> data, T identity, Combine combine, OperationCounter& counter)
        : data_(std::move(data)),
          identity_(identity),
          combine_(std::move(combine)) {
        build(counter);
    }

    [[nodiscard]] T query(std::size_t left, std::size_t right, OperationCounter& counter) const {
        if (data_.empty() || left > right) {
            return identity_;
        }

        T result = identity_;

        while (left <= right && left % block_size_ != 0) {
            counter.add();
            result = combine_(result, data_[left]);
            ++left;
        }

        while (left + block_size_ <= right + 1) {
            counter.add();
            result = combine_(result, blocks_[left / block_size_]);
            left += block_size_;
        }

        while (left <= right) {
            counter.add();
            result = combine_(result, data_[left]);
            ++left;
        }

        return result;
    }

    [[nodiscard]] std::size_t size() const noexcept { return data_.size(); }

    [[nodiscard]] std::size_t block_size() const noexcept { return block_size_; }

private:
    void build(OperationCounter& counter) {
        const std::size_t n = data_.size();
        block_size_ = n == 0 ? 1 : std::max<std::size_t>(1, static_cast<std::size_t>(std::sqrt(n)));

        const std::size_t block_count = (n + block_size_ - 1) / block_size_;
        blocks_.assign(block_count, identity_);

        for (std::size_t index = 0; index < n; ++index) {
            const std::size_t block_index = index / block_size_;
            counter.add();
            blocks_[block_index] = combine_(blocks_[block_index], data_[index]);
        }
    }

    std::vector<T> data_;
    std::vector<T> blocks_;
    T identity_;
    Combine combine_;
    std::size_t block_size_{1};
};

template <typename T, typename Combine>
[[nodiscard]] SqrtDecomposition<T, std::decay_t<Combine>> make_sqrt_decomposition(
    std::vector<T> data, T identity, Combine combine, OperationCounter& counter) {
    return SqrtDecomposition<T, std::decay_t<Combine>>(
        std::move(data), identity, std::move(combine), counter);
}

[[nodiscard]] inline auto make_sqrt_rsq(std::vector<std::int64_t> data, OperationCounter& counter) {
    return make_sqrt_decomposition(
        std::move(data), std::int64_t{0},
        [](std::int64_t lhs, std::int64_t rhs) { return lhs + rhs; }, counter);
}

[[nodiscard]] inline auto make_sqrt_rmq(std::vector<std::int64_t> data, OperationCounter& counter) {
    return make_sqrt_decomposition(
        std::move(data), std::numeric_limits<std::int64_t>::max(),
        [](std::int64_t lhs, std::int64_t rhs) { return std::min(lhs, rhs); }, counter);
}

}  
