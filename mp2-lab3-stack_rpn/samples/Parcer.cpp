#include <iostream>
#include <string>
#include "parcer.h"
#include "calculate.h"

int main() {
    std::cout << "Введите арифметическое выражение. Функции: sin, cos, ln, exp." << std::endl;
    std::cout << "Пример: (5 + sin(1.57)) * 2 - ln(exp(3))" << std::endl;
    std::cout << "> ";

    std::string expression;
    std::getline(std::cin, expression);

    Parser parser;
    Calculator calculator;

    try {
        std::string rpn_expression = parser.toRPN(expression);
        std::cout << "Обратная польская запись (ОПЗ): " << rpn_expression << std::endl;

        double result = calculator.evaluate(rpn_expression);
        std::cout << "Результат вычисления: " << result << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Произошла ошибка: " << e.what() << std::endl;
    }

    return 0;
}