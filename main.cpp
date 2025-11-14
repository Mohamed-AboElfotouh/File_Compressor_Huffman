#include <iostream>
#include <fstream>
#include <stdexcept>
using namespace std;
struct Node
{
    int freq;
    char ch;
    Node *right;
    Node *left;
    Node()
    {
        ch = '\0';
        freq = 0;
        left = nullptr;
        right = nullptr;
    }
    Node(char x, int d)
    {
        ch = x;
        freq = d;
        left = nullptr;
        right = nullptr;
    }
    Node(const Node &n)
    {
        this->ch = n.ch;
        this->freq = n.freq;
        this->left = n.left;
        this->right = n.right;
    }

    bool operator<(const Node &b) const
    {
        if (this->freq == b.freq)
            return this->ch < b.ch;
        else
            return this->freq < b.freq;
    }

    bool operator>(const Node &b) const
    {
        if (this->freq == b.freq)
            return this->ch > b.ch;
        else
            return this->freq > b.freq;
    }

    bool operator==(const Node &b) const
    {
        return (this->ch == b.ch && this->freq == b.freq && this->left == b.left && this->right == b.right);
    }

    Node &operator=(const Node &b)
    {
        this->freq = b.freq;
        this->ch = b.ch;
        this->left = b.left;
        this->right = b.right;
        return *this;
    }

    friend ostream &operator<<(ostream &os, const Node &n)
    {
        os << n.freq;
        os << n.ch; 
        return os;
    }
};



template <typename T>
class priorityQ
{
private:
    T *arr;
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
    void push(const T &x)
    {
        if (cur_capacity == max_capacity)
        {
            throw runtime_error("Debug: PriorityQ is Full");
        }
        cur_capacity++;
        arr[cur_capacity - 1] = x;
        shiftUp(cur_capacity - 1);
    }

    T pop()
    {
        if (isEmpty())
        {
            throw runtime_error("Debug: PriorityQ is Empty");
        }
        T popped = arr[0];
        arr[0] = arr[cur_capacity - 1];
        cur_capacity--;
        shiftDown(0);
        return popped;
    }
    T top()
    {
        if (isEmpty())
        {
            throw runtime_error("Debug: PriorityQ is Empty");
        }
        return arr[0];
    }
    int getLeftChild(int i)
    {
        return i * 2 + 1;
    }
    int getRightChild(int i)
    {
        return i * 2 + 2;
    }
    int getParent(int i)
    {
        return (i - 1) / 2;
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
        return i > 0;
    }
    bool isEmpty()
    {

        return !cur_capacity;
    }

    int getCapacity()
    {
        return cur_capacity;
    }

    void shiftDown(int i)
    {
        while (hasLeftChild(i))
        {
            int Smallest = i;

            if (arr[getLeftChild(i)] < arr[Smallest])
            {
                Smallest = getLeftChild(i);
            }
            if (hasRightChild(i) && arr[getRightChild(i)] < arr[Smallest])
            {
                Smallest = getRightChild(i);
            }

            if (Smallest != i)
            {
                swap(arr[i], arr[Smallest]);
                i = Smallest;
            }
            else
            {
                break;
            }
        }
    }
    void shiftUp(int i)
    {
        while (i > 0 && arr[i] < arr[getParent(i)]) 
        {
            swap(arr[i], arr[getParent(i)]);
            i = getParent(i);
        }
    }
    void print()
    {
        for (int i = 0; i < cur_capacity; i++)
        {
            cout << arr[i];
        }
        cout << endl;
    }
};

template <typename T1>
class huffmanTree
{
private:
    priorityQ<Node> theHuffman;
    int *freqTable;
    string *codeTable;
    Node* root;
    int capacity;

public:
    huffmanTree(int sz = 256) : theHuffman(sz)
    {
        capacity = sz;
        freqTable = new int[sz];
        codeTable = new string[sz];
        fill_n(freqTable, sz, 0);
        fill_n(codeTable, sz, "");
    }
    ~huffmanTree()
    {
        delete[] freqTable;
    }

    void generateFreqTable(ifstream& infile)
    {
        char x;

        if (infile.is_open())
        {
            while (infile.get(x))
            {
                freqTable[(unsigned char)x]++;
            }

            for (int i = 0; i < capacity; i++)
            {
                if (freqTable[i] != 0)
                {
                    Node n(static_cast<char>(i), freqTable[i]);
                    theHuffman.push(n);
                }
            }

            return;
        }
        else
        {
            cout << "\nError: Uncompressed file is not open!\n";
            return;
        }
    }

    void makeCodes(Node *node, string code)
    {
        if (!node)
            return;

        if (!node->left && !node->right) // reached a leaf node
        {
            codeTable[(unsigned char)node->ch] = code; // when you reach the leaf assign the code to its index.
            return;
        }

        makeCodes(node->left, code + "0");  // go left = 0
        makeCodes(node->right, code + "1"); // go right = 1;
    }

    void compress(ifstream& infile)
    {
        ofstream outfile("testCompressed.txt");
        while (theHuffman.getCapacity() > 1)
        {
            Node n1 = theHuffman.pop(); // the smaller
            Node n2 = theHuffman.pop(); // the larger or the same

            Node* leftChild = new Node(n1);
            Node* rightChild = new Node(n2);

            Node N('\0', leftChild->freq + rightChild->freq);

            if (n1 > n2)
            {
                N.right = leftChild;
                N.left = rightChild;
            }
            else
            {
                N.left = leftChild;
                N.right = rightChild;
            }

            theHuffman.push(N);
        } // Now you have a single top node that will be our whole tree root.

        root = new Node(theHuffman.top());

        makeCodes(root, ""); // now codeTable is filled at every needed index.

        if (!infile.is_open() || !outfile.is_open())
        {
            cout << "\nError: Uncompressed/compressed file is not open!\n";
            return;
        }
        else
        {
            char x;
            while (infile.get(x))
            {
                outfile << codeTable[(int((unsigned char)x))];
            }
        }
        return;
    }

    void decompress(ifstream& infile) {
        ofstream outfile("decompressed.txt");
        if (!infile.is_open())
            throw runtime_error("Compressed file not open.");

        string bitstring;
        string output;

        char bit;

        while (infile.get(bit))
            if (bit == '0' || bit == '1')
                bitstring += bit;

        Node* cur = root;

        for (auto x : bitstring) {
            if (x == '0') {
                cur = cur->left;
            } else {
                cur = cur->right;
            }
            if (!cur->left && !cur->right) {
                outfile <<  cur->ch;
                cur = root;
            }
        }
    
        return;

    }
    
};


int main()
{
   
    
    huffmanTree <Node> huff(256);
    //step 1: generate frequency table
    ifstream infile("testUncompressed.txt");
    if (!infile.is_open()) {
        cout << "File not found '\n";
        return 1;
    }
    huff.generateFreqTable(infile);
    infile.close();

    // step 2: compress the file
    infile.open("testUncompressed.txt");
    if (!infile.is_open()) {
        cout << "Error: Could not reopen file for compression.\n";
        return 1;
    }
    huff.compress(infile);
    infile.close();
    cout << "Compression done! Check testCompressed.txt\n";

    // step 3: decompress the file
    infile.open("testCompressed.txt");
    if (!infile.is_open()) {
        cout << "Error: Could not open compressed file for decompression.\n";
        return 1;
    }
    huff.decompress(infile);
    infile.close();
    cout << "Decompression done! Check decompressed.txt\n";

    return 0;
}
