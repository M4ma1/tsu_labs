#include <stdio.h>
#include <iostream>
#include <chrono>

using namespace std;

class Array
{
    int n, *arr;
    public:
        Array(int len=1, int flag=1, int lim=10)
        {
            arr = new int[len];
            n = len;
            if (flag==1) // non-ordered array
            {
                for (int i=0; i<n; i++)
                {
                    arr[i] = rand() % lim;
                }
            }

            else if (flag==2) // non-decreasing
            {
                arr[0] = rand() % lim;
                for (int i=1; i<n; i++)
                {
                    arr[i] = arr[i-1] + rand() % lim;
                }
            }

            else if (flag == 3) // non-increasing 
            {
                arr[n-1] = rand() % lim;
                for (int i=n-2; i>-1; i--)
                {
                    arr[i] = arr[i+1] + rand() % lim;
                }
            }

            else exit(-1);
        }

        Array(int *po, int len)
        {
            n=len;
            arr = new int[len];
            for (int i=0; i<len; i++)
            {
                arr[i] = po[i];
            }
        }

        Array(Array &tmp)
        {
            arr = new int[tmp.n];
            n = tmp.n;
            for (int i=0; i<n; i++)
            {
                arr[i] = tmp.arr[i];
            }
        }

        ~Array()
        {
            if (arr) delete[]arr;
            arr = NULL;
        }

        Array &operator=(const Array &);

        int &operator[](int); // get i element

        bool Test(); // check if non-decreasing 

        bool operator==(Array);
        
        void Shell_sort();

        void Heapsort();

        void heapify(int, int);

        void Hoare_sort();

        void Bit_sort(int, int, int, bool);

        int major_bit(); // for Bit_sort

        friend istream &operator>>(istream &, Array &);

        friend ostream &operator<<(ostream &, Array &);
};

Array &Array::operator=(const Array &tmp)
{
    if (this == &tmp) return *this;
    delete[]arr;
    arr = new int[tmp.n];
    n = tmp.n;
    for (int i=0; i<n; i++)
    {
        arr[i] = tmp.arr[i];
    }
    return *this;
}

int &Array::operator[](int ind)
{
    if (ind<0 || ind>n) return arr[0];
    return arr[ind];
}

bool Array::Test()
{
    for (int i=0; i<n-1; i++)
    {
        if (arr[i]>arr[i+1]) return 0;
    }
    return 1;
}

bool Array::operator==(Array tmp)
{
    if (n != tmp.n) return 0;
    bool flag;
    for (int i=0; i<n; i++)
    {
        flag = 0;
        for (int j=0; j<tmp.n; j++)
        {
            if (arr[i] == tmp.arr[j]) 
            {
                tmp.arr[j] = tmp.arr[tmp.n-1];
                tmp.n--;
                flag = 1;
                break;
            } 
        }
        if (!flag) return 0;
    }
    return 1;
}

istream &operator>>(istream &in, Array &obj) {
    int len;
    cout << "len: ";
    in >> len;
    cout << "\n array: ";
    int *arr;
    arr = new int[len];
    for (int i = 0; i < len; i++) {
        in >> arr[i];
    }
    delete[] obj.arr;
    obj.arr = arr;
    return in;
}

ostream &operator<<(ostream &out, Array &a) {
   out << "array: ";
    for (int i = 0; i < a.n; i++) {
        out << a[i] << " ";
    }
    out << "\n";
    return out;
}

void Array::Shell_sort() {
    for (int gap = n / 2; gap > 0; gap /= 2) { // Divide into gaps
        for (int start = 0; start < gap; start++) { // Start of each sub-array
            for (int i = start + gap; i < n; i += gap){ // Increment through elements in the sub-array
                int current_element = arr[i];
                int k;
                for (k = i - gap; k >= 0 && arr[k] > current_element; k -= gap) {
                    arr[k + gap] = arr[k];
                }
                arr[k + gap] = current_element;
            }
        }
    }
}

int partition(int *array, int left, int right)
{
    if (left >= right) {
        return 0;
    }
    int i = left;
    int j = right;
    int pivot = array[(left + right) / 2];
    while (i <= j) 
    {
        while (array[i] < pivot) {
            i++;
        }
        while (array[j] > pivot) {
            j--;
        }
        if (i <= j) {
            int tmp = array[i];
            array[i] = array[j];
            array[j] = tmp;
            
            i++;
            j--;
        }
    }

    partition(array, left, j);
    partition(array, i, right);
    return 0;
}

void Array::Hoare_sort() 
{
    partition(arr, 0, n - 1);
}

void Array::heapify(int size, int index) 
{
    int child = 2 * index + 1;
    int value = arr[index];
    bool flag = true;
    while (child < size && flag) {
        if (child + 1 < size && arr[child + 1] > arr[child]e child++; 
        if (arr[child] > value) {  
            arr[index] = arr[child];  
            index = child; 
            child = 2 * index + 1;
        } else {
            flag = false;
        }
    }
    arr[index] = value;
}

void Array::Heapsort() 
{
    for (int i = n / 2 - 1; i >= 0; i--)
        this->heapify(n, i);

    for (int i = n - 1; i > 0; i--) {

        int tmp = arr[0];
        arr[0] = arr[i];
        arr[i] = tmp;

        this->heapify(i, 0);
    }
}



void test() {

    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;
    Array t(10000, 1);
    Array t2=t;
    Array t3=t;

    double seconds = 0;
    auto time1 = high_resolution_clock::now();
    t.Shell_sort();
    auto time2 = high_resolution_clock::now();
    duration<double, std::milli> ms_double = time2 - time1;
    cout << "shell sort: " << ms_double.count() << " ms" << endl;
    time1 = high_resolution_clock::now();
    t2.Heapsort();
    time2 = high_resolution_clock::now();
    ms_double = time2 - time1;
    cout << "heap sort: " << ms_double.count() << " ms" << endl;
    time1 = high_resolution_clock::now();
    t3.Hoare_sort();
    time2 = high_resolution_clock::now();
    ms_double = time2 - time1;
    std::cout << "hoare sort: " << ms_double.count() << " ms" << endl;
    cout << (t2==t3) << "\n";
    cout << (t==t2) << "\n";
    cout << t.Test() << "\n";
    cout << t2.Test() << "\n";
    cout << t3.Test() << "\n";
}

int main() {
    Array t(20,1, 100);
    cout <<  t;
    t.Heapsort();
    cout << t;
    test();

    return 0;
}
