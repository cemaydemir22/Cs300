#include <iostream>
#include "BST.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;


// To make the words lowercase
string toLowercase(string str)
{
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

//Returns the minimum valued node
template<typename Key, typename Value>
BSTNode<Key,Value>* BST<Key, Value>::minvaluenode(BSTNode<Key,Value>* node)
{
    BSTNode<Key,Value>* current = root;
    while (current && current->left) {
        current = current->left;
    }
    return current;
}

template<typename Key, typename Value>
void BST<Key, Value>::insert(const Key& fileName, const Key& word, int frequency)
{
    root = insert(fileName, word, frequency, root);
}


template<typename Key, typename Value>
int BST<Key, Value>::tree_size()
{
    return tree_size(root);
}

template<typename Key, typename Value>
int BST<Key, Value>::tree_size(BSTNode<Key,Value>*  node)
{
    static int count=0;

    if(node->left)
    {
        tree_size(node->left);
    }
    count++;
    if(node->right)
    {
        tree_size(node->right);
    }
    return count;
}


template<typename Key, typename Value>
void BST<Key, Value>::print()
{
    print(root);
}

template<typename Key, typename Value>
void BST<Key, Value>::print(BSTNode<Key,Value>* t)
{
    if (t->left)
    {
        print(t->left);
    }
    cout << t->info.word << " ";
    if (t->right) {
        print(t->right);
    }
}

// To insert a node
template<typename Key, typename Value>
BSTNode<Key,Value>* BST<Key, Value>::insert(const Key& fileName, const Key& word, int frequency, BSTNode<Key,Value>* node)
{

    if (!node) {
        return new BSTNode<Key,Value>(fileName, word, frequency);
    }
    else if (toLowercase(word) < toLowercase(node->info.word)) {
        node->left = insert(fileName, word, frequency, node->left);
    }
    else if (toLowercase(node->info.word) < toLowercase(word)) {
        node->right = insert(fileName, word, frequency, node->right);
    }
    else {
        node->info.fileOccurrences[fileName] += frequency; // Increment frequency if word already exists else add and increment
        return node;
    }

    return node;
}

// Function to remove a word from the AVL tree
template<typename Key, typename Value>
void BST<Key, Value>::remove(const Key& word) {
    root = remove(word, root);
}

// Function to remove
template<typename Key, typename Value>
BSTNode<Key,Value>* BST<Key, Value>::remove(const Key& word, BSTNode<Key,Value>* node) {
    map<string, int> occurrences;
    if (!Find(toLowercase(word), node, occurrences)) {// either there is no node or the number is not in the tree
        return node;
    }
    else {
        if (toLowercase(word) < node->info.word) {
            node->left = remove(toLowercase(word), node->left);
        }
        else if (toLowercase(word) > node->info.word) {
            node->right = remove(toLowercase(word), node->right);
        }
        else {
            //equal case
            // the cases are node has no children or node has one children or node has 2 children
            if (node->left == nullptr && node->right == nullptr) {// no child
                delete node;
                node = nullptr;
                return node;
            }
            else if (node->left && !node->right) {// only left child 
                BSTNode<Key,Value>* temp;
                temp = node;
                node = node->left;
                delete node;
                return node;
            }
            else if (!node->left && node->right) {// only right child 
                BSTNode<Key,Value>* temp;
                temp = node;
                node = node->right;
                delete temp;
                return node;
            }
            else {// 2 child

                BSTNode<Key,Value>* temp;
                temp = minvaluenode(node->right);
                node->info.word = temp->info.word;
                node->right = remove(temp->info.word, node->right);


            }
        }
    }
    return node;
}


template<typename Key, typename Value>
bool BST<Key, Value>::Find(const Key& word,BSTNode<Key,Value>* node, map<string, int>& occurrences)
{
    if (!node) {
        return false;
    }
    else {

        if (word < toLowercase(node->info.word))
        {
            return Find(word, node->left, occurrences);
        }
        else if (toLowercase(node->info.word) < word)
        {
            return Find(word, node->right, occurrences);
        }
        else
        {
            occurrences = node->info.fileOccurrences;
            return true;
        }
    }
}


template<typename Key, typename Value>
bool BST<Key, Value>::word_exist(Key word)
{
    return word_exist(word, root);
}

template<typename Key, typename Value>
bool BST<Key, Value>::word_exist(Key word, BSTNode<Key, Value>* node)
{
    if (node == nullptr) {
        return false; // Base case: node is null, word not found
    }

    if (node->info.word == word) {
        return true; // Base case: word found
    }

    // Recursively search in the left subtree
    if (node->left) {
        if (word_exist(word, node->left)) {
            return true;
        }
    }

    // Recursively search in the right subtree
    if (node->right) {
        if (word_exist(word, node->right)) {
            return true;
        }
    }

    return false; // Word not found in this subtree
}




template<typename Key, typename Value>
void BST<Key, Value>::makeEmpty(BSTNode<Key,Value>*& t)
{
    if (t != nullptr)
    {
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
    }
    t = nullptr;
}

