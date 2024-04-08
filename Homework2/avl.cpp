#include <iostream>
#include "avl.h"
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
AVLNode<Key,Value>* AVLTree<Key, Value>::minvaluenode(AVLNode<Key,Value>* node)
{
    AVLNode<Key,Value>* current = root;
    while (current && current->left) {
        current = current->left;
    }
    return current;
}


// To balance the tree with 4 possible rotations
template<typename Key, typename Value>
AVLNode<Key,Value>* AVLTree<Key, Value>::Balance(AVLNode<Key,Value>* t, const Key& word)
{

    // Update height
    t->height = max(getHeight(t->left), getHeight(t->right)) + 1;

    // Find Balance factor
    int balance = balance_Factor(t);

    // Left Left 
    if (balance > 1 && balance_Factor(t->left) >= 0) {
        return rotateRight(t);
    }
    // Left Right 
    if (balance > 1 && balance_Factor(t->left) < 0) {
        t->left = rotateLeft(t->left);
        return rotateRight(t);
    }
    // Right Right 
    if (balance < -1 && balance_Factor(t->right) <= 0) {
        return rotateLeft(t);
    }
    // Right Left 
    if (balance < -1 && balance_Factor(t->right)>0) {
        t->right = rotateRight(t->right);
        return rotateLeft(t);
    }

    return t;

}


template<typename Key, typename Value>
void AVLTree<Key, Value>::insert(const Key& fileName, const Key& word, int frequency)
{
    root = insert(fileName, word, frequency, root);
}

template<typename Key, typename Value>
void AVLTree<Key, Value>::print()
{
    print(root);
}

template<typename Key, typename Value>
void AVLTree<Key, Value>::print(AVLNode<Key,Value>* t)
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
AVLNode<Key,Value>* AVLTree<Key, Value>::insert(const Key& fileName, const Key& word, int frequency, AVLNode<Key,Value>* node)
{

    if (!node) {
        return new AVLNode<Key,Value>(fileName, word, frequency);
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

    return Balance(node, word);
}

// Function to remove a word from the AVL tree
template<typename Key, typename Value>
void AVLTree<Key, Value>::remove(const Key& word) {
    root = remove(word, root);
}

// Function to remove
template<typename Key, typename Value>
AVLNode<Key,Value>* AVLTree<Key, Value>::remove(const Key& word, AVLNode<Key,Value>* node) {
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
                AVLNode<Key,Value>* temp;
                temp = node;
                node = node->left;
                delete node;
                return node;
            }
            else if (!node->left && node->right) {// only right child 
                AVLNode<Key,Value>* temp;
                temp = node;
                node = node->right;
                delete temp;
                return node;
            }
            else {// 2 child

                AVLNode<Key,Value>* temp;
                temp = minvaluenode(node->right);
                node->info.word = temp->info.word;
                node->right = remove(temp->info.word, node->right);


            }
        }
    }

    if (node == nullptr) {
        return node;
    }
    return Balance(node, toLowercase(word)); // call the function to balance the tree if necessary
}

// Perform right rotation
template<typename Key, typename Value>
AVLNode<Key,Value>* AVLTree<Key, Value>::rotateRight(AVLNode<Key,Value>* node)
{
    AVLNode<Key,Value>* new_node = node->left;
    node->left = new_node->right;
    new_node->right = node;
    // Update heigts (Only node and leftchild)
    node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
    new_node->height = max(getHeight(new_node->left), getHeight(new_node->right)) + 1;

    return new_node;
}

// Perform left rotation
template<typename Key, typename Value>
AVLNode<Key,Value>* AVLTree<Key, Value>::rotateLeft(AVLNode<Key,Value>* node)
{
    AVLNode<Key,Value>* new_node = node->right;
    node->right = new_node->left;
    new_node->left = node;

    // Update heigts (Only node and rightchild)
    node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
    new_node->height = max(getHeight(new_node->left), getHeight(new_node->right)) + 1;

    return new_node;
}

// Function to get the height of a node
template<typename Key, typename Value>
int AVLTree<Key, Value>::getHeight(AVLNode<Key,Value>* node)
{
    if (!node) {
        return 0;
    }
    else {
        return node->height;
    }
}

// To find balance factor if >1 then left heavy if <-1 then right heavy
template<typename Key, typename Value>
int AVLTree<Key, Value>::balance_Factor(AVLNode<Key,Value>* node)
{
    if (!node)
    {
        return 0;
    }
    else
    {
        return getHeight(node->left) - getHeight(node->right);
    }
}


template<typename Key, typename Value>
bool AVLTree<Key, Value>::Find(const Key& word,AVLNode<Key,Value>* node, map<string, int>& occurrences)
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
void AVLTree<Key, Value>::makeEmpty(AVLNode<Key,Value>*& t)
{
    if (t != nullptr)
    {
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
    }
    t = nullptr;
}

