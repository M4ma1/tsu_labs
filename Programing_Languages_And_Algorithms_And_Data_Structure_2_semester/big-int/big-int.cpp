#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <cstring>
#include <algorithm> 

using namespace std;

typedef unsigned int BASE;
#define BASE_SIZE (sizeof(BASE)*8)
typedef unsigned long long DBASE;
#define DBASE_SIZE (sizeof(DBASE)*8)


int findRandom()
{
    int num = ((int)rand() % 2);
    return num;
}
int generateBinaryString(int N)
{
    unsigned int S = 0;
    for (int i = 0; i < N; i++) {
        int x = findRandom();
        S = S << 1;
        S = S | x;
    }
    return S;
}

class BigInt{

    int n;
    int maxlen;
    BASE *coef;

    void check(BASE *array, int max){
        n=1;
        for (int i=max-1; i>-1; i--){
            if (array[i] != 0){
                n = i+1;
                break;
            }
        }
    }

    public:
        BigInt(int maxLen = 1, bool generate = 0){
            maxlen = maxLen;
            coef = new BASE[maxlen];
            if (generate){
                for (int i=0; i<maxlen; i++){
                    coef[i] = generateBinaryString(BASE_SIZE);
                }
                check(coef, maxlen);
            }
            else{
                for (int i=0; i<maxlen; i++){
                    coef[i] = 0;
                }
                check(coef, maxlen);
            }

        }

        BigInt(const BigInt& obj){
            n = obj.n;
            maxlen = obj.maxlen;
            coef = new BASE[maxlen];
            for (int i=0; i<maxlen; i++){
                coef[i] = obj.coef[i];
            }
        }

        ~BigInt(){
            if (coef) delete []coef;
            coef = NULL;
            maxlen = 0;
        }
        
        friend ostream &operator << (ostream& out, const BigInt &res){
            out << "Maxlen:" << res.maxlen << "\n";
            out << "len:" << res.n << "\n";
            // Initialize the shift value to the number of bits in BASE minus 4
            int k = BASE_SIZE - 4;
            // Loop through each coefficient of the BigInt starting from the most significant
            for (int j = res.n-1; j > -1;){
                // Extract 4 bits from the current coefficient
                int tmp = (res.coef[j]>>k) & (0xf);
                // If the extracted bits represent a value between 0 and 9, print the corresponding digit
                if (tmp<= 9 && tmp>=0){
                    out << (char)(tmp + '0');
                }
                // If the extracted bits represent a value between 10 and 15, print the corresponding hexadecimal character
                if (tmp >= 10 && tmp <= 15){
                    out << (char)(tmp - 10 + 'a');
                }
                // Decrement the shift value by 4 to move to the next set of 4 bits
                k -= 4;
                // If the shift value is less than 0, reset it and move to the next coefficient
                if (k<0){
                    k = BASE_SIZE - 4;
                    j--;
                    out << " ";
                }
            }
            return out;
        }

        friend istream &operator >> (istream& in, BigInt &res){
            string s;
            int tmp=0, j=0, k=0;
            cout << "String: ";
            in >> s;
            res.maxlen = res.n = (s.size()-1)/(BASE_SIZE/4)+1;
            BASE *coef;
            coef = new BASE[res.n]();
            for (int i = s.size()-1; i > -1; i--){
                if(s[i]>='0' && s[i]<='9'){
                    tmp = s[i] - '0';
                }
                else if(s[i]>='a' && s[i]<='f'){
                    tmp = s[i] - 'a' + 10;
                }
                else if(s[i]>='A' && s[i]<='F'){
                    tmp = s[i] - 'A' + 10;
                }
                else{
                    cout << "Something went wrong";
                    exit(1);
                }
                coef[j] |= tmp << (k*4);
                k++;
                if(k>=BASE_SIZE/4){
                    k=0;
                    j++;
                }
            }
            delete[]res.coef;
            res.coef = coef;
            while (res.n > 1 && res.coef[res.n - 1] == 0) {
                res.n--;
            }
            return in;
        };

        BigInt &operator= (const BigInt&);
        int cmp (const BigInt&);
        bool operator== (const BigInt&);
        bool operator> (const BigInt&);
        bool operator< (const BigInt&);
        bool operator!= (const BigInt&);
        bool operator<= (const BigInt&);
        bool operator>= (const BigInt&);
        BigInt operator+ (const BigInt&);
        BigInt operator- (const BigInt&);
        BigInt operator* (const BigInt&);
        BigInt &operator*= (const BigInt&);
        BigInt operator* (BASE);
        BigInt &operator+= (const BigInt&);
        BigInt &operator-= (const BigInt&);
        BigInt operator/ (BASE);
        BASE operator% (BASE);
        BigInt operator/ (const BigInt&);
        BigInt operator% (const BigInt&);
        BigInt &input (int c=10);
        void output (int c=10); 
};

