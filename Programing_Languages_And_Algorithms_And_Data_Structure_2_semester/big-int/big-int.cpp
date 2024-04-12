#include <stdlib.h>
#include <time.h>
#include <iostream>

using namespace std;

typedef unsigned char BASE;
#define BASE_SIZE (sizeof(BASE)*8)

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
    int *coef;

    void check(int *array, int maxlen){
        n=1;
        for (int i=maxlen-1; i>-1; i--){
            if (array[i] != 0){
                n = i+1;
                break;
            }
        }
    }

    public:
        BigInt(int maxLen = 1, bool generate = 0){
            maxlen = maxLen;
            coef = new int[maxlen];
            if (generate){
                for (int i=0; i<maxlen; i++){
                    coef[i] = generateBinaryString(BASE_SIZE);
                    cout << coef[i] << " ";
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
            coef = new int[maxlen];
            for (int i=0; i<maxlen; i++){
                coef[i] = obj.coef[i];
            }
        }

        ~BigInt(){
            if (coef) delete []coef;
            coef = NULL;
            maxlen = 0;
        }

        friend ostream& operator << (ostream& out, const BigInt &res){
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
                    cout << (char)(tmp + '0');
                }
                // If the extracted bits represent a value between 10 and 15, print the corresponding hexadecimal character
                if (tmp >= 10 && tmp <= 15){
                    cout << (char)(tmp - 10 + 'a');
                }
                // Decrement the shift value by 4 to move to the next set of 4 bits
                k -= 4;
                // If the shift value is less than 0, reset it and move to the next coefficient
                if (k<0){
                    k = BASE_SIZE - 4;
                    j--;
                    cout << " ";
                }
            }
            return out;
        }

        BigInt &operator= (const BigInt& );
};

BigInt &BigInt::operator=(const BigInt& obj){
    if (this == &obj) return *this;
    delete[]coef;
    coef = new int[obj.maxlen];
    maxlen = obj.maxlen;
    n = obj.n;
    for (int i=0; i<maxlen; i++){
        coef[i] = obj.coef[i];
    }
    return *this;
}

int main(){
    srand(time(NULL));
    BigInt asd(4, 1);
    // BigInt qwe(4, 1);
    // BigInt zxc(4, 1);
    // BigInt poi(4, 1);
    // qwe = poi;
    cout << "\n" << asd;
    // cout << "\n" << qwe;
    // cout << "\n" << zxc;
    // cout << "\n" << poi;
    return 0;
}