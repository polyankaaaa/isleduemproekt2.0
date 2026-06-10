#pragma once

#include <cstdint>

namespace semga67 {


class OperationCounter {
public:
    void reset() noexcept { count_ = 0; }

    void add(std::uint64_t value = 1) noexcept { count_ += value; }

    [[nodiscard]] std::uint64_t get() const noexcept { return count_; }

private:
    std::uint64_t count_{0};
};


class ScopedOperationCounter {
public:
    explicit ScopedOperationCounter(OperationCounter& counter) noexcept
        : counter_(counter) {
        counter_.reset();
    }

    ScopedOperationCounter(const ScopedOperationCounter&) = delete;
    ScopedOperationCounter& operator=(const ScopedOperationCounter&) = delete;

    ~ScopedOperationCounter() = default;

    [[nodiscard]] OperationCounter& counter() noexcept { return counter_; }

private:
    OperationCounter& counter_;
};

}  
