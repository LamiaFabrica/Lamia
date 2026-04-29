// lamia.runtime — Execution engine, VM. C++26.
// © 2025 D Hargreaves AKA Roylepython | MIT licensed in this repository.
// Version 0.5.0 — Public-ready.

module;
#include <string>
#include <string_view>
#include <vector>
#include <memory>
#include <cstdint>
#include <expected>

export module lamia.runtime;

import lamia.core;

export namespace lamia::runtime {

enum class Opcode : std::uint8_t {
    Nop, Const, Add, Sub, Mul, Div,
    Load, Store, Call, Ret, Jump, JumpIfNot,
    Halt
};

struct Instruction {
    Opcode op{Opcode::Nop};
    std::int32_t arg0{0};
    std::int32_t arg1{0};
};

class ExecutionEngine {
public:
    ExecutionEngine() = default;
    [[nodiscard]] lamia::Result<void, lamia::Error> load(std::vector<Instruction> program);
    [[nodiscard]] lamia::Result<void, lamia::Error> run();
    [[nodiscard]] std::string get_status() const { return status_; }
    void reset();

private:
    std::vector<Instruction> program_;
    std::vector<std::int64_t> stack_;
    std::vector<std::int64_t> locals_;
    std::size_t pc_{0};
    std::string status_{"idle"};
};

class VM {
public:
    VM() = default;
    [[nodiscard]] bool initialize();
    [[nodiscard]] lamia::Result<void, lamia::Error> execute_bytecode(std::vector<Instruction> const& code);
    [[nodiscard]] std::string get_status() const { return engine_.get_status(); }
    void shutdown() { engine_.reset(); }

private:
    ExecutionEngine engine_;
    bool initialized_{false};
};

inline lamia::Result<void, lamia::Error> ExecutionEngine::load(std::vector<Instruction> program) {
    program_ = std::move(program);
    stack_.clear();
    locals_.clear();
    pc_ = 0;
    status_ = "loaded";
    return lamia::Result<void, lamia::Error>::ok();
}
inline lamia::Result<void, lamia::Error> ExecutionEngine::run() {
    status_ = "running";
    while (pc_ < program_.size()) {
        Instruction const& i = program_[pc_++];
        switch (i.op) {
            case Opcode::Nop: break;
            case Opcode::Const: stack_.push_back(static_cast<std::int64_t>(i.arg0)); break;
            case Opcode::Add: if (stack_.size() < 2) return lamia::Result<void, lamia::Error>::err(lamia::Error(1, "stack underflow")); { std::int64_t b = stack_.back(); stack_.pop_back(); stack_.back() += b; } break;
            case Opcode::Sub: if (stack_.size() < 2) return lamia::Result<void, lamia::Error>::err(lamia::Error(1, "stack underflow")); { std::int64_t b = stack_.back(); stack_.pop_back(); stack_.back() -= b; } break;
            case Opcode::Mul: if (stack_.size() < 2) return lamia::Result<void, lamia::Error>::err(lamia::Error(1, "stack underflow")); { std::int64_t b = stack_.back(); stack_.pop_back(); stack_.back() *= b; } break;
            case Opcode::Div: if (stack_.size() < 2) return lamia::Result<void, lamia::Error>::err(lamia::Error(1, "stack underflow")); { std::int64_t b = stack_.back(); stack_.pop_back(); stack_.back() = (b != 0 ? stack_.back() / b : 0); } break;
            case Opcode::Load: if (static_cast<std::size_t>(i.arg0) >= locals_.size()) locals_.resize(static_cast<std::size_t>(i.arg0) + 1, 0); stack_.push_back(locals_[static_cast<std::size_t>(i.arg0)]); break;
            case Opcode::Store: if (stack_.empty()) return lamia::Result<void, lamia::Error>::err(lamia::Error(1, "stack underflow")); if (static_cast<std::size_t>(i.arg0) >= locals_.size()) locals_.resize(static_cast<std::size_t>(i.arg0) + 1, 0); locals_[static_cast<std::size_t>(i.arg0)] = stack_.back(); stack_.pop_back(); break;
            case Opcode::Jump: pc_ = static_cast<std::size_t>(i.arg0); break;
            case Opcode::JumpIfNot: if (stack_.empty()) return lamia::Result<void, lamia::Error>::err(lamia::Error(1, "stack underflow")); if (!stack_.back()) pc_ = static_cast<std::size_t>(i.arg0); stack_.pop_back(); break;
            case Opcode::Call: case Opcode::Ret: break;
            case Opcode::Halt: status_ = "halted"; return lamia::Result<void, lamia::Error>::ok();
        }
    }
    status_ = "halted";
    return lamia::Result<void, lamia::Error>::ok();
}
inline void ExecutionEngine::reset() {
    program_.clear();
    stack_.clear();
    locals_.clear();
    pc_ = 0;
    status_ = "idle";
}

inline bool VM::initialize() {
    if (initialized_) return true;
    initialized_ = true;
    return true;
}
inline lamia::Result<void, lamia::Error> VM::execute_bytecode(std::vector<Instruction> const& code) {
    if (!initialized_) return lamia::Result<void, lamia::Error>::err(lamia::Error(1, "VM not initialized"));
    auto r = engine_.load(std::vector<Instruction>(code));
    if (!r.is_ok()) return r;
    return engine_.run();
}

} // namespace lamia::runtime
