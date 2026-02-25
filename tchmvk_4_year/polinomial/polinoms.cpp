#include "polinomial.h"
#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

/*
Тест неприводимости №1 
Основан на проверке условия: полином f(x) неприводим над Zp тогда и только тогда,
когда НОД(f(x), x^(p^i) - x) = 1 для всех i = 1, 2, ..., floor(n/2)
где n = deg(f(x))
*/
bool Polynomial::irreducibilitypol1() const {
    Polynomial f = *this;
    Polynomial one({1}, f.mod);  // Полином 1
    Polynomial x({0, 1}, f.mod); // Полином x
    int deg = f.degree();        // Степень полинома

    Polynomial u = x; // Начинаем с u(x) = x

    // Шаг 2: для i от 1 до floor(n/2)
    for (int i = 0; i < int(deg / 2); i++) {
        // Вычисляем u(x) = u(x)^p mod f(x)
        // Это эффективное вычисление x^(p^i) mod f(x)
        u = (u.pow(f.mod)) % f;

        // Вычисляем x^(p^i) - x
        Polynomial tmp = u - x;
        
        // Находим НОД(f(x), x^(p^i) - x)
        Polynomial d = f.gcd(tmp);

        // Если НОД не равен 1, полином приводим
        if (d != one)
            return true; // Полином приводим
    }
    return false; // Полином неприводим
}

/*
Тест неприводимости №2 
Более эффективный тест, основанный на разложении степени полинома
*/
bool Polynomial::irreducibilitypol2() const {
    Polynomial f = *this;
    Polynomial one({1}, f.mod);  // Полином 1
    Polynomial x({0, 1}, f.mod); // Полином x

    // Шаг 1: Проверка условия x^(p^n) ≡ x (mod f(x))
    // где n = deg(f(x))
    int deg = std::pow(f.mod, f.degree()); // p^n
    Polynomial u = x.pow(deg) % f;         // x^(p^n) mod f(x)

    // Если условие не выполняется, полином приводим
    if (u != x)
        return true; // Полином приводим

    // Шаг 2: Разложение степени полинома на простые множители
    vector<int> factorization = PrimeFactorization(f.degree());

    // Для каждого простого делителя q степени полинома
    for (int k = 0; k < factorization.size(); k++) {
        // Вычисляем x^(p^(n/q)) mod f(x)
        int deg_x = std::pow(f.mod, (f.degree() / factorization[k]));
        u = x.pow(deg_x) % f;

        // Вычисляем НОД(f(x), x^(p^(n/q)) - x)
        Polynomial tmp = u - x;
        Polynomial d = f.gcd(tmp);

        // Если НОД не равен 1, полином приводим
        if (d != one)
            return true; // Полином приводим
    }
    return false; // Полином неприводим
}

/*
Тест на примитивность полинома 
Полином примитивен, если:
1) Он неприводим над Zp
2) Его корни являются примитивными элементами мультипликативной группы поля GF(p^n)
*/
bool Polynomial::primitiveness_pol() const {
    Polynomial f = *this;
    
    // Шаг 1: Проверка неприводимости
    // Если полином приводим, он не может быть примитивным
    if (f.irreducibilitypol2() == true) {
        return false;
    }
    
    Polynomial one({1}, f.mod);  // Полином 1
    Polynomial x({0, 1}, f.mod); // Полином x

    int deg = f.degree();
    // Порядок мультипликативной группы расширения поля
    int p = std::pow(f.mod, deg) - 1;

    // Шаг 2: Разложение порядка группы на простые множители
    vector<int> factorization = PrimeFactorization(p);

    // Шаг 3: Для каждого простого делителя q порядка группы
    int q = factorization[0];
    for (int k = 0; k < factorization.size(); k++) {
        // Проверяем, что x^((p^n-1)/q) ≠ 1 (mod f(x))
        int deg_x = p / q;
        Polynomial r = x.pow(deg_x) % f;
        
        // Если для какого-то q выполняется равенство 1, 
        // то полином не примитивен
        if (r == one) {
            return false;
        }
    }
    
    // Все проверки пройдены - полином примитивен
    return true;
}

