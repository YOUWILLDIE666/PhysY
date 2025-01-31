/*
Permission is hereby granted, free of charge, to any individual or organization obtaining a copy of this software and associated documentation files (the "Software"), to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit others to do so, under the following conditions:

1. **Attribution**: The above copyright notice and this permission notice must be included in all copies or substantial portions of the Software.
gar bage can
*/

#pragma once

#include <iostream>
#include <vector>
#include <regex>
#include <unordered_map>


enum class TokenType
{
    NUMBER,
    VARIABLE,
    OPERATOR,
    FUNCTION,
    LPAREN,
    RPAREN,
    COMMA,
    IMAGINARY,
    WHITESPACE,
    UNKNOWN
};


struct Token
{
    TokenType type;
    std::string value;
};

/**
 * @brief Lexer class for uhh managing... math
 */
class Lexer
{
public:
    Lexer(std::string_view input) : input(input), position(0) {}

    std::vector<Token> tokenize()
    {
        std::vector<Token> tokens;
        while ( position < input.length() ) 
        {
            if (std::regex_match(input.substr(position), numberRegex))
            {
                tokens.push_back({TokenType::NUMBER, matchRegex(numberRegex)});
            }
            else if (std::regex_match(input.substr(position), variableRegex))
            {
                tokens.push_back({TokenType::VARIABLE, matchRegex(variableRegex)});
            }
            else if (std::regex_match(input.substr(position), operatorRegex))
            {
                tokens.push_back({TokenType::OPERATOR, matchRegex(operatorRegex)});
            }
            else if (std::regex_match(input.substr(position), functionRegex))
            {
                tokens.push_back({TokenType::FUNCTION, matchRegex(functionRegex)});
            }
            else if (std::regex_match(input.substr(position), lparenRegex))
            {
                tokens.push_back({TokenType::LPAREN, matchRegex(lparenRegex)});
            }
            else if (std::regex_match(input.substr(position), rparenRegex))
            {
                tokens.push_back({TokenType::RPAREN, matchRegex(rparenRegex)});
            }
            else if (std::regex_match(input.substr(position), commaRegex))
            {
                tokens.push_back({TokenType::COMMA, matchRegex(commaRegex)});
            }
            else if (std::regex_match(input.substr(position), imaginaryRegex))
            {
                tokens.push_back({TokenType::IMAGINARY, matchRegex(imaginaryRegex)});
            }
            else if (std::regex_match(input.substr(position), whitespaceRegex))
            {
                position += matchRegex(whitespaceRegex).length(); // Skip whitespace
                continue;
            }
            else {
                tokens.push_back({TokenType::UNKNOWN, std::string(1, input[position])});
                ++position;
            }
        }
        return tokens;
    }

private:
    std::string input;
    std::size_t position;


    // Regex patterns
    std::regex numberRegex{R"(\d+(\.\d*)?)"};
    std::regex variableRegex{R"([a-zA-Z])"};
    std::regex operatorRegex{R"([\+\-\*/^=])"};
    std::regex functionRegex{R"(sinh|sin|asin|asinh|cosh|cos|acos|acosh|tanh|tan|atan|atanh|log10|log2|log3|log|sqrt|mod|abs)"};
    std::regex lparenRegex{R"(\()"};
    std::regex rparenRegex{R"(\))"};
    std::regex commaRegex{R"(,)"};
    std::regex imaginaryRegex{R"(i)"};
    std::regex whitespaceRegex{R"(\s+)"};


    std::string matchRegex(const std::regex& regex)
    {
        std::smatch match;
        std::regex_search(input.cbegin() + position, input.cend(), match, regex);
        position += match.length();
        return match.str();
    }
};

/**
 * @brief A function to print tokens
 * 
 * @note define `_DBG` to use this
 */
void printTokens(const std::vector<Token>& tokens)
{
# ifdef _DBG

    for ( const Token& token : tokens )
    {
        std::cout << "Type: " << static_cast<int>(token.type) << ", Value: " << token.value << std::endl;
    }

#endif
}