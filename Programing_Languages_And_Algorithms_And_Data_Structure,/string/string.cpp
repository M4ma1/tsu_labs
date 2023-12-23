#include <stdio.h>
#include <string.h>
#include <iostream>
using namespace std;
class String
{ 
    char *str;
    int len;
    public:

        String(int l = 0){
            str = new char[l+1];
            len = 0;
            strcpy(str, "\0");
        };

        String(const char *tmp){
            str = new char[strlen(tmp) + 1];
            strcpy(str, tmp);
            len = strlen(tmp);
        };

        String(String &tmp){
            str = new char[tmp.len + 1];
            len = tmp.len;
            strcpy(str, tmp.str);
        };

        ~String() { 
            if(str) delete []str; 
            str = NULL; 
        };
        
        int Len();
        String & operator = (const String &);
        char &operator [](int);
        bool operator == (String &);
        bool operator != (String &);
        String operator + (const String &);
        String & operator += (const String &);
        friend istream & operator >> (istream &, String &);
        friend ostream & operator << (ostream &, String &);
        int BMSearch(String &);
        String operator () (int,int);
};

int String::Len(){
    return len;
};

String &String::operator=(const String &tmp){
    if (this != &tmp){
        delete[] str;
        str = new char[tmp.len + 1];
        strcpy(str, tmp.str);
        len = tmp.len;
    };
};

char &String::operator[](int a) {
    if (a < 0 || a >= len) exit(1);
    return str[a];
};

bool String::operator==(String &tmp) {
    if (strcmp(str, tmp.str)) return false;
    return true;
};

bool String::operator!=(String &tmp) {
    if (strcmp(str, tmp.str)) return true;
    return false;
};

String String::operator+(const String &tmp) {
    String z(len + tmp.len + 1);
    z.len = len + tmp.len;
    strcpy(z.str, str);
    strcat(z.str, tmp.str);
    return z;
};

String &String::operator+=(const String &a) {
    char *t = new char[len + 1];
    strcpy(t, str);
    delete[] str;
    len = len + a.len;
    str = new char[len + 1];
    strcpy(str, t);
    strcat(str, a.str);
    delete[] t;
    return *this;
}

istream &operator>>(istream &in, String &tmp) {
    in >> tmp.str;
    tmp.len = strlen(tmp.str);
    return in;
}

ostream &operator<<(ostream &out, String &tmp) {
    cout << tmp.str;
    return out;
}

int String::BMSearch(String &tmp) {//BM-search
    int m = tmp.len;
    int n=len;//text length
    int t[256];
    fill_n(t, 256, m);
    for (int i = 0; i < m - 1; i++) {
        t[(int)tmp.str[i]] = m-1-i;
    }

    int i = m - 1, j;
    while (i < n) {
        j = m - 1;
        int k = i;
        while (j >= 0 && str[k] == tmp.str[j]) {
            k--;
            j--;
        }

        if (j < 0) {
            return k + 1; // Pattern found at index k+1 in the text
        } else {
            // i += max(1, max(j - t[(int)str[k]], 1));
            i += t[str[i]];
        }
    }
    return -1; // Pattern not found in the text
}




int main() {
    String a, b, c;

    cin>>b;
    cin>>c;

    a = b + c;

    cout<<a<<"\t"<<a.Len()<<"\n";

    cout<<a<<endl;
    a+=c;

    if(c==b) cout<<"equal\n";
    b = a;
    if(c!=b) cout<<"not equal\n";


    a = "aassad";
    b = "assa";
    cout<<"char_start_string="<<a.BMSearch(b)<<endl<<"string1="<<a<<"\nstring2="<<b<<endl;

    return 0;
}