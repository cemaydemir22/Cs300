#include <iostream>
#include "avl.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm> // for std::transform
using namespace std;
// Destructor
// Function to insert word occurrences into the AVL tree


// Utility function to convert string to lowercase
string toLowercase(string str) 
{
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}


template<typename Key, typename Value>
void AVLTree<Key, Value>::insert(const Key& fileName, const Key& word, int frequency) {
    root = insert(fileName, word, frequency, root);
}

template<typename Key, typename Value>
void AVLTree<Key,Value>::print(){
    print(root);
}

template<typename Key, typename Value>
void AVLTree<Key,Value>::print(AVLNode * t)
{
  if(t->left)
  {
     print(t->left);
  }
  cout<<t->info.word<<" ";
  if(t->right){
    print(t->right);
  }   
}

// Function to insert a node into the AVL tree
template<typename Key, typename Value>
typename AVLTree<Key, Value>::AVLNode* AVLTree<Key, Value>::insert(const Key& fileName, const Key& word, int frequency, AVLNode* t) {
    if (t == nullptr)
        return new AVLNode(fileName, word, frequency);
    else if (toLowercase(word) < toLowercase(t->info.word))
        t->left = insert(fileName, word, frequency, t->left);
    else if (toLowercase(t->info.word) < toLowercase(word))
        t->right = insert(fileName, word, frequency, t->right);
    else {
        t->info.fileOccurrences[fileName] += frequency; // Increment frequency if word already exists
        return t;
    }

    // Update height of current node
    t->height = max(getHeight(t->left), getHeight(t->right)) + 1;

    // Rebalance the tree
    int balance = getBalance(t);

    // Left Left Case
    if (balance > 1 && toLowercase(word) < toLowercase(t->left->info.word))
        return rotateRight(t);

    // Right Right Case
    if (balance < -1 && toLowercase(t->right->info.word) < toLowercase(word))
        return rotateLeft(t);

    // Left Right Case
    if (balance > 1 && toLowercase(t->left->info.word) < toLowercase(word)) {
        t->left = rotateLeft(t->left);
        return rotateRight(t);
    }

    // Right Left Case
    if (balance < -1 && toLowercase(word) < toLowercase(t->right->info.word)) {
        t->right = rotateRight(t->right);
        return rotateLeft(t);
    }

    return t;
}

// Function to remove a word from the AVL tree
template<typename Key, typename Value>
void AVLTree<Key, Value>::remove(const Key& word) {
    root = remove(word, root);
}

// Function to remove a node from the AVL tree
template<typename Key, typename Value>
typename AVLTree<Key, Value>::AVLNode* AVLTree<Key, Value>::remove(const Key& word, AVLNode* t) {
    if (t == nullptr)
        return t;

    if (toLowercase(word) < toLowercase(t->info.word))
        t->left = remove(word, t->left);
    else if (toLowercase(t->info.word) < toLowercase(word))
        t->right = remove(word, t->right);
    else {
        if (t->left == nullptr || t->right == nullptr) {
            AVLNode* temp = t->left ? t->left : t->right;
            if (temp == nullptr) {
                temp = t;
                t = nullptr;
            } else
                *t = *temp; // Copy the contents of temp to t
            delete temp;
            cout<< word << " has been REMOVED" << endl; // Print removal message
        } else {
            AVLNode* temp = t->right;
            while (temp->left != nullptr)
                temp = temp->left;
            t->info = temp->info;
            t->right = remove(temp->info.word, t->right);
        }
    }

    if (t == nullptr)
        return t;

    // Update height of current node
    t->height = 1 + max(getHeight(t->left), getHeight(t->right));

    // Rebalance the tree
    int balance = getBalance(t);

    // Left Left Case
    if (balance > 1 && getBalance(t->left) >= 0)
        return rotateRight(t);

    // Left Right Case
    if (balance > 1 && getBalance(t->left) < 0) {
        t->left = rotateLeft(t->left);
        return rotateRight(t);
    }

    // Right Right Case
    if (balance < -1 && getBalance(t->right) <= 0)
        return rotateLeft(t);

    // Right Left Case
    if (balance < -1 && getBalance(t->right) > 0) {
        t->right = rotateRight(t->right);
        return rotateLeft(t);
    }

    return t;
}


