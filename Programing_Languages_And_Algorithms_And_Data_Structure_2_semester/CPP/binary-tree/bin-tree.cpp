#include <iostream>
#include <queue>
using namespace std;

class tree_elem
{
    public:
        int m_data;     //ключ - число, хранящееся в вершине
        tree_elem * m_left;    //указатель на левого потомка данной вершины
        tree_elem * m_right;  //указатель на правого потомка данной вершины
        // Конструктор класса
        //Левые и правые поддеревья инициализирует нулевыми указателями
        tree_elem(int val){
            m_left = nullptr;   
            m_right = nullptr;
            m_data = val;
        }
};

class binary_tree
{
  private:
    tree_elem * m_root;        //указатель на корень дерева
        int m_size;                       //количество элементов в дереве
        void print_tree (tree_elem *);   //используется в рекурсивном алгоритме обхода дерева
        void delete_tree (tree_elem *); //используется в рекурсивном алгоритме удаления в дереве
    public:
        // binary_tree (int);      //конструктор

        binary_tree(int len = 1){                   //конструктор с рандомными значениями
            m_root = new tree_elem(rand() % 100);
            m_size=0;
            for (int i = 0; i < len; i++) {
                this -> insert(rand() % 100);
            }
        }

        binary_tree(int len, int *arr){         //конструктор со значениями с массива
            m_root = new tree_elem(arr[0]);
            for (int i = 1; i < len; i++) {
                this -> insert(arr[i]);
            }
        }

        binary_tree(const binary_tree &tree){             //конструктор копирования
            m_root = copy(m_root, tree.m_root);
        }        

        ~binary_tree ();         //деструктор    
        void print ();             //вывод всех элементов поддерева
        bool find (int);          //поиск элемента в дереве
        tree_elem* copy(tree_elem *, tree_elem *);
        void insert (int);         //добавление нового элемента (узла) 
        void erase (int);         //удаление узла с указанным ключом 
        int size(tree_elem *);                  // возврат количества элементов в дереве 
        binary_tree operator =(const binary_tree &);
        int max(tree_elem *);
        int min(tree_elem *);
        void BFS();
        void LRN(tree_elem *);
};

int binary_tree::min(tree_elem *node = nullptr){
    if(node == nullptr)node = m_root;
    tree_elem* curr = node;
    while (curr->m_left) {
        curr = curr->m_left; //находим самый правый лист, у которого будет максимальное значение
    }
    if (curr) {
        return curr->m_data;   //вернем его ключ, если нашли
    }
    return -1;
}

int binary_tree::max(tree_elem *node = nullptr){
    if(node == nullptr)node = m_root;
    tree_elem* curr = node;
    while (curr->m_right) {
        curr = curr->m_right; //находим самый правый лист, у которого будет максимальное значение
    }
    if (curr) {
        return curr->m_data;   //вернем его ключ, если нашли
    }
    return -1;
}

binary_tree binary_tree::operator =(const binary_tree &tree){
    if (this->m_root != tree.m_root && this->m_size != tree.m_size){
        delete_tree(m_root);
        m_root = copy(m_root, tree.m_root);
    }
    return *this;
}

bool binary_tree::find(int k){    //поиск по ключу
    if (m_root == nullptr)
        return false;
    tree_elem* n = m_root;
    while(n) {
        if (n->m_data == k) return true;
        else if (n -> m_data > k && n -> m_left == nullptr) return false;
        else if (n -> m_data < k && n -> m_right == nullptr) return false;
        if (n -> m_data > k) n = n -> m_left;
        else n = n -> m_right;
    }
}

tree_elem * binary_tree::copy(tree_elem *new_node, tree_elem *node){ //копирование дерева
    if(!node){          //если узела нет
        return  nullptr;
    }
    new_node = new tree_elem(node -> m_data);                               //копируем значения узла
    new_node -> m_left =copy(new_node -> m_left,node -> m_left);     //копируем левого потомка
    new_node -> m_right = copy(new_node -> m_right, node -> m_right);//копируем правого потомка
    return new_node;
}

int binary_tree::size(tree_elem* node) 
{ 
    return m_size; 
} 

// binary_tree::binary_tree(int key){
//     m_root = new tree_elem (key);
//     m_size = 1;
// }

binary_tree::~binary_tree (){
    delete_tree (m_root);
}

void binary_tree::delete_tree (tree_elem * curr){
    if (curr){
       delete_tree (curr->m_left);
       delete_tree (curr->m_right);
       delete curr;
    }
}

