#include <iostream>
#include <fstream>
#include <stdexcept>
#include "Node.cpp"
#include "priorityQ.cpp"
#include <QDebug>
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
            qDebug() << "\nError: Uncompressed file is not open!\n";
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
                codeTable[(unsigned char)node->ch] = "0"; // in case only 1 character in the whole text
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

    string compress(ifstream &infile, const string &filename)
    {
        // Validate file extension
        size_t lastDot = filename.find_last_of('.');
        if (lastDot == string::npos || filename.substr(lastDot) != ".txt") {
            throw runtime_error("Error: Compression requires a .txt file.");
        }
        
        string resultfile = filename.substr(0, filename.find_last_of('.')) + ".DAAB";
        ofstream outfile(resultfile, ios::binary);
        if (!outfile.is_open())
            throw runtime_error("Output file failed.");

        int *freqT = generateFreqTable(infile);
        Node *root = populate(freqT);
        makeCodes(root, "");

        outfile.write(reinterpret_cast<char *>(freqT), sizeof(int) * 256);

        infile.clear();
        infile.seekg(0, ios::beg);

        unsigned char buffer = 0;
        int bitCount = 0;

        char c;
        while (infile.get(c))
        {
            string &code = codeTable[(unsigned char)c];
            for (char bit : code)
            {
                buffer <<= 1;
                if (bit == '1')
                    buffer |= 1;
                bitCount++;

                if (bitCount == 8)
                {
                    outfile.put(buffer);
                    bitCount = 0;
                    buffer = 0;
                }
            }
        }

        if (bitCount > 0)
        {
            buffer <<= (8 - bitCount);
            outfile.put(buffer);
        }

        deleteTree(root);
        outfile.close();
        infile.close();
        return resultfile;
    }

    string decompress(ifstream &infile, const string &filename)
    {
        // Validate file extension
        size_t lastDot = filename.find_last_of('.');
        if (lastDot == string::npos || filename.substr(lastDot) != ".DAAB") {
            throw runtime_error("Error: Decompression requires a .DAAB file.");
        }
        
        string resultfile = filename.substr(0, filename.find_last_of('.')) + ".txt";
        ofstream outfile(resultfile, ios::binary);
        if (!outfile.is_open())
            throw runtime_error("Output file failed.");

        int freq[256];
        infile.read(reinterpret_cast<char *>(freq), sizeof(int) * 256);

        Node *root = populate(freq);
        Node *cur = root;

        unsigned char byte;
        while (infile.read(reinterpret_cast<char *>(&byte), 1))
        {
            for (int i = 7; i >= 0; i--)
            {
                bool bit = (byte >> i) & 1;

                cur = bit ? cur->right : cur->left;

                if (!cur->left && !cur->right)
                {
                    outfile.put(cur->ch);
                    cur = root;
                }
            }
        }

        deleteTree(root);
        infile.close();
        outfile.close();
        return resultfile;
    }
};

string getOutputFilename(const string &inputFilename, const string &suffix)
{
    size_t lastDot = inputFilename.find_last_of('.');

    if (lastDot == string::npos)
    {
        return inputFilename + suffix;
    }

    return inputFilename.substr(0, lastDot) + suffix + inputFilename.substr(lastDot);
}

string run_process(string filename, bool process)
{

    string resultfile = "";
    
    // Validate file extension based on operation type
    size_t lastDot = filename.find_last_of('.');
    if (lastDot != string::npos) {
        string extension = filename.substr(lastDot);
        
        if (process) {
            // Compression: must be .txt
            if (extension != ".txt") {
                qDebug() << "Error: Compression requires a .txt file.\n";
                return string();
            }
        } else {
            // Decompression: must be .DAAB
            if (extension != ".DAAB") {
                qDebug() << "Error: Decompression requires a .DAAB file.\n";
                return string();
            }
        }
    } else {
        qDebug() << "Error: File must have an extension.\n";
        return string();
    }
    
    huffmanTree huff(256);

    ifstream infile(filename, ios::binary);
    if (!infile.is_open())
    {
        qDebug() << "Error: Could not open file.\n";
        return string();
    }
    if (process)
    {

        resultfile = huff.compress(infile, filename);
        infile.close();
        qDebug() << "Compression done! Check testCompressed.txt\n";
    }
    else
    {
        resultfile = huff.decompress(infile, getOutputFilename(filename, "_decompressed"));
        infile.close();
        qDebug() << "Decompression done! Check decompressed.txt\n";
    }

    return resultfile;
}