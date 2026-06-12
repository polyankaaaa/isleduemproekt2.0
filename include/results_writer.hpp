#pragma once

#include "types.hpp"

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace semga67 {

class ResultsWriter {
public:
    explicit ResultsWriter(std::filesystem::path output_directory = "results")
        : output_directory_(std::move(output_directory)) {
        std::filesystem::create_directories(output_directory_);
    }

    void write_1d_csv(const std::string& filename, const std::vector<BenchmarkResult>& results) const {
        std::ofstream output(output_directory_ / filename);
        output << "structure,n,build_ops,avg_query_ops,avg_update_ops\n";
        for (const BenchmarkResult& result : results) {
            output << result.structure_name << ',' << result.n << ',' << result.build_ops << ','
                   << result.avg_query_ops << ',' << result.avg_update_ops << '\n';
        }
    }

    void write_2d_csv(const std::string& filename, const std::vector<BenchmarkResult>& results) const {
        std::ofstream output(output_directory_ / filename);
        output << "structure,n,m,build_ops,avg_query_ops,avg_update_ops\n";
        for (const BenchmarkResult& result : results) {
            output << result.structure_name << ',' << result.n << ',' << result.m << ','
                   << result.build_ops << ',' << result.avg_query_ops << ','
                   << result.avg_update_ops << '\n';
        }
    }

    [[nodiscard]] const std::filesystem::path& output_directory() const noexcept {
        return output_directory_;
    }

private:
    std::filesystem::path output_directory_;
};

}  
