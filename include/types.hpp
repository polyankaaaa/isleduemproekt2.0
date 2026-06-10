#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

namespace semga67 {

struct RangeQuery {
    std::size_t left{0};
    std::size_t right{0};
};

struct RectQuery {
    std::size_t row_top{0};
    std::size_t col_left{0};
    std::size_t row_bottom{0};
    std::size_t col_right{0};
};

struct PointUpdate {
    std::size_t index{0};
    std::int64_t value{0};
};

struct BenchmarkResult {
    std::string structure_name;
    std::size_t n{0};
    std::size_t m{0};  // 0 для одномерных структур
    std::uint64_t build_ops{0};
    double avg_query_ops{0.0};
    double avg_update_ops{0.0};
};

}  // namespace semga67
