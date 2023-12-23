#include <iostream>
#include <cmath>
using namespace std;

class complex {
    double re, im;
    public:
        complex(double Re = 0, double Im = 0);

        double getRe(){
            return re;
        };

        void setRe(double);

        double getIm(){
            return im;
        };

        void setIm(double);

        double mod(){
            return sqrt(re*re + im*im);
        }; // возвращает модуль комплексного числа

        friend istream& operator >> (istream&st, complex &res){
            st >> res.re;
            st >> res.im;
            return st;
        };

        friend ostream& operator << (ostream&st, const complex &res){
            if (res.im>0)
                st << res.re << "+" << res.im << "*i";
            else if (res.im<0)
                st << res.re << "-" << -1 * res.im << "*i";
            else
                st << res.re;
            return st;
        };

        bool operator == (complex);
        bool operator != (complex);

        operator char *(){
            char* buffer = new char[100];  
            snprintf(buffer, 100, "%.5f + %.5f*i", re, im);
            return buffer;
        }; // преобразует комплексное число в строку
        complex operator - () const {
            complex num;
            num.re = re;
            num.im = -im;
            return num;
        };// возвращает комплексно сопряженное число
        complex operator + (const complex &);
        complex operator += (const complex &);
        complex operator - (const complex &);
        complex operator -= (const complex &);
        complex operator * (const complex &);
        complex operator *= (const complex &);
        complex operator / (const complex &);
        complex operator /= (const complex &);
};

complex::complex(double Re, double Im){
    re = Re;
    im = Im;
};

void complex::setRe(double Re){
    re = Re;
};

void complex::setIm(double Im){
    im = Im;
};

bool complex::operator == (complex num){
    if (re == num.re && im == num.im)
        return 1;
    else
        return 0;
};

bool complex::operator != (complex num){
    if (re != num.re || im != num.im)
        return 1;
    else
        return 0;
};

complex complex::operator+(const complex &num){
    complex res;
    res.re = re + num.re;
    res.im = im + num.im;
    return res;
};

complex complex::operator+=(const complex &num){
    *this = *this + num;
    return *this;
};

complex complex::operator-(const complex &num){
    complex res;
    res.re = re - num.re;
    res.im  = im - num.im;
    return res;
};

complex complex::operator-=(const complex &num){
    *this = *this - num;
    return *this;
};

complex complex::operator*(const complex &num){
    complex res;
    res.re = re * num.re - im * num.im;
    res.im = re * num.im + im * num.re;
    return res;
};

complex complex::operator*=(const complex &num){
    *this = *this * num; 
    return *this;
};

complex complex::operator/(const complex &num){
    complex conjugate = -num;
    double numSquare = num.re * num.re + num.im * num.im;
    complex res = (*this) * conjugate;
    res.re /= numSquare;
    res.im /= numSquare;
    return res;
};

complex complex::operator/=(const complex &num){
    *this = *this * -num;
    double numSquare = num.re * num.re + num.im * num.im;
    re /= numSquare;
    im /= numSquare;
    return *this;
};

int main(){
    cout << "hello world!" << "\n";
    complex fi, sec(-7,6);
    cout << "Enter first \n";
    cin >> fi;
    cout << "First:"<< fi << "\nSecond:" << sec;
    if (fi == sec)
        cout << "\nThey are equal";
    if (fi != sec)
        cout << "\nThey are not equal";
    cout << "\nFirst's re change:" << fi.getRe()<< "\nSecond's im change:" << sec.getIm();
    cout << "\nFirst module:"<< fi.mod()<< "\nSecond module:"<< sec.mod();
    cout << "\nFirst + second:" << fi + sec;
    cout << "\nFirst - second:" << fi - sec;
    cout << "\nFirst * second:" << fi * sec;
    cout << "\nFirst / second:" << fi / sec;
    fi /= sec;
    cout << "\nFirst after /=:" << fi << "\n";
    char* str = fi;
    cout << str;

};