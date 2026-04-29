// lamia.core — Base types, memory management, error handling. C++26.
// © 2025 D Hargreaves AKA Roylepython | MIT licensed in this repository.
// Version 0.5.0 — Public-ready core.

module;
#include <string>
#include <vector>
#include <memory>
#include <cstddef>
#include <cstring>

export module lamia.core;

export namespace lamia {

// -----------------------------------------------------------------------------
// Error — no std::string so consumer TU needs no C++ std headers for Result<T,Error>.
// -----------------------------------------------------------------------------
struct Error {
    static constexpr std::size_t message_capacity = 256;
    int code{0};
    char message[message_capacity]{};

    Error() = default;
    Error(int c, char const* msg) : code(c) { set_message(msg); }

    [[nodiscard]] bool ok() const noexcept { return code == 0; }

    void set_message(char const* msg) noexcept {
        if (!msg) { message[0] = '\0'; return; }
        std::size_t i = 0;
        while (i < message_capacity - 1 && msg[i]) { message[i] = msg[i]; ++i; }
        message[i] = '\0';
    }
};

// -----------------------------------------------------------------------------
// Result<T, E> — union + placement new, no std::optional.
// -----------------------------------------------------------------------------
template<typename T, typename E = Error>
class Result {
public:
    Result() noexcept : is_ok_(true), active_(false) {}

    static Result ok(T value) {
        Result r;
        new (&r.value_) T(static_cast<T&&>(value));
        r.is_ok_ = true;
        r.active_ = true;
        return r;
    }
    static Result err(E e) {
        Result r;
        new (&r.err_) E(static_cast<E&&>(e));
        r.is_ok_ = false;
        r.active_ = true;
        return r;
    }

    Result(Result&& other) noexcept {
        is_ok_ = other.is_ok_;
        active_ = other.active_;
        if (active_) {
            if (is_ok_) {
                new (&value_) T(static_cast<T&&>(other.value_));
                other.value_.~T();
            } else {
                new (&err_) E(static_cast<E&&>(other.err_));
                other.err_.~E();
            }
            other.active_ = false;
        }
    }
    Result& operator=(Result&& other) noexcept {
        if (this == &other) return *this;
        destroy();
        is_ok_ = other.is_ok_;
        active_ = other.active_;
        if (active_) {
            if (is_ok_) {
                new (&value_) T(static_cast<T&&>(other.value_));
                other.value_.~T();
            } else {
                new (&err_) E(static_cast<E&&>(other.err_));
                other.err_.~E();
            }
            other.active_ = false;
        }
        return *this;
    }
    ~Result() { destroy(); }

    Result(Result const&) = delete;
    Result& operator=(Result const&) = delete;

    [[nodiscard]] bool is_ok() const noexcept { return is_ok_; }
    [[nodiscard]] bool is_err() const noexcept { return !is_ok_; }

    [[nodiscard]] T& value() { return value_; }
    [[nodiscard]] T const& value() const { return value_; }
    [[nodiscard]] E& error() { return err_; }
    [[nodiscard]] E const& error() const { return err_; }

    [[nodiscard]] T unwrap_or(T default_val) const {
        return active_ && is_ok_ ? value_ : default_val;
    }

private:
    void destroy() noexcept {
        if (!active_) return;
        if (is_ok_) value_.~T();
        else err_.~E();
        active_ = false;
    }

    union {
        T value_;
        E err_;
    };
    bool is_ok_;
    bool active_;
};

template<typename E>
class Result<void, E> {
public:
    Result() noexcept : is_err_(false), active_(false) {}
    static Result ok() { Result r; r.is_err_ = false; r.active_ = true; return r; }
    static Result err(E e) {
        Result r;
        new (&r.err_) E(static_cast<E&&>(e));
        r.is_err_ = true;
        r.active_ = true;
        return r;
    }
    Result(Result&& other) noexcept {
        is_err_ = other.is_err_;
        active_ = other.active_;
        if (active_ && is_err_) {
            new (&err_) E(static_cast<E&&>(other.err_));
            other.err_.~E();
            other.active_ = false;
        }
    }
    ~Result() { if (active_ && is_err_) err_.~E(); }
    Result(Result const&) = delete;
    Result& operator=(Result const&) = delete;

    [[nodiscard]] bool is_ok() const noexcept { return !is_err_; }
    [[nodiscard]] bool is_err() const noexcept { return is_err_; }
    [[nodiscard]] E& error() { return err_; }
    [[nodiscard]] E const& error() const { return err_; }

private:
    union { E err_; };
    bool is_err_;
    bool active_;
};

// -----------------------------------------------------------------------------
// MemoryPool
// -----------------------------------------------------------------------------
class MemoryPool {
public:
    explicit MemoryPool(std::size_t block_size, std::size_t block_count)
        : block_size_(block_size), block_count_(block_count) {
        blocks_.reserve(block_count_);
        free_list_.reserve(block_count_);
        for (std::size_t i = 0; i < block_count_; ++i) {
            blocks_.emplace_back(block_size_);
            free_list_.push_back(blocks_.back().data());
        }
    }
    ~MemoryPool() = default;

    void* allocate() {
        if (free_list_.empty()) return nullptr;
        void* p = free_list_.back();
        free_list_.pop_back();
        ++used_;
        return p;
    }
    void deallocate(void* ptr) noexcept {
        if (!ptr) return;
        free_list_.push_back(ptr);
        --used_;
    }
    [[nodiscard]] std::size_t block_size() const noexcept { return block_size_; }
    [[nodiscard]] std::size_t block_count() const noexcept { return block_count_; }
    [[nodiscard]] std::size_t used() const noexcept { return used_; }

    MemoryPool(MemoryPool const&) = delete;
    MemoryPool& operator=(MemoryPool const&) = delete;

private:
    std::size_t block_size_;
    std::size_t block_count_;
    std::size_t used_{0};
    std::vector<std::vector<std::byte>> blocks_;
    std::vector<void*> free_list_;
};

// -----------------------------------------------------------------------------
// Core runtime — version 0.5.0
// -----------------------------------------------------------------------------
inline bool g_lamia_initialized = false;

[[nodiscard]] inline bool initialize() {
    if (g_lamia_initialized) return true;
    g_lamia_initialized = true;
    return true;
}
inline void shutdown() { g_lamia_initialized = false; }
[[nodiscard]] inline std::string get_version() { return "0.5.0"; }
[[nodiscard]] inline char const* get_version_cstr() noexcept { return "0.5.0"; }

} // namespace lamia