void binary_tree::insert (int key)
 {
    tree_elem * curr = m_root;   //настройка на корень дерева
    while (curr && curr->m_data != key)  // цикл, пока key ≠ ключу текущего узла
    {    
       if (curr->m_data > key && curr->m_left == nullptr)
         {  //key меньше ключа текущего узла  и левая ссылка текущего узла NULL
            curr->m_left = new tree_elem(key);     //новый узел формируется по левой ветви
            ++m_size;
            return;
          }
        if (curr->m_data < key && curr->m_right == nullptr)
         { //key больше ключа текущего узла  и правая ссылка текущего узла NULL
            curr->m_right = new tree_elem(key); //новый узел формируется по правой ветви
            ++m_size;
            return;
        }
        //Движение по левой или правой ветви родительского узла
        if (curr->m_data > key)
            curr = curr->m_left;
        else
            curr = curr->m_right;
    } 
}

void binary_tree:: print(){
    print_tree (m_root);
    cout << endl;
}

void binary_tree::print_tree (tree_elem * curr){
    if (curr)   // Проверка на ненулевой указатель
    {
       print_tree (curr->m_left);
       cout << curr->m_data << " "; 
       print_tree (curr->m_right);
    }
}

void binary_tree::erase(int key) {
    tree_elem * curr = m_root;
    tree_elem * parent = NULL;
    while (curr && curr->m_data != key){ 
        // Настройка на адрес текущего узла; ключ узла не равен ключу поиска 
        parent = curr;
        if (curr->m_data > key)  {
            curr = curr->m_left;    //продолжение спуска по левому поддереву
        }
        else  {
            curr = curr->m_right;  //продолжение спуска по правому поддереву
        }
    }

    //Узла в дереве нет
    if (!curr)   
        return; 

    if (curr->m_left == nullptr) {
        // Вместо curr подвешивается его правое поддерево
        if (parent && parent->m_left == curr)
                parent->m_left = curr->m_right;
        if (parent && parent->m_right == curr)
                parent->m_right = curr->m_right;
            --m_size;
            delete curr;
            return;
    }

    else if (curr->m_right == nullptr){
        // Вместо curr подвешивается его левое поддерево
        if (parent && parent->m_left == curr)
                parent->m_left = curr->m_left;
            if (parent && parent->m_right == curr)
                parent->m_right = curr->m_left;
            --m_size;
            delete curr;
            return;
    }

    else{
        curr->m_data = max(curr->m_left);
        curr = curr->m_left;
        while (curr->m_right) {
            curr = curr->m_right;
        }
        if (curr) {
            delete curr;
        }
        --m_size;
        return;
    }

}

void binary_tree::BFS() {
    if (m_root == nullptr) {
        return;
    }

    queue<tree_elem*> q;
    q.push(m_root);

    while (!q.empty()) {
        tree_elem* n = q.front();
        q.pop();
        cout << n->m_data << " ";

        if (n->m_left != nullptr) {
            q.push(n->m_left);
        }

        if (n->m_right != nullptr) {
            q.push(n->m_right);
        }
    }
}

void binary_tree::LRN (tree_elem *r = NULL){
    if (r == NULL) r = m_root;
    if (r -> m_left) LRN(r -> m_left);
    if (r -> m_right) LRN(r -> m_right);
    cout << r -> m_data << " ";
}

int main(){

    int a[] = {7, 2, 12, 9, 3, 11, 13};
    binary_tree tree1(10);
    binary_tree tree2(tree1);
    binary_tree tree3(6);
    binary_tree tree4(7, a);
    tree3 = tree1;

    cout << "tree1: " << endl;
    tree1.print();
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << "tree2: " << endl;
    tree2.print();
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << "tree4: " << endl;
    tree3.print();
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << "tree4: " << endl;
    tree4.print();
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;

    cout<< "min tree4 = " << tree4.min() << endl;
    cout<< "max tree4 = " << tree4.max() << endl;
    cout<< "search 3 in tree4 = " << tree4.find(3) << endl;
    cout<< "search 1000 in tree4 = " << tree4.find(1000) << endl << endl;
    cout<< "delete 3:"<< endl;
    tree4.print();
    cout <<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" <<endl;
    tree4.erase(3);
    tree4.print();
    cout << endl;
    cout << "LRN:" << endl;
    tree4.LRN();
    cout << endl;
    cout << "BFS:" << endl;
    tree4.BFS();
    cout << endl << endl;

    binary_tree tree5(12);
    int key;
    tree5.print();
    cout<< "Enter key: "<< endl;
    cin >> key;
    tree5.erase(key);
    tree5.print();

    return 0;
}