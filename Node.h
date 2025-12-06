#ifndef NODE_H
#define NODE_H

#include <iostream>
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
        os << " " <<n.ch;
        return os;
    }
};

#endif // NODE_H