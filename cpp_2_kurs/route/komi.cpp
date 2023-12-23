#include <iostream>
using namespace std;
// Класс “Маршрут”
class route {
int *r, n; // r – массив, представляющий маршрут; n – количество городов
public:
    route(int num = 0); // создает начальную перестановку по возрастанию
    route(const route&); // конструктор копирования
    route & operator = (const route&); // операция присваивания
    ~route() { // деструктор
        if(r) delete []r; 
        r = NULL;
    }
    int routePrice(int **); // вычисляет стоимость маршрута по матрице стоимости
    bool nextRoute(); // вычисляет следующий маршрут, используя алгоритм Дейкстры
    friend ostream & operator << (ostream&, const route&); // вывод маршрута
};

route::route(int num){
    n=num;
    r=new int[num];
    for (int i = 0; i < num; i++) {
        r[i] = i + 1; //fill array with numbers
    }
}

route::route(const route &arr) {//copy route 
    n = arr.n;
    r = new int[arr.n];
    for (int i = 0; i < arr.n; i++) {
        r[i] = arr.r[i];
    }
}

route& route::operator=(const route &tmp) {
    if(this==&tmp){
        return *this;
    }
    delete []r;
    n=tmp.n;
    r=new int [tmp.n];
    for(int i=0; i<tmp.n; i++){
        r[i]=tmp.r[i];
    }
    return *this;
}

int route::routePrice(int **arr) {
    int sum=0, b;
    for(int i=0; i<n-1;i++){
        b=r[i+1];//next town
        sum+=arr[r[i]-1][b-1];//insert route price i 
    }
    b=r[0];
    sum+=arr[r[n-1]-1][b-1];// from last to first
    return sum;
}

bool route::nextRoute() {
    int i;
    int j;
    bool flag = false;
    for(i = n-2; i>-1; i--)
    {
        if(r[i]<r[i+1])
        {
            flag = true;
            break;
        }
    }//find i

    if(flag){
        for(j = n-1; j>i; j--)
        {
            if(r[i]<r[j])
            {
                break;
            }
        }//find j

        int c;
        c=r[i];
        r[i]=r[j];
        r[j]=c;


        if(i<n-2){
            int left = i+1;
            int right = n-1;
            while(left<right){
                c=r[left];
                r[left]=r[right];
                r[right]=c;
                left++;
                right--;
            }
        }//invert tail
        if(r[0]!=1){
            flag = false;// Since all paths are not equal to 1, we can disregard them as paths are cyclic
        }
        return flag;
    }
    else{
        return flag;
    }
}

ostream &operator<<(ostream &p, const route &x) {
    for (int i = 0; i < x.n; i++) {
        p << x.r[i] << " ";
    }
    return p;
}
route komi(int num){//solve komi problem
    int**matrix=new int* [num];
    for (int i=0; i<num; i++) matrix[i]=new int [num];//allocate 

    for(int i =0; i<num; i++){//fill the matrix
        for(int j =0; j<num; j++){
            cout << "enter price for: ", cout << i, cout << " and ", cout << j << endl, cin >> matrix[i][j];
        }
    }
    route r(num);//starting path ascending
    route final_r = r;//create final 
    int sum = r.routePrice(matrix);
    cout<<r<<" : "<<sum<<endl;
    while(r.nextRoute()){
        int new_sum = r.routePrice(matrix);
        cout<<r<<" : "<<new_sum<<endl;
        if(sum > new_sum && new_sum>0){
            sum = new_sum;
            final_r = r;
        }
    }

    return final_r;
}
int main(){
   int n;
   cout << "enter n:", cin >> n;
   cout << komi(n);
}