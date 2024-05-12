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
class BST;
struct WordInfo
{
    string word; //Word
    map<string, int> fileOccurrences; // Map stores filenames and frequencies
};

template<typename Key,typename Value>
class BSTNode
{
    Value info;
    BSTNode* left;
    BSTNode* right;
    int height;
    BSTNode(const Key& fileName, const Key& word, int frequency, BSTNode* lt = nullptr, BSTNode* rt = nullptr, int h = 1) : left(lt), right(rt), height(h) {
        info.fileOccurrences[fileName] = frequency;
        info.word = word;
    }
    friend class BST<Key,Value>;
};


template<typename Key, typename Value>
class BST
{
private:
    BSTNode<Key,Value>* root;
   
   
    void makeEmpty(BSTNode<Key,Value>*& t); // Deletes the tree
    BSTNode<Key,Value>* insert(const Key& fileName, const Key& word, int frequency, BSTNode<Key,Value>* t);
    BSTNode<Key,Value>* remove(const Key& word, BSTNode<Key,Value>* t);
    void print(BSTNode<Key,Value>* root);
    bool word_exist(Key word,BSTNode<Key,Value>* t);
    BSTNode<Key,Value>* minvaluenode(BSTNode<Key,Value>* node); // returns the node minimum key
    int tree_size(BSTNode<Key,Value>* node );
public:
    BST() : root(nullptr) {}
    ~BST() { makeEmpty(root); }
    void remove(const Key& word);
    BSTNode<Key,Value>* getRoot() const { return root; } // Function to access the root
    void insert(const Key& fileName, const Key& word, int frequency); // Public insert function
    bool Find(const Key& word, BSTNode<Key,Value>* t, map<string, int>& occurrences);// Check whether the Key is in tree or not
    void print();
    bool word_exist(Key word);
    int tree_size();
};
