#include <iostream>
#include <string>
#include <limits> // Для очистки буфера ввода
#include "polinoms.h" 
using namespace std;

// Функция для очистки потока ввода (чтобы getline не "проглатывал" пустую строку после cin)
void clearInputBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void printMenu() {
    cout << "\n----------------------------------------" << endl;
    cout << "Выберите действие:" << endl;
    cout << "1. Ввести новый полином (Сбросить текущий)" << endl;
    cout << "2. Прибавить полином (+)" << endl;
    cout << "3. Вычесть полином (-)" << endl;
    cout << "4. Умножить на полином (*)" << endl;
    cout << "5. Вычислить значение в точке (x, y, z)" << endl;
    cout << "6. Очистить (удалить все)" << endl;
    cout << "0. Выход" << endl;
    cout << "----------------------------------------" << endl;
    cout << "Ваш выбор > ";
}

int main() {

    Polynomial currentPoly; // Наш "аккумулятор" результата
    int choice = -1;
    string inputStr;

    cout << "=== КАЛЬКУЛЯТОР ПОЛИНОМОВ ===" << endl;
    cout << "Формат ввода: 3x^2y + 2(xz)^2 - 5" << endl;

    while (choice != 0) {
        // Шаг 1: Вывод текущего состояния
        cout << "\n========================================" << endl;
        cout << "ТЕКУЩИЙ ПОЛИНОМ: ";
        // Если список пуст, выводим 0 (хотя print() внутри полинома должен это обрабатывать,
        // но для наглядности можно и тут)
        currentPoly.print();
        cout << "========================================" << endl;

        // Шаг 2: Меню
        printMenu();
        if (!(cin >> choice)) {
            // Защита от ввода букв вместо цифр меню
            cout << "Ошибка: введите число!" << endl;
            cin.clear();
            clearInputBuffer();
            continue;
        }
        clearInputBuffer(); // Убираем Enter из буфера перед getline

        if (choice == 0) break;

        // Обработка действий
        if (choice >= 1 && choice <= 4) {
            cout << "Введите формулу полинома: ";
            getline(cin, inputStr);

            Polynomial tempPoly = stringToPolynomial(inputStr);

            switch (choice) {
            case 1: // Новый ввод (замена)
                currentPoly = tempPoly;
                cout << "-> Полином обновлен." << endl;
                break;
            case 2: // Сложение
                currentPoly = currentPoly + tempPoly;
                cout << "-> Сложение выполнено." << endl;
                break;
            case 3: // Вычитание
                currentPoly = currentPoly - tempPoly;
                cout << "-> Вычитание выполнено." << endl;
                break;
            case 4: // Умножение
                currentPoly = currentPoly * tempPoly;
                cout << "-> Умножение выполнено." << endl;
                break;
            }
        }
        else if (choice == 5) { // Вычисление в точке
            double x, y, z;
            cout << "Введите x y z (через пробел): ";
            if (cin >> x >> y >> z) {
                double res = 0;
                try {
                    for (auto m : currentPoly.list) {
                        res += m.coef * pow(x, m.x) * pow(y, m.y) * pow(z, m.z);
                    }
                }
                catch (...) {
                    cout << "Ошибка при вычислении." << endl;
                }
                cout << "-> Значение в точке (" << x << ", " << y << ", " << z << ") = " << res << endl;
            }
            else {
                cout << "Некорректный ввод координат." << endl;
                cin.clear();
                clearInputBuffer();
            }
            cout << "Нажмите Enter, чтобы продолжить...";
            cin.get(); // Пауза
        }
        else if (choice == 6) { // Очистка
            // Просто создаем новый пустой полином
            currentPoly = Polynomial();
            cout << "-> Полином очищен." << endl;
        }
        else {
            cout << "Неверный пункт меню." << endl;
        }
    }

    cout << "Программа завершена." << endl;
    return 0;
}