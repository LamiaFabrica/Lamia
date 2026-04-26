#pragma once

#include <filesystem>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

namespace lamia::runtime {

class shared_library_error final : public std::runtime_error {
public:
    explicit shared_library_error(std::string message);
};

enum class binding_mode {
    lazy,
    immediate
};

enum class symbol_visibility {
    local,
    global
};

struct shared_library_options {
    binding_mode binding = binding_mode::immediate;
    symbol_visibility visibility = symbol_visibility::local;
    bool allow_absolute_paths = false;
};

struct shared_library_descriptor {
    std::string logical_name;
    std::string root_name;
    std::filesystem::path relative_path;
};

class shared_library final {
public:
    shared_library() noexcept;
    ~shared_library();

    shared_library(const shared_library&) = delete;
    shared_library& operator=(const shared_library&) = delete;

    shared_library(shared_library&& other) noexcept;
    shared_library& operator=(shared_library&& other) noexcept;

    [[nodiscard]] static shared_library open(const std::filesystem::path& resolved_path,
                                             const shared_library_options& options = {});

    [[nodiscard]] bool is_loaded() const noexcept;
    [[nodiscard]] const std::filesystem::path& path() const noexcept;
    [[nodiscard]] void* native_handle() const noexcept;

    [[nodiscard]] void* raw_symbol(std::string_view symbol_name) const;

    template <typename FunctionPointer>
    [[nodiscard]] FunctionPointer symbol(std::string_view symbol_name) const
    {
        void* symbol_address = raw_symbol(symbol_name);
        auto typed_symbol = reinterpret_cast<FunctionPointer>(symbol_address);
        return typed_symbol;
    }

    void close();

private:
    using native_handle_type = void*;

    shared_library(std::filesystem::path resolved_path, native_handle_type handle) noexcept;

    void release_noexcept() noexcept;

    std::filesystem::path resolved_path_;
    native_handle_type handle_;
};

class shared_library_locator final {
public:
    shared_library_locator();

    void add_root(std::string root_name, std::filesystem::path root_path);
    void map_library(std::string logical_name, std::string root_name, std::filesystem::path relative_path);

    [[nodiscard]] std::vector<shared_library_descriptor> descriptors() const;
    [[nodiscard]] std::vector<std::filesystem::path> roots() const;
    [[nodiscard]] std::filesystem::path resolve(std::string_view logical_name) const;
    [[nodiscard]] std::filesystem::path resolve_path(const std::filesystem::path& requested_path,
                                                     const shared_library_options& options = {}) const;

    [[nodiscard]] shared_library load(std::string_view logical_name,
                                      const shared_library_options& options = {}) const;
    [[nodiscard]] shared_library open_path(const std::filesystem::path& requested_path,
                                           const shared_library_options& options = {}) const;

private:
    struct mapped_library {
        std::string root_name;
        std::filesystem::path relative_path;
    };

    [[nodiscard]] static std::filesystem::path canonical_directory(const std::filesystem::path& path);
    [[nodiscard]] static std::filesystem::path normalise_relative_path(const std::filesystem::path& path);
    [[nodiscard]] static bool is_path_within_root(const std::filesystem::path& candidate,
                                                  const std::filesystem::path& root);
    [[nodiscard]] static std::vector<std::filesystem::path> decorated_names(const std::filesystem::path& requested_path);

    mutable std::mutex mutex_;
    std::unordered_map<std::string, std::filesystem::path> roots_;
    std::unordered_map<std::string, mapped_library> libraries_;
};

} // namespace lamia::runtime
