// Test lamia.runtime 0.5.0: bytecode (Const, Add, Halt).
// © 2025 D Hargreaves AKA Roylepython | MIT licensed in this repository.

module;
#include <cstdio>
#include <cstdlib>
#include <new>
#include <vector>
#include <string>
import lamia.core;
import lamia.runtime;

int main() {
    using namespace lamia;
    using namespace lamia::runtime;

    if (!initialize()) {
        std::fprintf(stderr, "FAIL: initialize()\n");
        return EXIT_FAILURE;
    }

    VM vm;
    if (!vm.initialize()) {
        std::fprintf(stderr, "FAIL: VM::initialize()\n");
        shutdown();
        return EXIT_FAILURE;
    }

    std::vector<Instruction> code = {
        {Opcode::Const, 2, 0},
        {Opcode::Const, 3, 0},
        {Opcode::Add, 0, 0},
        {Opcode::Halt, 0, 0}
    };

    auto r = vm.execute_bytecode(code);
    if (!r.is_ok()) {
        std::fprintf(stderr, "FAIL: execute_bytecode() error %d %s\n", r.error().code, r.error().message);
        shutdown();
        return EXIT_FAILURE;
    }

    if (vm.get_status() != "halted") {
        std::fprintf(stderr, "FAIL: expected status halted, got %s\n", vm.get_status().c_str());
        shutdown();
        return EXIT_FAILURE;
    }

    vm.shutdown();
    shutdown();
    std::fprintf(stdout, "PASS: lamia.runtime test (0.5.0)\n");
    return EXIT_SUCCESS;
}
