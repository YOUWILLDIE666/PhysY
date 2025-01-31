#pragma once

#include <stdexcept>
#include <cmath>
#include <unordered_map>
#include <vector>
#include "lexer.hpp"

/**
 * @brief Interpreter class
 */
class Interpreter
{
public:
    Interpreter(const std::vector<Token>& tokens) : tokens(tokens), position(0) {}

    double eval()
    {
        return expression();
    }

    void assign(std::string_view varName, double value)
    {
        variables[(std::string)varName] = value;
    }

    double getVariable(std::string_view varName)
    {
        auto it = variables.find((std::string)varName);
        if (it != variables.end())
        {
            return it->second;
        }
        else
        {
            throw std::runtime_error("Unknown variable: " + (std::string)varName);
        }
    }

private:
    std::vector<Token> tokens;
    std::size_t position;
    std::unordered_map<std::string, double> variables;

    double expression()
    {
        double result = term();
        while ( position < tokens.size() && tokens[position].type == TokenType::OPERATOR &&
               ( tokens[position].value == "+" || tokens[position].value == "-" ) )
        {
            std::string op = tokens[position++].value;
            double rhs = term();
            if (op == "+") result += rhs;
            else result -= rhs;
        }

        return result;
    }

    double term()
    {
        double result = exponent();
        while ( position < tokens.size() && tokens[position].type == TokenType::OPERATOR &&
               ( tokens[position].value == "*" || tokens[position].value == "/" ) )
        {
            std::string op = tokens[position++].value;
            double rhs = exponent();
            if (op == "*")
            {
                result *= rhs;
            }
            else
            {
                if (rhs == 0) throw std::runtime_error("Division by zero");
                result /= rhs;
            }
        }

        return result;
    }

    double exponent()
    {
        double result = factor();
        while ( position < tokens.size() && tokens[position].type == TokenType::OPERATOR &&
               tokens[position].value == "^" )
        {
            ++position; // consume '^'
            double rhs = factor();
            result = std::pow(result, rhs);
        }

        return result;
    }

    double function()
    {
        if (position < tokens.size() && tokens[position].type == TokenType::FUNCTION)
        {
            std::string funcName = tokens[position++].value;
            if (tokens[position].type == TokenType::LPAREN)
            {
                ++position; // consume '('
                double arg = expression();
                if (position < tokens.size() && tokens[position].type == TokenType::RPAREN)
                {
                    ++position; // consume ')'
                }
                else
                {
                    throw std::runtime_error("Mismatched parentheses");
                }

                static const std::unordered_map<std::string, std::function<double(double)>> functions = { // PLEASE HELP ME
                    {"sin", std::sin}, {"cos", std::cos}, {"tan", std::tan},
                    {"atan", std::atan}, {"asin", std::asin}, {"acos", std::acos},
                    {"sinh", std::sinh}, {"asinh", std::asinh}, {"cosh", std::cosh},
                    {"tanh", std::tanh}, {"acosh", std::acosh}, {"log", std::log},
                    {"log10", std::log10}, {"log2", std::log2}, {"sqrt", std::sqrt},
                    {"abs", std::abs}
                };

                auto it = functions.find(funcName);
                if (it != functions.end())
                {
                    return it->second(arg);
                }
                else
                {
                    throw std::runtime_error("Unknown function: " + funcName);
                }
            }
        }

        return factor();
    }

    double factor()
    {
        if (position < tokens.size())
        {
            if (tokens[position].type == TokenType::NUMBER)
            {
                return std::stod(tokens[position++].value);
            }
            else if (tokens[position].type == TokenType::VARIABLE)
            {
                std::string varName = tokens[position++].value;
                return getVariable(varName); // get variable value
            }
            else if (tokens[position].type == TokenType::LPAREN)
            {
                ++position; // consume '('
                double result = expression();
                if (position < tokens.size() && tokens[position].type == TokenType::RPAREN)
                {
                    ++position; // consume ')'
                }
                else
                {
                    throw std::runtime_error("Mismatched parentheses");
                }
                return result;
            }
        }
        throw std::runtime_error("Unexpected token");
    }
};