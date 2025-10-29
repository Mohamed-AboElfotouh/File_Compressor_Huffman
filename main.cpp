#include <iostream>
#include <fstream>
#include <stdexcept>
#include "Node.cpp"
#include "priorityQ.cpp"
using namespace std;
class huffmanTree
{
private:
    priorityQ<Node *> theHuffman;
    int *freqTable;
    string *codeTable;
    int capacity;

    void deleteTree(Node *node)
    {
        if (node)
        {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }

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
        delete[] codeTable;
    }

    int *generateFreqTable(ifstream &infile)
    {
        char x;

        if (infile.is_open())
        {
            while (infile.get(x))
            {
                freqTable[(unsigned char)x]++;
            }
            return freqTable;
        }
        else
        {
            cout << "\nError: Uncompressed file is not open!\n";
            return NULL;
        }
    }

    void makeCodes(Node *node, string code)
    {
        if (!node)
            return;

        if (!node->left && !node->right) // reached a leaf node
        {
            if (code != "")
            {
                codeTable[(unsigned char)node->ch] = code; // when you reach the leaf assign the code to its index.
                return;
            }
            else
            {
                codeTable[(unsigned char)node->ch] = "0"; //in case only 1 character in the whole text
                return;
            }
        }

        makeCodes(node->left, code + "0");  // go left = 0
        makeCodes(node->right, code + "1"); // go right = 1;
    }

    Node *populate(int *freqT)
    {
        priorityQ<Node *> localHuffman(capacity);

        for (int i = 0; i < capacity; i++)
        {
            if (freqT[i] != 0)
            {
                Node *n = new Node(static_cast<char>(i), freqT[i]);
                localHuffman.push(n);
            }
        }

        while (localHuffman.getCapacity() > 1)
        {
            Node *n1 = localHuffman.pop();
            Node *n2 = localHuffman.pop();

            Node *N = new Node('\0', n1->freq + n2->freq);

            if (*n1 < *n2)
            {
                N->left = n1;
                N->right = n2;
            }
            else
            {
                N->left = n2;
                N->right = n1;
            }
            localHuffman.push(N);
        }

        return localHuffman.top();
    }

    void compress(ifstream &infile)
    {
        ofstream outfile("testCompressed.txt");

        int *freqT = generateFreqTable(infile);
        Node *root = populate(freqT);
        makeCodes(root, ""); // now codeTable is filled at every needed index.

        if (!infile.is_open() || !outfile.is_open())
        {
            cout << "\nError: Input/Output file is not open!\n";
            return;
        }
        else
        {
            bool first = true;
            for (int i = 0; i < capacity; i++)
            {
                if (freqT[i] != 0)
                {
                    if (!first)
                        outfile << ','; // comma before not after
                    first = false;
                    if (char(i) == '\n')
                        outfile << "En" << ':' << freqT[i];
                    else
                        outfile << char(i) << ':' << freqT[i];
                }
            }
            outfile << endl;

            infile.clear();
            infile.seekg(0, ios::beg); // to reset it from outer files

            char x;
            while (infile.get(x))
            {
                // infile.get(x);
                outfile << codeTable[(int((unsigned char)x))];
            }
        }

        outfile.close();
        infile.close();
        deleteTree(root);

        return;
    }

    void decompress(ifstream &infile)
    {
        ofstream outfile("decompressed.txt");
        if (!infile.is_open())
            throw runtime_error("Compressed file not open.");

        string head;
        getline(infile, head);

        int freq[capacity]{};

        for (int i = 0; i < head.length(); i++)
        {
            char symbol = head[i];
            i++;
            if (i >= head.length())
                break;
            if (head[i] == 'n')
            {
                symbol = '\n';
                i++;
            }
            i++;

            int count = 0;

            while (i < head.length() && isdigit(head[i]))
            {
                count = count * 10 + (head[i] - '0');
                i++;
            }

            freq[(unsigned char)symbol] = count;
        }

        Node *decompress_root = populate(freq);

        string bitstring;
        string output;

        char bit;

        while (infile.get(bit))
            if (bit == '0' || bit == '1')
                bitstring += bit;

        Node *cur = decompress_root;

        for (auto x : bitstring)
        {
            if (x == '0')
            {
                cur = cur->left;
            }
            else
            {
                cur = cur->right;
            }
            if (!cur->left && !cur->right)
            {
                outfile << cur->ch;
                cur = decompress_root;
            }
        }

        deleteTree(decompress_root);
        infile.close();
        outfile.close();

        return;
    }
};

int main()
{

    huffmanTree huff(256);
    // step 1: generate frequency table
    ifstream infile("testUncompressed.txt");
    if (!infile.is_open())
    {
        cout << "File not found '\n";
        return 1;
    }
    // huff.generateFreqTable(infile);
    infile.close();

    // step 2: compress the file
    infile.open("testUncompressed.txt");
    if (!infile.is_open())
    {
        cout << "Error: Could not reopen file for compression.\n";
        return 1;
    }
    huff.compress(infile);
    infile.close();
    cout << "Compression done! Check testCompressed.txt\n";

    // step 3: decompress the file
    infile.open("testCompressed.txt");
    if (!infile.is_open())
    {
        cout << "Error: Could not open compressed file for decompression.\n";
        return 1;
    }
    huff.decompress(infile);
    infile.close();
    cout << "Decompression done! Check decompressed.txt\n";

    return 0;
}