/*
Тестирование алгоритмов на полиномах над Z2
Содержит наборы заведомо неприводимых и приводимых полиномов
для проверки корректности работы алгоритмов
*/
void run_test1(int selection) {
    // Набор заведомо неприводимых полиномов над Z2
    vector<vector<int>> irreducibilitypol = {
            {1, 0, 0, 0, 1, 1, 1, 0, 1}, // x^8 + x^4 + x^3 + x^2 + 1
            {1, 0, 0, 1, 0, 1, 0, 1, 1}, // x^8 + x^5 + x^3 + x + 1
            {1, 0, 0, 1, 0, 1, 1, 0, 1}, // x^8 + x^5 + x^4 + x^2 + 1
            {1, 0, 0, 0, 1, 1, 0, 1, 1}, // x^8 + x^4 + x^3 + x + 1
            {1, 0, 0, 1, 1, 1, 0, 0, 1}, // x^8 + x^5 + x^4 + x^3 + 1
            {1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1}, // x^10 + x^7 + 1
            {1, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1}, // x^10 + x^6 + x^5 + x^3 + 1
            {1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1}, // x^10 + x^7 + x^6 + x^5 + 1
            {1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1}, // x^10 + x^6 + x^5 + x^4 + 1
            {1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1}  // x^10 + x^5 + x^4 + x^2 + 1
    };

    // Набор заведомо приводимых полиномов над Z2
    vector<vector<int>> reduciblepol = {
            {1, 0, 0, 1, 1, 1, 0, 1, 1}, // x^8 + x^5 + x^4 + x^3 + x + 1
            {1, 0, 0, 0, 1, 1, 1, 1, 1}, // x^8 + x^4 + x^3 + x^2 + x + 1
            {1, 0, 0, 0, 1, 0, 0, 1, 1}, // x^8 + x^4 + x + 1
            {1, 0, 0, 1, 1, 1, 1, 0, 1}, // x^8 + x^5 + x^4 + x^3 + x^2 + 1
            {1, 0, 0, 1, 0, 0, 0, 1, 1}, // x^8 + x^5 + x + 1
            {1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1}, // x^10 + x^7 + x^3 + 1
            {1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1}, // x^10 + x^7 + x^6 + 1
            {1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1}, // x^10 + x^5 + x^4 + x^3 + x^2 + 1
            {1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1}, // x^10 + x^5 + x^3 + x^2 + x + 1
            {1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1}  // x^10 + x^5 + x^4 + x^2 + x + 1
    };

    int p = 2; // Поле Z2

    cout << "Тест неприводимых полиномов над Z2:\n ";
    for (const auto &poly: irreducibilitypol) {
        Polynomial test_poly(poly, p);
        if (selection == 1) {
            if (test_poly.irreducibilitypol1()) {
                cout << "Полином приводим (ошибка)\n";
            } else {
                cout << "Полином неприводим\n";
            }
        } else if (selection == 2) {
            if (test_poly.irreducibilitypol2()) {
                cout << "Полином приводим (ошибка)\n";
            } else {
                cout << "Полином неприводим\n";
            }
        }
    }

    cout << "Тест приводимых полиномов над Z2:\n ";
    for (const auto &poly: reduciblepol) {
        Polynomial test_poly(poly, p);
        if (selection == 1) {
            if (test_poly.irreducibilitypol1()) {
                cout << "Полином приводим\n";
            } else {
                cout << "Полином неприводим (ошибка)\n";
            }
        } else if (selection == 2) {
            if (test_poly.irreducibilitypol2()) {
                cout << "Полином приводим\n";
            } else {
                cout << "Полином неприводим (ошибка)\n";
            }
        }
    }
}

