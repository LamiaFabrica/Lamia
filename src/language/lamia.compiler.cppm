// lamia.compiler — Lexer, Parser, AST, Transpiler, Compiler + options. C++26.
// © 2025 D Hargreaves AKA Roylepython | MIT licensed in this repository.
// Version 0.5.0 — Public-ready; compiler and transpiler options.

module;
#include <string>
#include <string_view>
#include <vector>
#include <memory>
#include <cstddef>
#include <expected>
#include <sstream>
#include <cctype>

export module lamia.compiler;

import lamia.core;

export namespace lamia::compiler {

// -----------------------------------------------------------------------------
// CompilerOptions — public API for compiler behaviour (0.5.0).
// -----------------------------------------------------------------------------
struct CompilerOptions {
    std::string target{"html"};   // html | js | gcode
    bool pretty_print{true};       // Indented, readable output
    bool emit_comments{false};     // Emit source-location comments in output
    bool strict{false};            // Strict parsing: reject unknown constructs
};

// -----------------------------------------------------------------------------
// TranspilerOptions — public API for transpiler behaviour (0.5.0).
// -----------------------------------------------------------------------------
struct TranspilerOptions {
    std::string target{"html"};
    bool pretty_print{true};
    bool emit_comments{false};
};

// -----------------------------------------------------------------------------
// Token
// -----------------------------------------------------------------------------
struct Token {
    enum class Type {
        Manifest, Create, Identifier, String, Number,
        Lbrace, Rbrace, Semicolon, Colon, Comma, Arrow, At,
        Lbracket, Rbracket, Newline, EndOfFile
    };
    Type type{Type::EndOfFile};
    std::string value;
    std::size_t line{0};
    std::size_t column{0};
};

// -----------------------------------------------------------------------------
// Lexer
// -----------------------------------------------------------------------------
class Lexer {
public:
    explicit Lexer(std::string_view source);
    [[nodiscard]] lamia::Result<std::vector<Token>, lamia::Error> tokenize();
    [[nodiscard]] std::vector<Token> get_tokens() const { return tokens_; }

private:
    std::string source_;
    std::vector<Token> tokens_;
    std::size_t pos_{0};
    std::size_t line_{1};
    std::size_t column_{1};
    void skip_whitespace();
    [[nodiscard]] bool at_end() const;
    [[nodiscard]] char peek() const;
    char advance();
};

// -----------------------------------------------------------------------------
// AST
// -----------------------------------------------------------------------------
enum class NodeType {
    Manifest, Create, RadiantHeading, RadiantText, RadiantButton,
    ConstellationList, RadiantQuote, GcodeBlock, ReturnLight, Startup
};

struct ASTNode {
    NodeType type{NodeType::Startup};
    std::string name;
    std::vector<std::pair<std::string, std::string>> attributes;
    std::vector<std::string> content;
    std::vector<std::shared_ptr<ASTNode>> children;