// BigInt BigInt::operator% (const BigInt& obj){
    
// }

// BigInt BigInt::operator/ (const BigInt& obj){
//     BASE d = (1 << BASE_SIZE)/(obj.coef[n-1]+1);
    
// }

BigInt &BigInt::input (int base){
    string str; cout << "enter string: ", cin >> str;
    BigInt tmp(str.size(), 0), tmp1(1, 0);
    for(int i=0; i<str.size(); i++){
        tmp1.coef[0] = str[i] - '0';
        if(str[i]>57 || str[i]<48)return *this;
        tmp = tmp * base + tmp1;
    }
    *this = tmp;
    return *this;
}

void BigInt::output(int base){
    char chr;
    string str;
    BigInt tmp = *this;
    while(!(tmp.n==1 && tmp.coef[0]==0)){
        chr = tmp % base;
        str += chr+'0';
        tmp = tmp / base;
    }
    reverse(str.begin(), str.end());
    cout << "string: " << str << endl;
}

BASE BigInt::operator% (BASE devisor){
    DBASE tmp;
    BASE ratio;
    for(int i=n-1; i>-1; i--){
        tmp = (DBASE)((DBASE)((DBASE)ratio<<(DBASE)BASE_SIZE) + (DBASE)coef[i]);
        ratio = (BASE)(tmp % (DBASE)devisor);
    }
    return ratio;
}

BigInt BigInt::operator* (BASE multiplier){
    DBASE tmp, carry=0;
    BigInt res(n, 0);
    for(int i=0; i<n; i++){
        tmp = (DBASE)coef[i] * (DBASE)multiplier + (DBASE)carry;
        res.coef[i] = (BASE)tmp;
        carry = (BASE)(tmp>>BASE_SIZE);
    }
    if(carry > 0){
        res.coef[n] = carry;
        res.n = n+1;
    }
    else{
        res.n = n;
    }
    return res;
}

BigInt BigInt::operator/ (BASE devisor){
    DBASE tmp;
    BASE ratio=0;
    BigInt res(n, 0);
    res.n = n;
    for(int i=n-1; i>-1; i--){
        tmp = (DBASE)((DBASE)((DBASE)ratio<<(DBASE)BASE_SIZE) + (DBASE)coef[i]);
        res.coef[i] = (BASE)((DBASE)tmp / (DBASE)devisor);
        ratio = (BASE)((DBASE)tmp % (DBASE)devisor);
    }
    while (res.n > 1 && res.coef[res.n - 1] == 0) {
        res.n--;
    }
    return res;
}

BigInt &BigInt::operator+= (const BigInt& obj){
    *this = *this + obj;
    return *this;
}

BigInt &BigInt::operator-= (const BigInt& obj){
    *this = *this - obj;
        return *this;
}

BigInt BigInt::operator* (const BigInt& obj){
    BigInt res(n+obj.n, 0);
    res.n = n+obj.n;
    DBASE carry=0, tmp;
    for(int i=0; i<n; i++){
        carry = 0;
        for(int j=0; j<obj.n; j++){
            tmp = (DBASE)coef[i] * (DBASE)obj.coef[j] + (DBASE)res.coef[i+j] + (DBASE)carry;
            res.coef[i+j] = (BASE)tmp;
            carry = (BASE)(tmp>>BASE_SIZE);
        }
        res.coef[i+obj.n] += carry;
    }
    while (res.n > 1 && res.coef[res.n - 1] == 0) {
        res.n--;
    }
    return res;
}

BigInt &BigInt::operator*= (const BigInt& obj){
    *this = *this * obj;
    return *this;
}

bool BigInt::operator!= (const BigInt& obj){
    return cmp(obj);
}

bool BigInt::operator<= (const BigInt& obj){
    int result = cmp(obj);
    return result == 0 || result == -1;
}

bool BigInt::operator>= (const BigInt& obj){
    int result = cmp(obj);
    return result == 0 || result == 1;
}

