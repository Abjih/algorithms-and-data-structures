#pragma once

#include <string>
#include <sstream> 
#include <cmath>   
#include "stack.h"

class Calculator {
public:
    double evaluate(const std::string& rpn) {
        if (rpn.empty()) {
            return 0.0;
        }

        Stack<double> calc_stack(rpn.length());
        std::stringstream ss(rpn);
        std::string token;

        while (ss >> token) {
            if (isdigit(token[0]) || (token.length() > 1 && token[0] == '.')) {
                calc_stack.push(std::stod(token));
            }
            else { 
                if (token == "+") {
                    double op2 = calc_stack.pop();
                    double op1 = calc_stack.pop();
                    calc_stack.push(op1 + op2);
                }
                else if (token == "-") {
                    double op2 = calc_stack.pop();
                    double op1 = calc_stack.pop();
                    calc_stack.push(op1 - op2);
                }
                else if (token == "*") {
                    double op2 = calc_stack.pop();
                    double op1 = calc_stack.pop();
                    calc_stack.push(op1 * op2);
                }
                else if (token == "/") {
                    double op2 = calc_stack.pop();
                    if (op2 == 0) throw std::runtime_error("§°§ê§Ú§Ò§Ü§Ñ: §Õ§Ö§Ý§Ö§ß§Ú§Ö §ß§Ñ §ß§à§Ý§î.");
                    double op1 = calc_stack.pop();
                    calc_stack.push(op1 / op2);
                }
                else if (token == "s") { 
                    calc_stack.push(sin(calc_stack.pop()));
                }
                else if (token == "c") { 
                    calc_stack.push(cos(calc_stack.pop()));
                }
                else if (token == "l") { 
                    calc_stack.push(log(calc_stack.pop()));
                }
                else if (token == "e") { 
                    calc_stack.push(exp(calc_stack.pop()));
                }
            }
        }

        return calc_stack.pop();
    }
};