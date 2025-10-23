#include <iostream>
#include <fstream>
#include <stdexcept>
using namespace std;
struct Node
{
    int data;
    Node* right;
    Node* left;
    Node()
    {
        data = 0;
        left=nullptr;
        right =nullptr;
    }
    Node(int d)
    {
        data = d;
        left=nullptr;
        right =nullptr;
    }
    bool operator<(const Node& b) const
    {
        return this->data < b.data; // I think 'or equal' makes no sense
    }
    bool operator>(const Node& b) const
    {
        return this->data > b.data;
    }
    Node& operator=(const Node& b)
    {
        this->data = b.data;
        this->left = b.left;
        this->right = b.right;
        return *this;
    }
    friend ostream& operator<<(ostream& os, const Node& n) // made friend function
    {
        os << n.data;
        return os;
    }
};
template <typename T>
void swap(T& a, T& b)
{
    T temp = a;
    a = b;
    b = temp;
}

template <typename T>
class priorityQ
{
private:
    T* arr;
    int max_capacity;
    int cur_capacity;
public:
    priorityQ(int sz)
    {
        max_capacity = sz;
        arr = new T[sz];
        cur_capacity = 0;
    }    
    ~priorityQ()
    {
        delete[] arr;
    }
    void push(const T& x)
    {
        if(cur_capacity==max_capacity)
        {
            throw runtime_error("Debug: PriorityQ is Full");
        }
        cur_capacity++;
        arr[cur_capacity-1]=x;
        shiftUp(cur_capacity-1);
    }

    T pop()
    {
        if(isEmpty())
        {
            throw runtime_error("Debug: PriorityQ is Empty");
        }
        T popped=arr[0];
        arr[0]=arr[cur_capacity-1];
        cur_capacity--;
        shiftDown(0);
        return popped;
    }
    T top()
    {
        if(isEmpty())
        {
            throw runtime_error("Debug: PriorityQ is Empty");
        }
        return arr[0];
    }
    int getLeftChild(int i)
    {
        return i*2+1;
    }
    int getRightChild(int i)
    {
        return i*2+2;
    }
    int getParent(int i)
    {
        return (i-1)/2;
    }
    bool hasLeftChild(int i)
    {
        return cur_capacity > getLeftChild(i);
    }
    bool hasRightChild(int i)
    {
        return cur_capacity > getRightChild(i);
    }
    bool hasParent(int i)
    {
        return i > 0 ; // corrected
    }
    bool isEmpty()
    {
        
        return !cur_capacity; // was inverted
    }
    void shiftDown(int i)
    {
        while(hasLeftChild(i))
        {
            int Smallest = i;

            if(arr[getLeftChild(i)]< arr[Smallest])
            {
                Smallest=getLeftChild(i);
            }
            if(hasRightChild(i) && arr[getRightChild(i)]< arr[Smallest])
            {
                Smallest=getRightChild(i);
            }
            
            if(Smallest!=i)
            {
                swap(arr[i],arr[Smallest]);
                i=Smallest;
            }
            else
            {
                break;
            }
        }
    }
    void shiftUp(int i)
    {
        while(i > 0 && arr[i] < arr[getParent(i)]) // smallest to the top
        {
            swap(arr[i], arr[getParent(i)]);
            i = getParent(i);
        }
    }
    void print()
    {
        for(int i = 0; i < cur_capacity; i++)
        {
            cout << arr[i];
        }
        cout << endl;
    }
};

template <typename T>
class huffmanTree
{
private:
    priorityQ <T> pq;
    int* freq;
    int capacity;
public:
    huffmanTree(int sz): pq(sz)
    {
        capacity = sz;
        freq = new int[sz];
        fill_n(freq, sz, 0);
    }
    ~huffmanTree()
    {
        delete[] freq;
    }
    void populate_freq(ifstream infile) // Elhusseiny
    {

    }
    ofstream compress(); //Elhusseiny
    ofstream decompress(ifstream infile); //Omar
    void preOrder()   // Omar
    {

    }
};
int main()
{

    return 0;
}