// Аналогичные тесты для полей Z3 и Z5 (структура аналогична тесту для Z2)
void run_test2(int selection) {
    vector<vector<int>> irreducibilitypol = {
            {1, 0, 0, 0, 2, 1},
            {1, 0, 0, 2, 1, 1},
            {1, 0, 0, 0, 2, 2},
            {1, 0, 0, 1, 1, 2},
            {1, 0, 2, 1, 2, 2},
            {1, 0, 0, 0, 0, 1, 2},
            {1, 0, 0, 0, 0, 2, 2},
            {1, 0, 0, 0, 1, 1, 1},
            {1, 0, 0, 0, 1, 2, 1},
            {1, 0, 0, 0, 2, 0, 1}
    };

    vector<vector<int>> reduciblepol = {
            {1, 0, 0, 0, 1, 2},
            {1, 0, 0, 2, 1, 2},
            {1, 0, 0, 2, 2, 1},
            {1, 0, 1, 2, 0, 2},
            {1, 0, 2, 1, 0, 2},
            {1, 0, 0, 0, 0, 1, 1},
            {1, 0, 0, 0, 1, 2, 2},
            {1, 0, 0, 0, 1, 1, 2},
            {1, 0, 0, 0, 2, 1, 1},
            {1, 0, 0, 0, 2, 1, 2}
    };

    int p = 3;

    cout << "Тест неприводимых полиномов над Z3:\n ";
    for (const auto &poly: irreducibilitypol) {
        Polynomial test_poly(poly, p);
        if (selection == 1) {
            if (test_poly.irreducibilitypol1()) {
                cout << "Полином приводим (ошибка)\n";
            } else {
                cout << "Полином неприводим\n";
            }
        } else if (selection == 2) {
            if (test_poly.irreducibilitypol2()) {
                cout << "Полином приводим (ошибка)\n";
            } else {
                cout << "Полином неприводим\n";
            }
        }
    }

    cout << "Тест приводимых полиномов над Z3:\n ";
    for (const auto &poly: reduciblepol) {
        Polynomial test_poly(poly, p);
        if (selection == 1) {
            if (test_poly.irreducibilitypol1()) {
                cout << "Полином приводим\n";
            } else {
                cout << "Полином неприводим (ошибка)\n";
            }
        } else if (selection == 2) {
            if (test_poly.irreducibilitypol2()) {
                cout << "Полином приводим\n";
            } else {
                cout << "Полином неприводим (ошибка)\n";
            }
        }
    }
}

void run_test3(int selection) {
    vector<vector<int>> irreducibilitypol = {
            {1, 0, 1, 2, 2},
            {1, 0, 1, 3, 2},
            {1, 0, 0, 0, 2},
            {1, 0, 0, 1, 4},
            {1, 0, 1, 0, 2},
            {1, 0, 0, 0, 4, 2},
            {1, 0, 0, 0, 4, 3},
            {1, 0, 0, 0, 4, 1},
            {1, 0, 0, 0, 4, 4},
            {1, 0, 0, 1, 2, 4}
    };

    vector<vector<int>> reduciblepol = {
            {1, 0, 0, 0, 4},
            {1, 0, 0, 1, 2},
            {1, 0, 0, 1, 3},
            {1, 0, 1, 1, 2},
            {1, 0, 1, 2, 4},
            {1, 0, 0, 0, 3, 1},
            {1, 0, 0, 0, 3, 4},
            {1, 0, 0, 0, 2, 1},
            {1, 0, 0, 1, 1, 2},
            {1, 0, 0, 1, 2, 3}
    };

    int p = 5;

    cout << "Тест неприводимых полиномов над Z5:\n ";
    for (const auto &poly: irreducibilitypol) {
        Polynomial test_poly(poly, p);
        if (selection == 1) {
            if (test_poly.irreducibilitypol1()) {
                cout << "Полином приводим (ошибка)\n";
            } else {
                cout << "Полином неприводим\n";
            }
        } else if (selection == 2) {
            if (test_poly.irreducibilitypol2()) {
                cout << "Полином приводим (ошибка)\n";
            } else {
                cout << "Полином неприводим\n";
            }
        }
    }

    cout << "Тест приводимых полиномов над Z5:\n ";
    for (const auto &poly: reduciblepol) {
        Polynomial test_poly(poly, p);
        if (selection == 1) {
            if (test_poly.irreducibilitypol1()) {
                cout << "Полином приводим\n";
            } else {
                cout << "Полином неприводим (ошибка)\n";
            }
        } else if (selection == 2) {
            if (test_poly.irreducibilitypol2()) {
                cout << "Полином приводим\n";
            } else {
                cout << "Полином неприводим (ошибка)\n";
            }
        }
    }
}

/*
Тест примитивности полинома
Демонстрация работы алгоритма проверки примитивности
*/
void test_primitiveness_pol() {
    // Тестируемый полином: x^5 + 2x + 2 над Z3
    Polynomial test({2, 2, 0, 0, 0, 1}, 3);

    cout << "Тестируемый полином: ";
    test.print();
    if (test.primitiveness_pol() == true) {
        cout << "Результат: полином примитивен" << endl;
    } else {
        cout << "Результат: полином не примитивен" << endl;
    }
}

int main() {
    setlocale(LC_ALL, "Russian");

    short int selection = 0;
    cout << "Введите число\n1 - тест 1 на неприводимость полиномов\n2 - тест 2 на неприводимость полиномов\n";
    cin >> selection;

    if (selection > 0 && selection <= 2) {
         run_test1(selection);
         cout << endl;
         run_test2(selection);
         cout << endl;
         run_test3(selection);
         cout << endl;
        test_primitiveness_pol();
        return 0;
    }
}