    ASTNode() = default;
    ASTNode(NodeType t, std::string n) : type(t), name(std::move(n)) {}
};

// -----------------------------------------------------------------------------
// Parser
// -----------------------------------------------------------------------------
class Parser {
public:
    explicit Parser(std::vector<Token> tokens);
    void set_strict(bool s) { strict_ = s; }
    [[nodiscard]] lamia::Result<std::shared_ptr<ASTNode>, lamia::Error> parse();

private:
    std::vector<Token> tokens_;
    std::size_t pos_{0};
    bool strict_{false};
    [[nodiscard]] Token const& current() const;
    Token advance();
    [[nodiscard]] bool at_end() const;
};

// -----------------------------------------------------------------------------
// Transpiler — options-aware (0.5.0)
// -----------------------------------------------------------------------------
class Transpiler {
public:
    [[nodiscard]] lamia::Result<std::string, lamia::Error> transpile(std::shared_ptr<ASTNode> root);
    void set_target(std::string target) { options_.target = std::move(target); }
    void set_options(TranspilerOptions const& opts) { options_ = opts; }
    [[nodiscard]] TranspilerOptions const& options() const { return options_; }

private:
    TranspilerOptions options_;
    [[nodiscard]] std::string emit_node(ASTNode const& n);
};

// -----------------------------------------------------------------------------
// Compiler — single entry point, options-aware (0.5.0)
// -----------------------------------------------------------------------------
class Compiler {
public:
    Compiler() = default;
    void set_target(std::string target) { options_.target = std::move(target); }
    void set_options(CompilerOptions const& opts) { options_ = opts; }
    [[nodiscard]] CompilerOptions const& options() const { return options_; }
    [[nodiscard]] lamia::Result<std::string, lamia::Error> compile(std::string_view source);
    [[nodiscard]] std::shared_ptr<ASTNode> ast() const { return ast_; }

private:
    CompilerOptions options_;
    std::shared_ptr<ASTNode> ast_;
};

[[nodiscard]] lamia::Result<std::string, lamia::Error> compile_source(std::string_view source);

// --- Inline implementations --------------------------------------------------
inline Lexer::Lexer(std::string_view source) : source_(source) {}
inline bool Lexer::at_end() const { return pos_ >= source_.size(); }
inline char Lexer::peek() const { return (pos_ + 1 < source_.size()) ? source_[pos_ + 1] : '\0'; }
inline char Lexer::advance() {
    if (at_end()) return '\0';
    char c = source_[pos_++];
    if (c == '\n') { line_++; column_ = 1; } else { column_++; }
    return c;
}
inline void Lexer::skip_whitespace() {
    while (!at_end() && std::isspace(static_cast<unsigned char>(source_[pos_])) && source_[pos_] != '\n')
        advance();
}

inline lamia::Result<std::vector<Token>, lamia::Error> Lexer::tokenize() {
    tokens_.clear();
    pos_ = 0; line_ = 1; column_ = 1;
    using T = Token::Type;
    while (!at_end()) {
        skip_whitespace();
        if (at_end()) break;
        char cur = source_[pos_];
        if (cur == '/' && peek() == '/') { while (!at_end() && source_[pos_] != '\n') advance(); continue; }
        if (cur == '/' && peek() == '*') {
            advance(); advance();
            while (pos_ + 1 < source_.size() && !(source_[pos_] == '*' && source_[pos_+1] == '/')) advance();
            if (pos_ + 1 < source_.size()) advance(), advance();
            continue;
        }
        if (std::isalpha(static_cast<unsigned char>(cur)) || cur == '_' || cur == '@') {
            std::size_t sl = line_, sc = column_;
            std::string v;
            while (!at_end() && (std::isalnum(static_cast<unsigned char>(source_[pos_])) || source_[pos_] == '_' || source_[pos_] == '@'))
                v += source_[pos_], advance();
            if (v == "manifest") tokens_.push_back(Token{T::Manifest, v, sl, sc});
            else if (v == "create") tokens_.push_back(Token{T::Create, v, sl, sc});
            else if (!v.empty() && v[0] == '@') tokens_.push_back(Token{T::At, v, sl, sc});
            else tokens_.push_back(Token{T::Identifier, v, sl, sc});
            continue;
        }
        if (cur == '"') {
            std::size_t sl = line_, sc = column_;
            std::string v; advance();
            while (!at_end() && source_[pos_] != '"') {
                if (source_[pos_] == '\\' && pos_+1 < source_.size()) {
                    advance();
                    if (source_[pos_] == 'n') v += '\n'; else if (source_[pos_] == 't') v += '\t';
                    else if (source_[pos_] == '"') v += '"'; else v += source_[pos_];
                } else v += source_[pos_];
                advance();
            }
            if (!at_end()) advance();
            tokens_.push_back(Token{T::String, v, sl, sc});
            continue;
        }
        if (std::isdigit(static_cast<unsigned char>(cur))) {
            std::size_t sl = line_, sc = column_;
            std::string v;
            while (!at_end() && (std::isdigit(static_cast<unsigned char>(source_[pos_])) || source_[pos_] == '.'))
                v += source_[pos_], advance();
            tokens_.push_back(Token{T::Number, v, sl, sc});
            continue;
        }
        switch (cur) {
            case '{': tokens_.push_back(Token{T::Lbrace, "{", line_, column_}); advance(); break;
            case '}': tokens_.push_back(Token{T::Rbrace, "}", line_, column_}); advance(); break;
            case ':': tokens_.push_back(Token{T::Colon, ":", line_, column_}); advance(); break;
            case ',': tokens_.push_back(Token{T::Comma, ",", line_, column_}); advance(); break;
            case '[': tokens_.push_back(Token{T::Lbracket, "[", line_, column_}); advance(); break;
            case ']': tokens_.push_back(Token{T::Rbracket, "]", line_, column_}); advance(); break;
            case ';': tokens_.push_back(Token{T::Semicolon, ";", line_, column_}); advance(); break;
            case '\n': tokens_.push_back(Token{T::Newline, "\n", line_, column_}); advance(); break;
            case '-': if (peek() == '>') { tokens_.push_back(Token{T::Arrow, "->", line_, column_}); advance(); advance(); } else advance(); break;
            default: advance(); break;
        }
    }
    tokens_.push_back(Token{T::EndOfFile, "", line_, column_});
    return lamia::Result<std::vector<Token>, lamia::Error>::ok(std::move(tokens_));
}

inline Parser::Parser(std::vector<Token> tokens) : tokens_(std::move(tokens)) {}
inline Token const& Parser::current() const { return tokens_[pos_]; }
inline Token Parser::advance() { return at_end() ? tokens_.back() : tokens_[pos_++]; }
inline bool Parser::at_end() const { return pos_ >= tokens_.size() || tokens_[pos_].type == Token::Type::EndOfFile; }

inline lamia::Result<std::shared_ptr<ASTNode>, lamia::Error> Parser::parse() {
    auto root = std::make_shared<ASTNode>(NodeType::Manifest, "program");
    auto skip_nl = [this]() { while (!at_end() && tokens_[pos_].type == Token::Type::Newline) pos_++; };
    while (!at_end()) {
        skip_nl();
        if (at_end()) break;
        if (current().type == Token::Type::Manifest) {
            advance();
            auto node = std::make_shared<ASTNode>(NodeType::Manifest, "");
            if (current().type == Token::Type::Identifier) node->name = current().value, advance();
            if (current().type == Token::Type::Lbrace) {
                advance();
                while (!at_end() && current().type != Token::Type::Rbrace) {
                    skip_nl();
                    if (current().type == Token::Type::Rbrace) break;
                    if (current().type == Token::Type::Identifier) {
                        std::string k = current().value; advance();
                        if (current().type == Token::Type::Colon) advance();
                        if (!at_end()) node->attributes.emplace_back(k, current().value), advance();
                    } else advance();
                    if (current().type == Token::Type::Comma) advance();
                }
                if (current().type == Token::Type::Rbrace) advance();
            }
            root->children.push_back(node);
            continue;
        }
        if (current().type == Token::Type::Create) {
            advance();
            auto node = std::make_shared<ASTNode>(NodeType::Create, "");
            if (current().type == Token::Type::Identifier) {
                std::string w = current().value;
                node->attributes.emplace_back("widget_type", w);
                if (w == "RADIANT_HEADING") node->type = NodeType::RadiantHeading;
                else if (w == "RADIANT_TEXT") node->type = NodeType::RadiantText;
                else if (w == "RADIANT_BUTTON") node->type = NodeType::RadiantButton;
                else if (w == "CONSTELLATION_LIST") node->type = NodeType::ConstellationList;
                else if (w == "RADIANT_QUOTE") node->type = NodeType::RadiantQuote;
                else if (w == "GCODE_BLOCK") node->type = NodeType::GcodeBlock;
                advance();
            }
            if (current().type == Token::Type::Lbrace) {
                advance();
                while (!at_end() && current().type != Token::Type::Rbrace) {
                    skip_nl();
                    if (current().type == Token::Type::Rbrace) break;
                    if (current().type == Token::Type::Identifier) {
                        std::string k = current().value; advance();
                        if (current().type == Token::Type::Colon) advance();
                        if (!at_end()) node->attributes.emplace_back(k, current().value), advance();
                    } else advance();
                    if (current().type == Token::Type::Comma) advance();
                }
                if (current().type == Token::Type::Rbrace) advance();
            }
            root->children.push_back(node);
            continue;
        }
        if (strict_) return lamia::Result<std::shared_ptr<ASTNode>, lamia::Error>::err(lamia::Error(2, "strict: unknown statement"));
        advance();
    }
    return lamia::Result<std::shared_ptr<ASTNode>, lamia::Error>::ok(root);
}

inline std::string Transpiler::emit_node(ASTNode const& n) {
    auto attr = [&n](char const* k) {
        for (auto const& p : n.attributes) if (p.first == k) return p.second;
        return std::string{};
    };
    auto esc = [](std::string s) {
        for (std::size_t i = 0; i < s.size(); )
            if (s[i] == '<') s.replace(i, 1, "&lt;"), i += 4;
            else if (s[i] == '>') s.replace(i, 1, "&gt;"), i += 4;
            else if (s[i] == '&') s.replace(i, 1, "&amp;"), i += 5;
            else i++;
        return s;
    };
    std::ostringstream out;
    std::string sp = options_.pretty_print ? "    " : "";
    std::string nl = options_.pretty_print ? "\n" : "";
    switch (n.type) {
        case NodeType::Manifest:
        case NodeType::Startup:
            for (auto const& c : n.children) out << emit_node(*c);
            break;
        case NodeType::RadiantHeading: out << sp << "<div class=\"radiant-heading\"><h1>" << esc(attr("content")) << "</h1></div>" << nl; break;
        case NodeType::RadiantText: out << sp << "<div class=\"radiant-text\"><p>" << esc(attr("content")) << "</p></div>" << nl; break;
        case NodeType::RadiantButton: out << sp << "<div class=\"radiant-button\"><button onclick=\"" << attr("action") << "\">" << esc(attr("content")) << "</button></div>" << nl; break;
        case NodeType::RadiantQuote: out << sp << "<div class=\"radiant-quote\"><blockquote>" << esc(attr("content")) << "</blockquote></div>" << nl; break;
        case NodeType::GcodeBlock: out << sp << "<div class=\"gcode-block\"><pre>" << esc(attr("commands")) << "</pre></div>" << nl; break;
        case NodeType::ConstellationList: out << sp << "<div class=\"constellation-list\"><h3>" << esc(attr("title")) << "</h3><ul></ul></div>" << nl; break;
        default: break;
    }
    return out.str();
}
inline lamia::Result<std::string, lamia::Error> Transpiler::transpile(std::shared_ptr<ASTNode> root) {
    if (!root) return lamia::Result<std::string, lamia::Error>::err(lamia::Error(1, "null AST"));
    std::ostringstream html;
    if (options_.emit_comments) html << "<!-- Lamia 0.5.0 transpiler output -->\n";
    html << "<!DOCTYPE html>\n<html lang=\"en\">\n<head><meta charset=\"UTF-8\"><title>Lamia</title></head>\n<body>\n<div class=\"lamia-app\">\n";
    for (auto const& c : root->children) html << emit_node(*c);
    html << "</div>\n</body>\n</html>\n";
    return lamia::Result<std::string, lamia::Error>::ok(html.str());
}

inline lamia::Result<std::string, lamia::Error> Compiler::compile(std::string_view source) {
    Lexer lex(source);
    auto tr = lex.tokenize();
    if (!tr.is_ok()) return lamia::Result<std::string, lamia::Error>::err(tr.error());
    Parser pr(tr.value());
    pr.set_strict(options_.strict);
    auto ar = pr.parse();
    if (!ar.is_ok()) return lamia::Result<std::string, lamia::Error>::err(ar.error());
    ast_ = ar.value();
    Transpiler t;
    t.set_options(TranspilerOptions{options_.target, options_.pretty_print, options_.emit_comments});
    return t.transpile(ast_);
}

[[nodiscard]] inline lamia::Result<std::string, lamia::Error> compile_source(std::string_view source) {
    Compiler c;
    return c.compile(source);
}

} // namespace lamia::compiler
