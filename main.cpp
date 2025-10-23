#include <iostream>
#include <fstream>
using namespace std;
struct Node
{
    int data;
    Node* right;
    Node* left;
    Node(int d)
    {
        data = d;
    }
    bool operator>(const Node& b) const
    {
        return this->data >= b->data;
    }
    ostream& operator<<(ostream& os, const Node& n)
    {
        os << n.data;
        return os;
    }
};


template <typename T>
class priorityQ
{
private:
    T* arr;
    int max_capacity;
    int cur_capacity;
public:
    priorityQ(int sz);
    void push();// Eskandrani
    T pop();// Eskandrani
    T top();// Eskandrani
    T getLeftChild();// Eskandrani
    T getRightChild();// Eskandrani
    T getParent();// Eskandrani
    bool hasLeftChild(int i)
    {
        return cur_capacity > i * 2 + 1;
    }
    bool hasRightChild(int i)
    {
        return cur_capacity > i * 2 + 2;
    }
    bool hasParent(int i)
    {
        return cur_capacity > (i - 1) / 2;
    }
    bool isEmpty()
    {
        if(cur_capacity)
        {
            return 1;
        }
        return 0;
    }
    void shiftDown(); // Eskandrani
    void shiftUp(int i)
    {
        while(i > 0 && arr[i] > arr[getParent(i)])
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
    huffmanTree(int sz)
    {
        capacty = sz;
        freq = new int(sz);
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


