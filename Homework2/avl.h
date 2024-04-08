#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm> 
using namespace std;

template<typename Key,typename Value>
class AVLTree;
struct WordInfo
{
    string word; //Word
    map<string, int> fileOccurrences; // Map stores filenames and frequencies
};

template<typename Key,typename Value>
class AVLNode
{
    Value info;
    AVLNode* left;
    AVLNode* right;
    int height;
    AVLNode(const Key& fileName, const Key& word, int frequency, AVLNode* lt = nullptr, AVLNode* rt = nullptr, int h = 1) : left(lt), right(rt), height(h) {
        info.fileOccurrences[fileName] = frequency;
        info.word = word;
    }
    friend class AVLTree<Key,Value>;
};


template<typename Key, typename Value>
class AVLTree
{
private:
    AVLNode<Key,Value>* root;
    AVLNode<Key,Value>* rotateRight(AVLNode<Key,Value>* node); // Right rotation
    AVLNode<Key,Value>* rotateLeft(AVLNode<Key,Value>* node); // Left rotation
    int getHeight(AVLNode<Key,Value>* node); // return height of a node
    int balance_Factor(AVLNode<Key,Value>* node); // returns bf=L_height-R_height
    void makeEmpty(AVLNode<Key,Value>*& t); // Deletes the tree
    AVLNode<Key,Value>* insert(const Key& fileName, const Key& word, int frequency, AVLNode<Key,Value>* t);
    AVLNode<Key,Value>* remove(const Key& word, AVLNode<Key,Value>* t);
    void print(AVLNode<Key,Value>* root);
    AVLNode<Key,Value>* Balance(AVLNode<Key,Value>* node, const Key& word); // Balances the tree
    AVLNode<Key,Value>* minvaluenode(AVLNode<Key,Value>* node); // returns the node minimum key

public:
    AVLTree() : root(nullptr) {}
    ~AVLTree() { makeEmpty(root); }
    void remove(const Key& word);
    AVLNode<Key,Value>* getRoot() const { return root; } // Function to access the root
    void insert(const Key& fileName, const Key& word, int frequency); // Public insert function
    bool Find(const Key& word, AVLNode<Key,Value>* t, map<string, int>& occurrences);// Check whether the Key is in tree or not
    void print();
};
