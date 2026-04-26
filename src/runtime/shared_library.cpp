#include "lamia/runtime/shared_library.hpp"

#include <algorithm>
#include <cerrno>
#include <cstring>
#include <sstream>
#include <system_error>

#if defined(_WIN32)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#else
#include <dlfcn.h>
#endif

namespace lamia::runtime {
namespace {

[[nodiscard]] std::string path_to_string(const std::filesystem::path& path)
{
    auto value = path.u8string();
    std::string converted(value.begin(), value.end());
    return converted;
}

[[nodiscard]] std::string quote_path(const std::filesystem::path& path)
{
    std::string rendered = path_to_string(path);
    return "'" + rendered + "'";
}

[[nodiscard]] std::string last_loader_error()
{
#if defined(_WIN32)
    const DWORD error_code = GetLastError();
    if (error_code == 0U) {
        std::string message = "Windows loader did not provide an error code";
        return message;
    }

    LPWSTR buffer = nullptr;
    const DWORD flags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
    const DWORD size = FormatMessageW(flags,
                                      nullptr,
                                      error_code,
                                      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                                      reinterpret_cast<LPWSTR>(&buffer),
                                      0,
                                      nullptr);
    if (size == 0U || buffer == nullptr) {
        std::ostringstream stream;
        stream << "Windows loader error code " << error_code;
        return stream.str();
    }

    std::wstring wide_message(buffer, size);
    LocalFree(buffer);
    std::string message(wide_message.begin(), wide_message.end());
    while (!message.empty() && (message.back() == '\n' || message.back() == '\r' || message.back() == ' ')) {
        message.pop_back();
    }
    return message;
#else
    const char* error_message = dlerror();
    if (error_message != nullptr && std::strlen(error_message) > 0U) {
        std::string message(error_message);
        return message;
    }

    std::string message = std::strerror(errno);
    if (!message.empty()) {
        return message;
    }

    std::string fallback = "dynamic loader did not provide an error";
    return fallback;
#endif
}

[[nodiscard]] std::filesystem::path canonical_existing_path(const std::filesystem::path& path)
{
    std::error_code error;
    auto canonical = std::filesystem::canonical(path, error);
    if (!error) {
        return canonical;
    }

    std::ostringstream stream;
    stream << "Path " << quote_path(path) << " cannot be resolved: " << error.message();
    throw shared_library_error(stream.str());
}

[[nodiscard]] bool has_shared_library_extension(const std::filesystem::path& path)
{
    const std::string extension = path.extension().string();
#if defined(_WIN32)
    const bool recognised = extension == ".dll";
#elif defined(__APPLE__)
    const bool recognised = extension == ".dylib" || extension == ".so";
#else
    const bool recognised = extension == ".so";
#endif
    return recognised;
}

[[nodiscard]] std::filesystem::path native_library_name(const std::filesystem::path& requested_path)
{
    const std::filesystem::path parent = requested_path.parent_path();
    const std::string stem = requested_path.filename().string();

    if (stem.empty()) {
        std::ostringstream stream;
        stream << "Library path " << quote_path(requested_path) << " does not contain a file name";
        throw shared_library_error(stream.str());
    }

#if defined(_WIN32)
    std::filesystem::path file_name = has_shared_library_extension(requested_path) ? requested_path.filename()
                                                                                   : std::filesystem::path(stem + ".dll");
#elif defined(__APPLE__)
    const bool already_named = has_shared_library_extension(requested_path);
    std::string decorated = stem;
    if (!already_named && decorated.rfind("lib", 0) != 0) {
        decorated = "lib" + decorated;
    }
    std::filesystem::path file_name = already_named ? requested_path.filename()
                                                    : std::filesystem::path(decorated + ".dylib");
#else
    const bool already_named = has_shared_library_extension(requested_path);
    std::string decorated = stem;
    if (!already_named && decorated.rfind("lib", 0) != 0) {
        decorated = "lib" + decorated;
    }
    std::filesystem::path file_name = already_named ? requested_path.filename()
                                                    : std::filesystem::path(decorated + ".so");
#endif

    std::filesystem::path candidate = parent / file_name;
    return candidate.lexically_normal();
}

} // namespace

shared_library_error::shared_library_error(std::string message)
    : std::runtime_error(std::move(message))
{
}

shared_library::shared_library() noexcept
    : resolved_path_(),
      handle_(nullptr)
{
}

shared_library::shared_library(std::filesystem::path resolved_path, native_handle_type handle) noexcept
    : resolved_path_(std::move(resolved_path)),
      handle_(handle)
{
}

shared_library::~shared_library()
{
    release_noexcept();
}

shared_library::shared_library(shared_library&& other) noexcept
    : resolved_path_(std::move(other.resolved_path_)),
      handle_(std::exchange(other.handle_, nullptr))
{
}

shared_library& shared_library::operator=(shared_library&& other) noexcept
{
    if (this != &other) {
        release_noexcept();
        resolved_path_ = std::move(other.resolved_path_);
        handle_ = std::exchange(other.handle_, nullptr);
    }
    return *this;
}

shared_library shared_library::open(const std::filesystem::path& resolved_path, const shared_library_options& options)
{
    const std::filesystem::path canonical_path = canonical_existing_path(resolved_path);
    if (!std::filesystem::is_regular_file(canonical_path)) {
        std::ostringstream stream;
        stream << "Library path " << quote_path(canonical_path) << " is not a regular file";
        throw shared_library_error(stream.str());
    }

#if defined(_WIN32)
    SetLastError(0U);
    HMODULE module = LoadLibraryW(canonical_path.wstring().c_str());
    auto handle = reinterpret_cast<native_handle_type>(module);
#else
    (void)dlerror();
    int flags = options.binding == binding_mode::lazy ? RTLD_LAZY : RTLD_NOW;
    flags |= options.visibility == symbol_visibility::global ? RTLD_GLOBAL : RTLD_LOCAL;
    native_handle_type handle = dlopen(canonical_path.c_str(), flags);
#endif

    if (handle == nullptr) {
        std::ostringstream stream;
        stream << "Failed to load shared library " << quote_path(canonical_path) << ": " << last_loader_error();
        throw shared_library_error(stream.str());
    }

    shared_library library(canonical_path, handle);
    return library;
}

bool shared_library::is_loaded() const noexcept
{
    const bool loaded = handle_ != nullptr;
    return loaded;
}

const std::filesystem::path& shared_library::path() const noexcept
{
    return resolved_path_;
}

void* shared_library::native_handle() const noexcept
{
    auto* handle = handle_;
    return handle;
}

void* shared_library::raw_symbol(std::string_view symbol_name) const
{
    if (handle_ == nullptr) {
        throw shared_library_error("Cannot resolve a symbol from an unloaded shared library");
    }

    if (symbol_name.empty()) {
        throw shared_library_error("Cannot resolve an empty symbol name");
    }

    const std::string symbol(symbol_name);
#if defined(_WIN32)
    SetLastError(0U);
    FARPROC procedure = GetProcAddress(reinterpret_cast<HMODULE>(handle_), symbol.c_str());
    auto* address = reinterpret_cast<void*>(procedure);
#else
    (void)dlerror();
    void* address = dlsym(handle_, symbol.c_str());
#endif

    if (address == nullptr) {
        std::ostringstream stream;
        stream << "Symbol '" << symbol << "' was not found in " << quote_path(resolved_path_) << ": "
               << last_loader_error();
        throw shared_library_error(stream.str());
    }

    return address;
}

void shared_library::close()
{
    if (handle_ == nullptr) {
        resolved_path_.clear();
        return;
    }

    native_handle_type closing_handle = std::exchange(handle_, nullptr);
    std::filesystem::path closing_path = std::move(resolved_path_);
    resolved_path_.clear();

#if defined(_WIN32)
    const BOOL closed = FreeLibrary(reinterpret_cast<HMODULE>(closing_handle));
    if (closed == 0) {
        std::ostringstream stream;
        stream << "Failed to unload shared library " << quote_path(closing_path) << ": " << last_loader_error();
        throw shared_library_error(stream.str());
    }
#else
    const int closed = dlclose(closing_handle);
    if (closed != 0) {
        std::ostringstream stream;
        stream << "Failed to unload shared library " << quote_path(closing_path) << ": " << last_loader_error();
        throw shared_library_error(stream.str());
    }
#endif
}

void shared_library::release_noexcept() noexcept
{
    if (handle_ == nullptr) {
        resolved_path_.clear();
        return;
    }

    native_handle_type closing_handle = std::exchange(handle_, nullptr);
    resolved_path_.clear();

#if defined(_WIN32)
    const BOOL closed = FreeLibrary(reinterpret_cast<HMODULE>(closing_handle));
    if (closed == 0) {
        SetLastError(0U);
    }
#else
    const int closed = dlclose(closing_handle);
    if (closed != 0) {
        (void)dlerror();
    }
#endif
}

shared_library_locator::shared_library_locator()
    : mutex_(),
      roots_(),
      libraries_()
{
}

void shared_library_locator::add_root(std::string root_name, std::filesystem::path root_path)
{
    if (root_name.empty()) {
        throw shared_library_error("A shared-library root requires a non-empty name");
    }

    const std::filesystem::path canonical_root = canonical_directory(root_path);

    std::scoped_lock lock(mutex_);
    auto [iterator, inserted] = roots_.try_emplace(std::move(root_name), canonical_root);
    if (!inserted) {
        iterator->second = canonical_root;
    }
}

void shared_library_locator::map_library(std::string logical_name,
                                         std::string root_name,
                                         std::filesystem::path relative_path)
{
    if (logical_name.empty()) {
        throw shared_library_error("A mapped shared library requires a non-empty logical name");
    }

    if (root_name.empty()) {
        throw shared_library_error("Mapped library '" + logical_name + "' requires a non-empty root name");
    }

    const std::filesystem::path normalised_relative_path = normalise_relative_path(relative_path);

    std::scoped_lock lock(mutex_);
    if (roots_.find(root_name) == roots_.end()) {
        throw shared_library_error("Mapped library '" + logical_name + "' references unknown root '" + root_name + "'");
    }

    mapped_library mapping{std::move(root_name), normalised_relative_path};
    libraries_.insert_or_assign(std::move(logical_name), std::move(mapping));
}

std::vector<shared_library_descriptor> shared_library_locator::descriptors() const
{
    std::scoped_lock lock(mutex_);
    std::vector<shared_library_descriptor> result;
    result.reserve(libraries_.size());

    for (const auto& [logical_name, mapping] : libraries_) {
        result.push_back(shared_library_descriptor{logical_name, mapping.root_name, mapping.relative_path});
    }

    std::ranges::sort(result, {}, &shared_library_descriptor::logical_name);
    return result;
}

std::vector<std::filesystem::path> shared_library_locator::roots() const
{
    std::scoped_lock lock(mutex_);
    std::vector<std::filesystem::path> result;
    result.reserve(roots_.size());

    for (const auto& [root_name, root_path] : roots_) {
        std::filesystem::path stored_root = root_path;
        result.push_back(std::move(stored_root));
    }

    std::ranges::sort(result);
    return result;
}

std::filesystem::path shared_library_locator::resolve(std::string_view logical_name) const
{
    if (logical_name.empty()) {
        throw shared_library_error("Cannot resolve an empty logical library name");
    }

    const std::string lookup_name(logical_name);
    std::scoped_lock lock(mutex_);
    const auto library = libraries_.find(lookup_name);
    if (library == libraries_.end()) {
        throw shared_library_error("No shared library is mapped as '" + lookup_name + "'");
    }

    const auto root = roots_.find(library->second.root_name);
    if (root == roots_.end()) {
        throw shared_library_error("Library '" + lookup_name + "' references unavailable root '" +
                                   library->second.root_name + "'");
    }

    const std::filesystem::path candidate = (root->second / library->second.relative_path).lexically_normal();
    const std::filesystem::path canonical_candidate = canonical_existing_path(candidate);
    if (!is_path_within_root(canonical_candidate, root->second)) {
        std::ostringstream stream;
        stream << "Resolved library " << quote_path(canonical_candidate) << " escaped root "
               << quote_path(root->second);
        throw shared_library_error(stream.str());
    }

    return canonical_candidate;
}

std::filesystem::path shared_library_locator::resolve_path(const std::filesystem::path& requested_path,
                                                           const shared_library_options& options) const
{
    if (requested_path.empty()) {
        throw shared_library_error("Cannot resolve an empty shared-library path");
    }

    const auto candidate_names = decorated_names(requested_path);
    std::vector<std::filesystem::path> root_snapshot;
    {
        std::scoped_lock lock(mutex_);
        root_snapshot.reserve(roots_.size());
        for (const auto& [root_name, root_path] : roots_) {
            root_snapshot.push_back(root_path);
        }
    }

    std::ostringstream attempted;
    bool wrote_attempt = false;

    if (requested_path.is_absolute()) {
        if (!options.allow_absolute_paths) {
            throw shared_library_error("Absolute library path " + quote_path(requested_path) +
                                       " was rejected because absolute loading is disabled");
        }

        for (const auto& candidate_name : candidate_names) {
            std::error_code exists_error;
            const bool exists = std::filesystem::exists(candidate_name, exists_error);
            if (exists && !exists_error) {
                const std::filesystem::path canonical_candidate = canonical_existing_path(candidate_name);
                const bool guarded_by_root = std::ranges::any_of(root_snapshot, [&](const auto& root) {
                    return is_path_within_root(canonical_candidate, root);
                });
                if (guarded_by_root) {
                    return canonical_candidate;
                }

                std::ostringstream stream;
                stream << "Absolute library path " << quote_path(canonical_candidate)
                       << " is outside every registered Lamia library root";
                throw shared_library_error(stream.str());
            }

            attempted << (wrote_attempt ? ", " : "") << quote_path(candidate_name);
            wrote_attempt = true;
        }

        throw shared_library_error("No loadable library matched absolute path candidates: " + attempted.str());
    }

    const std::filesystem::path safe_relative = normalise_relative_path(requested_path);
    for (const auto& root : root_snapshot) {
        for (const auto& candidate_name : decorated_names(safe_relative)) {
            const std::filesystem::path candidate = (root / candidate_name).lexically_normal();
            std::error_code exists_error;
            const bool exists = std::filesystem::exists(candidate, exists_error);
            if (exists && !exists_error) {
                const std::filesystem::path canonical_candidate = canonical_existing_path(candidate);
                if (is_path_within_root(canonical_candidate, root)) {
                    return canonical_candidate;
                }
            }

            attempted << (wrote_attempt ? ", " : "") << quote_path(candidate);
            wrote_attempt = true;
        }
    }

    throw shared_library_error("No loadable library matched registered roots. Attempted: " + attempted.str());
}

shared_library shared_library_locator::load(std::string_view logical_name, const shared_library_options& options) const
{
    const std::filesystem::path resolved = resolve(logical_name);
    shared_library library = shared_library::open(resolved, options);
    return library;
}

shared_library shared_library_locator::open_path(const std::filesystem::path& requested_path,
                                                 const shared_library_options& options) const
{
    const std::filesystem::path resolved = resolve_path(requested_path, options);
    shared_library library = shared_library::open(resolved, options);
    return library;
}

std::filesystem::path shared_library_locator::canonical_directory(const std::filesystem::path& path)
{
    if (path.empty()) {
        throw shared_library_error("A shared-library root cannot be an empty path");
    }

    const std::filesystem::path canonical_path = canonical_existing_path(path);
    if (!std::filesystem::is_directory(canonical_path)) {
        std::ostringstream stream;
        stream << "Shared-library root " << quote_path(canonical_path) << " is not a directory";
        throw shared_library_error(stream.str());
    }

    return canonical_path;
}

std::filesystem::path shared_library_locator::normalise_relative_path(const std::filesystem::path& path)
{
    if (path.empty()) {
        throw shared_library_error("A shared-library relative path cannot be empty");
    }

    if (path.is_absolute()) {
        throw shared_library_error("Shared-library mapping " + quote_path(path) + " must be relative");
    }

    const std::filesystem::path normalised = path.lexically_normal();
    for (const auto& part : normalised) {
        if (part == "..") {
            throw shared_library_error("Shared-library path " + quote_path(path) + " attempts to escape its root");
        }
    }

    return normalised;
}

bool shared_library_locator::is_path_within_root(const std::filesystem::path& candidate,
                                                 const std::filesystem::path& root)
{
    const auto candidate_normal = candidate.lexically_normal();
    const auto root_normal = root.lexically_normal();

    auto candidate_it = candidate_normal.begin();
    auto root_it = root_normal.begin();
    while (root_it != root_normal.end()) {
        if (candidate_it == candidate_normal.end() || *candidate_it != *root_it) {
            return false;
        }
        ++candidate_it;
        ++root_it;
    }

    const bool inside_or_equal = true;
    return inside_or_equal;
}

std::vector<std::filesystem::path> shared_library_locator::decorated_names(const std::filesystem::path& requested_path)
{
    std::vector<std::filesystem::path> names;
    names.reserve(2);

    const std::filesystem::path normalised = requested_path.lexically_normal();
    names.push_back(normalised);

    const std::filesystem::path native_name = native_library_name(normalised);
    const bool already_present = std::ranges::any_of(names, [&](const auto& existing) {
        return existing == native_name;
    });
    if (!already_present) {
        names.push_back(native_name);
    }

    return names;
}

} // namespace lamia::runtime