// Function to perform a right rotation
template<typename Key, typename Value>
typename AVLTree<Key, Value>::AVLNode* AVLTree<Key, Value>::rotateRight(AVLNode* node) {
    AVLNode* leftChild = node->left;
    AVLNode* leftRightChild = leftChild->right;

    // Perform rotation
    leftChild->right = node;
    node->left = leftRightChild;

    // Update heights
    node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
    leftChild->height = max(getHeight(leftChild->left), getHeight(leftChild->right)) + 1;

    return leftChild;
}

// Function to perform a left rotation
template<typename Key, typename Value>
typename AVLTree<Key, Value>::AVLNode* AVLTree<Key, Value>::rotateLeft(AVLNode* node) 
{
    AVLNode* rightChild = node->right;
    AVLNode* rightLeftChild = rightChild->left;

    // Perform rotation
    rightChild->left = node;
    node->right = rightLeftChild;

    // Update heights
    node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
    rightChild->height = max(getHeight(rightChild->left), getHeight(rightChild->right)) + 1;

    return rightChild;
}

// Function to get the height of a node
template<typename Key, typename Value>
int AVLTree<Key, Value>::getHeight(AVLNode* node) {
    return (node == nullptr) ? 0 : node->height;
}

// Function to get the balance factor of a node
template<typename Key, typename Value>
int AVLTree<Key, Value>::getBalance(AVLNode* node) {
    return (node == nullptr) ? 0 : getHeight(node->left) - getHeight(node->right);
}

// Corrected searchWords function
template<typename Key, typename Value>
void AVLTree<Key, Value>::searchWords(const vector<Key>& words) 
{
    map<string, map<string, vector<string>>> occurrencesByFile; // Map to store occurrences by file name

    // Iterate over each word in the query
    for (size_t i = 0; i < words.size(); ++i) {
        if (toLowercase(words[i]) == "remove" && i + 1 < words.size()) 
        { // REMOVE or remove
            remove(words[i + 1]); // Remove the specified word
            ++i; // Skip the next word
            continue;
        }

        map<string, int> occurrences;

        // Search for the word in the tree and get its occurrences in each file
        if (search(toLowercase(words[i]), root, occurrences)) {
            // Update occurrencesByFile with the word occurrences for each file
            for (const auto& pair : occurrences) 
            {
                occurrencesByFile[pair.first][words[i]].push_back(to_string(pair.second) + " times.");
            }
        }
        else 
        {
            cout << "No document contains the given query" << endl;
        }
    }

    // Output occurrences grouped by file name
    for (const auto& filePair : occurrencesByFile) 
    {
        cout << "in Document " << filePair.first << ", ";
        for (const auto& wordPair : filePair.second) {
            cout << wordPair.first << " found ";
            for (size_t i = 0; i < wordPair.second.size(); ++i) {
                cout << wordPair.second[i];
                if (i != wordPair.second.size() - 1) {
                    cout << ", ";
                }
            }
            
        }
        cout << endl;
    }
}

// Helper function to search for a word in the AVL tree
template<typename Key, typename Value>
bool AVLTree<Key, Value>::search(const Key& word, AVLNode* t, map<string, int>& occurrences) {
    if (t == nullptr) {
        return false;
    }

    if (word < toLowercase(t->info.word))
        return search(word, t->left, occurrences);
    else if (toLowercase(t->info.word) < word)
        return search(word, t->right, occurrences);
    else {
        occurrences = t->info.fileOccurrences;
        return true;
    }
}

// Utility function to deallocate memory of the AVL tree
template<typename Key, typename Value>
void AVLTree<Key, Value>::makeEmpty(AVLNode*& t) 
{
    if (t != nullptr) {
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
    }
    t = nullptr;
}
