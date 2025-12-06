#ifndef DRIVER_H
#define DRIVER_H

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include "Node.h"        // Change to .h
#include "priorityQ.h"   // Change to .h

using namespace std;

class huffmanTree
{
private:
    priorityQ<Node *> theHuffman;
    int *freqTable;
    string *codeTable;
    int capacity;

    void deleteTree(Node *node);

public:
    huffmanTree(int sz = 256);
    ~huffmanTree();

    int *generateFreqTable(ifstream &infile);
    void makeCodes(Node *node, string code);
    Node *populate(int *freqT);
    string compress(ifstream &infile, const string &filename);
    string decompress(ifstream &infile, const string &filename);
};

// Declare functions (not define)
string getOutputFilename(const string &inputFilename, const string &suffix);
string run_process(string filename, bool process);

#endif // DRIVER_H
