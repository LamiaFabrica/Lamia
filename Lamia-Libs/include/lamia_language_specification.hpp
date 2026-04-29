/**
 * © 2025 D Hargreaves AKA Roylepython | All Rights Reserved
 * 
 * LAMIA LANGUAGE SPECIFICATION v0.3.0c
 * ====================================
 * 
 * Core language specification for the revolutionary Lamia language
 * Ground-up implementation with Yorkshire Champion Standards
 */

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

namespace MedusaServ::Language::Lamia {

    /**
     * @brief Core language tokens
     */
    enum class LamiaTokenType {
        KEYWORD,
        IDENTIFIER,
        LITERAL,
        OPERATOR,
        DELIMITER,
        COMMENT,
        WHITESPACE,
        EOF_TOKEN
    };

    /**
     * @brief Language keywords
     */
    const std::vector<std::string> LAMIA_KEYWORDS = {
        "function", "class", "interface", "enum",
        "if", "else", "while", "for", "foreach",
        "return", "break", "continue",
        "try", "catch", "finally",
        "public", "private", "protected",
        "static", "const", "var", "let",
        "import", "export", "module",
        "async", "await", "yield",
        "type", "namespace", "using"
    };

    /**
     * @brief Token structure
     */
    struct LamiaToken {
        LamiaTokenType type;
        std::string value;
        size_t line;
        size_t column;
        
        LamiaToken(LamiaTokenType t, const std::string& v, size_t l, size_t c)
            : type(t), value(v), line(l), column(c) {}
    };

    /**
     * @brief Language specification constants
     */
    namespace LamiaSpec {
        const std::string VERSION = "0.3.0c";
        const std::string NAME = "Lamia";
        const std::string AUTHOR = "D Hargreaves AKA Roylepython";
        
        const std::vector<std::string> OPERATORS = {
            "+", "-", "*", "/", "%", "**",
            "=", "+=", "-=", "*=", "/=", "%=",
            "==", "!=", "<", ">", "<=", ">=",
            "&&", "||", "!", "&", "|", "^", "~",
            "<<", ">>", "++", "--",
            "?", ":", "??"
        };
        
        const std::vector<std::string> DELIMITERS = {
            "(", ")", "[", "]", "{", "}",
            ";", ",", ".", "::", "->", "=>",
            "@", "#", "$"
        };
    }

    /**
     * @brief Language grammar rules
     */
    class LamiaGrammar {
    public:
        static bool is_keyword(const std::string& token);
        static bool is_operator(const std::string& token);
        static bool is_delimiter(const std::string& token);
        static bool is_valid_identifier(const std::string& token);
        static LamiaTokenType classify_token(const std::string& token);
    };

    /**
     * @brief Language semantics
     */
    class LamiaSemantics {
    public:
        static bool validate_syntax(const std::vector<LamiaToken>& tokens);
        static std::string generate_ast_json(const std::vector<LamiaToken>& tokens);
        static bool type_check(const std::string& ast_json);
    };

} // namespace MedusaServ::Language::Lamia