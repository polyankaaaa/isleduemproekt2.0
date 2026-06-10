#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Semga67 Lab 3\n";
        std::cout << "Commands:\n";
        std::cout << "  verify     run correctness checks\n";
        std::cout << "  benchmark  run operation-count benchmarks\n";
        return 0;
    }

    std::string command = argv[1];

    if (command == "verify") {
        std::cout << "Verify is not implemented yet.\n";
        return 0;
    }

    if (command == "benchmark") {
        std::cout << "Benchmark is not implemented yet.\n";
        return 0;
    }

    std::cerr << "Unknown command: " << command << "\n";
    return 1;
}