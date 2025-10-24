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

    friend ostream &operator<<(ostream &os, const Node &n) // made friend function
    {
        os << n.freq;
        os << n.ch; // D: Not sure about this.
        return os;
    }
};

template <typename T>
void swap(T &a, T &b)
{
    T temp = a;
    a = b;
    b = temp;
}

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
        return i > 0; // corrected
    }
    bool isEmpty()
    {

        return !cur_capacity; // was inverted
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
        while (i > 0 && arr[i] < arr[getParent(i)]) // smallest to the top
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

// template <typename T>
class huffmanTree
{
private:
    priorityQ<Node> theHuffman;
    int *freqTable;
    string *codeTable;
    Node root;
    int capacity;

    // ifstream infile;
    // ofstream outfile;

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

    // int freqTableCurrSize()
    // {
    //     int count = 0;
    //     int i = 0;
    //     while (i <= capacity && freqTable[i].freq != 0)
    //     {
    //         count++;
    //     }
    //     return count;
    // }

    void generateFreqTable(ifstream infile)
    {
        infile.open("testUncompressed.txt");
        char x;

        if (infile.is_open())
        {
            // int i = 0;
            int used[capacity]; /////////not used yet
            while (!infile.eof())
            {
                infile.get(x);
                freqTable[int((unsigned char)x)]++; // learned it from AI, to amek sure we don't get negative chars.
            }
            infile.close();

            for (int i = 0; i < capacity; i++)
            {
                if (freqTable[i] != 0)
                {
                    Node n(char(i), freqTable[i]);
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

    ofstream compress(ifstream infile)
    {
        priorityQ<Node> temp = theHuffman;
        ofstream outfile;
        infile.open("testUncompressed.txt");
        outfile.open("testCompressed.txt");

        while (temp.getCapacity() > 1)
        {
            Node *n1 = &temp.pop(); // the smaller
            Node *n2 = &temp.pop(); // the larger or the same
            Node N('\0', n1->freq + n2->freq);

            if (n1 > n2)
            {
                N.right = n1;
                N.left = n2;
            }
            else
            {
                N.left = n1;
                N.right = n2;
            }

            temp.push(N);
        } // Now you have a single top node that will be our whole tree root.
        root = temp.top();

        makeCodes(&root, ""); // now codeTable is filled at every needed index.

        if (!infile.is_open() || !outfile.is_open())
        {
            cout << "\nError: Uncompressed/compressed file is not open!\n";
            return outfile;
        }
        else
        {
            char x;
            while (!infile.eof())
            {
                infile.get(x);
                outfile << codeTable[(int((unsigned char)x))];
            }
        }
        // delete[] freqTable; ???? should we do it?
        return outfile;
    }

    ofstream decompress(ifstream infile); // Omar
    void preOrder()                       // Omar
    {
    }
};

int main()
{
    // Create a small sample file to test with
    // ofstream testFile("testUncompressed.txt");
    // testFile << "hello world";
    // testFile.close();

    // Create Huffman tree object
    huffmanTree huff;

    // Step 1: Generate frequency table
    ifstream infile;
    huff.generateFreqTable(infile);

    // Step 2: Compress file
    ifstream infile2;
    huff.compress(infile2);

    // Step 3: Display success message
    cout << "Compression done! Check 'testCompressed.txt'.\n";

    return 0;
}