BigInt BigInt::operator- (const BigInt& obj){
    DBASE tmp;
    int borrow=0;
    BigInt res(n, 0);
    if(*this<obj)return -1;
    int i=0;
    for(;i<obj.n; i++){
        tmp = ((DBASE)1<<BASE_SIZE) | coef[i];
        tmp = tmp - obj.coef[i] - borrow;
        res.coef[i] = (BASE)tmp;
        borrow = !(tmp>>BASE_SIZE);
    }

    for(;i<n; i++){
        tmp = ((DBASE)1<<BASE_SIZE) | coef[i];
        tmp -= borrow;
        res.coef[i] = (BASE)tmp;
        borrow = !(tmp>>BASE_SIZE);
    }

    res.n = n;
    while (res.n > 1 && res.coef[res.n - 1] == 0) {
        res.n--;
    }
    return res;
}

BigInt BigInt::operator+ (const BigInt& obj){
    BASE carry=0;
    DBASE tmp;
    int max = std::max(n, obj.n);
    int min = std::min(n, obj.n);
    BigInt res(max+1, 0);
    int i=0;
    for(;i<min; i++){
        tmp = (DBASE)obj.coef[i] + (DBASE)coef[i] + (DBASE)carry;
        res.coef[i] = (BASE) tmp;
        carry = (BASE)(tmp >> BASE_SIZE);
    }
    for(;i<n; i++){
        tmp = (DBASE)coef[i] + (DBASE)carry;
        res.coef[i] = (BASE) tmp;
        carry = (BASE)(tmp >> BASE_SIZE);
    }
    
    for(;i<obj.n; i++){
        tmp = (DBASE)obj.coef[i] + (DBASE)carry;
        res.coef[i] = (BASE) tmp;
        carry = (BASE)(tmp >> BASE_SIZE);
    }
    
    if(carry > 0){
        res.coef[max] = carry;
        res.n = max+1;
    }
    else{
        res.n = max;
    }
    
    return res;
}

int BigInt::cmp (const BigInt& obj){
    if(n < obj.n){
        return -1;
    }
    if(n > obj.n){
        return 1;
    }
    for (int i=n; i>-1; i--){
        if(coef[i] < obj.coef[i]){
            return -1;
        }
        if(coef[i] > obj.coef[i]){
            return 1;
        }
    }
    return 0;
}

bool BigInt::operator== (const BigInt& obj){
    if(!cmp(obj)){
        return 1;
    }
    return 0;
}

bool BigInt::operator< (const BigInt& obj){
    if(cmp(obj) == -1){
        return 1;
    }
    return 0;
}

bool BigInt::operator> (const BigInt& obj){
    if(cmp(obj) == 1){
        return 1;
    }
    return 0;
}

BigInt &BigInt::operator= (const BigInt& obj){
    if (this == &obj) return *this;
    delete[]coef;
    // BASE *coef;
    coef = new BASE[obj.maxlen];
    maxlen = obj.maxlen;
    n = obj.n;
    for (int i=0; i<maxlen; i++){
        coef[i] = obj.coef[i];
    }
    return *this;
}

int main(){
    srand(time(NULL));
    BigInt zxc(4, 1);
    BigInt poi(4, 1);
    int n,m;

    // cout << "enter number len: ", cin >> n;
    // BigInt asd(n, 1);
    // cout << asd << endl;
    // cout << "enter number len: ", cin >> m;
    // BigInt qwe(m, 1);
    // cout << qwe << endl;
    // cout << "equality comparison: " << (qwe == asd) << endl;
    // cout << "greater than comparison: " << (asd > qwe) << endl;
    // cout << "less than comparison: " << (asd < qwe) << endl;
    // zxc = qwe + asd;
    // cout << "addition assignment:\n" <<  zxc << endl;
    // poi = zxc - qwe;
    // cout << "subtraction result:\n" << poi << endl;
    // zxc = qwe * asd;
    // cout << "multiply result:\n" << zxc << endl;
    // int multiplier; cout << "enter multiplier: ",  cin >> multiplier;
    // BigInt rty = asd * multiplier;
    // cout << "multiplication\n" << asd << "by number: " << multiplier << " equals:\n" << rty << endl;
    
    BigInt rty(4,1);

    rty = rty.input(10);
    // cout << rty;
    // int devisor; cout << "enter devisor: ",  cin >> devisor;
    // cout << "devision\n" << rty << "by number: " << devisor << " equals:\n" << (rty / devisor)<< endl;
    
    // cout << "devision\n" << rty << "by number: " << devisor << " equals:\n" << (rty % devisor)<< endl;
    rty.output(10);
    return 0;
}