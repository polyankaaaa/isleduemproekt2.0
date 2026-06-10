#pragma once

#include "operation_counter.hpp"

#include <cstddef>
#include <cstdint>
#include <vector>

namespace semga67 {


class SegmentTreeRsq {
public:
    SegmentTreeRsq(const std::vector<std::int64_t>& data, OperationCounter& counter) { build(data, counter); }

    void update(std::size_t index, std::int64_t value, OperationCounter& counter) {
        update_impl(1, 0, size_ - 1, index, value, counter);
    }

    [[nodiscard]] std::int64_t query(std::size_t left, std::size_t right,
                                     OperationCounter& counter) const {
        if (size_ == 0 || left > right) {
            return 0;
        }
        return query_impl(1, 0, size_ - 1, left, right, counter);
    }

    [[nodiscard]] std::size_t size() const noexcept { return size_; }

private:
    void build(const std::vector<std::int64_t>& data, OperationCounter& counter) {
        size_ = data.size();
        tree_.assign(4 * size_ + 4, 0);
        if (size_ == 0) {
            return;
        }
        build_impl(data, 1, 0, size_ - 1, counter);
    }

    void build_impl(const std::vector<std::int64_t>& data, std::size_t node, std::size_t left,
                    std::size_t right, OperationCounter& counter) {
        if (left == right) {
            tree_[node] = data[left];
            return;
        }

        const std::size_t middle = left + (right - left) / 2;
        build_impl(data, node * 2, left, middle, counter);
        build_impl(data, node * 2 + 1, middle + 1, right, counter);
        counter.add();
        tree_[node] = tree_[node * 2] + tree_[node * 2 + 1];
    }

    void update_impl(std::size_t node, std::size_t left, std::size_t right, std::size_t index,
                     std::int64_t value, OperationCounter& counter) {
        if (left == right) {
            tree_[node] = value;
            return;
        }

        const std::size_t middle = left + (right - left) / 2;
        if (index <= middle) {
            update_impl(node * 2, left, middle, index, value, counter);
        } else {
            update_impl(node * 2 + 1, middle + 1, right, index, value, counter);
        }
        counter.add();
        tree_[node] = tree_[node * 2] + tree_[node * 2 + 1];
    }

    [[nodiscard]] std::int64_t query_impl(std::size_t node, std::size_t left, std::size_t right,
                                          std::size_t query_left, std::size_t query_right,
                                          OperationCounter& counter) const {
        if (query_left > right || query_right < left) {
            return 0;
        }
        if (query_left <= left && right <= query_right) {
            return tree_[node];
        }

        const std::size_t middle = left + (right - left) / 2;
        const std::int64_t left_sum =
            query_impl(node * 2, left, middle, query_left, query_right, counter);
        const std::int64_t right_sum =
            query_impl(node * 2 + 1, middle + 1, right, query_left, query_right, counter);
        counter.add();
        return left_sum + right_sum;
    }

    std::size_t size_{0};
    std::vector<std::int64_t> tree_;
};

}  
