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
};


template <typename T>
class priorityQ
{
private:
    T* arr;
    int max_capacity;
public:
    priorityQ(int sz);
    void push();// Eskandrani
    T pop();// Eskandrani
    T top();// Eskandrani
    T getLeftChild();// Eskandrani
    T getRightChild();// Eskandrani
    T getParent();// Eskandrani
    bool hasLeftChild();// AboElfotouh
    bool hasRightChild();  // AboElfotouh
    bool hasParent();   // AboElfotouh
    bool isEmpty();  // AboElfotouh
    void shiftDown(); // Eskandrani
    void shiftUp();   // AboElfotouh
    void print();// AboElfotouh
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


