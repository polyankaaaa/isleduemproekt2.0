#include "benchmark.hpp"
#include "operation_counter.hpp"
#include "verify.hpp"

#include <cstdlib>
#include <iostream>
#include <string_view>

namespace {

void print_usage(std::string_view program_name) {
    std::cout << "Semga67 — лабораторная работа №3 (структуры данных для запросов на отрезках)\n\n"
              << "Использование:\n"
              << "  " << program_name << " --help\n"
              << "  " << program_name << " self-test\n"
              << "  " << program_name << " verify\n"
              << "  " << program_name << " benchmark\n";
}

bool run_self_test() {
    semga67::OperationCounter counter;
    counter.add(3);
    counter.add(2);
    if (counter.get() != 5) {
        std::cerr << "self-test: ошибка счётчика операций\n";
        return false;
    }

    counter.reset();
    if (counter.get() != 0) {
        std::cerr << "self-test: ошибка сброса счётчика\n";
        return false;
    }

    semga67::OperationCounter scoped_storage;
    {
        semga67::ScopedOperationCounter scoped(scoped_storage);
        scoped.counter().add(7);
    }
    if (scoped_storage.get() != 7) {
        std::cerr << "self-test: ошибка ScopedOperationCounter\n";
        return false;
    }

    std::cout << "self-test: OK\n";
    return true;
}

}

int main(int argc, char* argv[]) {
    const std::string_view program_name = (argc > 0) ? argv[0] : "semga67_cli";

    if (argc < 2) {
        print_usage(program_name);
        return EXIT_SUCCESS;
    }

    const std::string_view command = argv[1];
    if (command == "--help" || command == "-h") {
        print_usage(program_name);
        return EXIT_SUCCESS;
    }

    if (command == "self-test") {
        return run_self_test() ? EXIT_SUCCESS : EXIT_FAILURE;
    }

    if (command == "verify") {
        return semga67::run_verify() ? EXIT_SUCCESS : EXIT_FAILURE;
    }

    if (command == "benchmark") {
        return semga67::run_benchmark() ? EXIT_SUCCESS : EXIT_FAILURE;
    }

    std::cerr << "Неизвестная команда: " << command << '\n';
    print_usage(program_name);
    return EXIT_FAILURE;
}
