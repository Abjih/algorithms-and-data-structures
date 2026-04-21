#pragma once

#include <string>
#include <map>
#include "stack.h"

class Parser {
private:
    int get_priority(char op) {
        switch (op) {
        case '+': case '-': return 1;
        case '*': case '/': return 2;
        case 's': case 'c': case 'l': case 'e': return 3; 
        default: return 0; 
        }
    }

    bool is_operator(char c) {
        return c == '+' || c == '-' || c == '*' || c == '/';
    }

    bool is_function(char c) {
        return c == 's' || c == 'c' || c == 'l' || c == 'e';
    }

    char get_func_char(const std::string& func_str) {
        if (func_str == "sin") return 's';
        if (func_str == "cos") return 'c';
        if (func_str == "ln") return 'l';
        if (func_str == "exp") return 'e';
        throw std::runtime_error("Неизвестная функция: " + func_str);
    }

public:
    std::string toRPN(const std::string& infix) {
        std::string rpn_string = "";
        Stack<char> op_stack(infix.length());
        int balance = 0;

        for (int i = 0; i < infix.length(); ++i) {
            char token = infix[i];

            if (isspace(token)) { 
                continue;
            }

            if (isdigit(token) || (token == '.')) { 
                std::string number_str;
                while (i < infix.length() && (isdigit(infix[i]) || infix[i] == '.')) {
                    number_str += infix[i];
                    i++;
                }
                i--; 
                rpn_string += number_str + " ";
            }
            else if (isalpha(token)) { 
                std::string func_str;
                while (i < infix.length() && isalpha(infix[i])) {
                    func_str += infix[i];
                    i++;
                }
                i--;
                char func_char = get_func_char(func_str);
                op_stack.push(func_char);
            }
            else if (token == '(') {
                op_stack.push(token);
                balance++;
            }
            else if (token == ')') {
                balance--;
                if (balance < 0) throw std::runtime_error("Ошибка: нарушен баланс скобок.");

                while (!op_stack.is_empty() && op_stack.top() != '(') {
                    rpn_string += op_stack.pop();
                    rpn_string += " ";
                }
                if (op_stack.is_empty()) throw std::runtime_error("Ошибка: пропущена открывающая скобка.");
                op_stack.pop(); 

                if (!op_stack.is_empty() && is_function(op_stack.top())) {
                    rpn_string += op_stack.pop();
                    rpn_string += " ";
                }

            }
            else if (is_operator(token)) { 
                while (!op_stack.is_empty() && get_priority(op_stack.top()) >= get_priority(token)) {
                    rpn_string += op_stack.pop();
                    rpn_string += " ";
                }
                op_stack.push(token);
            }
            else {
                throw std::runtime_error("Ошибка: недопустимый символ в выражении.");
            }
        }

        if (balance != 0) throw std::runtime_error("Ошибка: нарушен баланс скобок.");

        while (!op_stack.is_empty()) {
            if (op_stack.top() == '(') throw std::runtime_error("Ошибка: пропущена закрывающая скобка.");
            rpn_string += op_stack.pop();
            rpn_string += " ";
        }

        return rpn_string;
    }
};